/**
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * This file is part of CoreScope.
 *
 * CoreScope is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * CoreScope is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */
#include "corescope/hdr/scope.h"
#include "srsran/common/string_helpers.h"

namespace corescope {

/* CoreScope holds eNB component and one or more UEs */
scope::scope(srslog::sink& log_sink) :
  thread("SCOPE"),
  task_sched(512, 128),
  scope_log(srslog::fetch_basic_logger("SCOPE", log_sink, false)),
  gnb(&task_sched, log_sink),
  rest_log(srslog::fetch_basic_logger("REST", log_sink, false)),
  rest_api(srslog::fetch_basic_logger("REST", log_sink, false), this),
  args({})
{
  pool = srsran::byte_buffer_pool::get_instance();

  // add sync queue
  sync_task_queue  = task_sched.make_task_queue();
  scope_task_queue = task_sched.make_task_queue();
  gtpu_task_queue  = task_sched.make_task_queue();
}

scope::~scope() {}

int scope::init(all_args_t _args)
{
  int ret = SRSRAN_SUCCESS;
  args    = _args;

  /* set up logs */
  scope_log.set_level(srslog::str_to_basic_level(args.log.main_level));
  scope_log.set_hex_dump_max_size(args.log.main_hex_limit);
  rest_log.set_level(srslog::str_to_basic_level(args.rest.log_level));
  rest_log.set_hex_dump_max_size(args.rest.log_hex_limit);

  if (rest_api.init(args.rest) != SRSRAN_SUCCESS) {
    scope_log.error("Couldn't initialize REST API");
    ret = SRSRAN_ERROR;
  }

  /* init gNB components */
  if (gnb.init(args.gnb) != SRSRAN_SUCCESS) {
    scope_log.error("Couldn't initialize PDCP");
  }

  if (ret == SRSRAN_SUCCESS) {
    srsran::console("\n==== CoreScope started ===\n");
    started = true;
    start(DEFAULT_PRIORITY);
  } else {
    stop();
  }

  return ret;
}

void scope::run_thread()
{
  while (started) {
    task_sched.run_next_task();
  }
}

void scope::run_tti(uint32_t tti)
{
  sync_task_queue.push([this, tti]() { tti_clock_impl(tti); });
}

void scope::tti_clock_impl(uint32_t tti)
{
  task_sched.tic();
  for (auto user : users) {
    user.second.get()->run_tti(tti);
  }
}

void scope::stop()
{
  scope_log.info("Stopping...");
  if (started) {
    scope_task_queue.push([this]() { stop_impl(); });
    wait_thread_finish();
  }
}

void scope::stop_impl()
{
  started = false;

  rest_api.stop();
  gnb.stop();
  // ue objects are deallocated
  for (auto ue : users) {
    ue.second->stop();
  }
  task_sched.stop();
  // users.clear();
}

/*********************************
 * REST API
 *********************************/
int scope::get_gnb_status(uint32_t gnb_id, gnb_metrics_t& gnb_metic)
{
  if (gnb_id != 0) {
    scope_log.warning("gNodeB id %d not found", gnb_id);
    return SRSRAN_ERROR;
  }
  gnb.get_metrics(gnb_metic);
  return SRSRAN_SUCCESS;
}

int scope::get_gnb_config(uint32_t gnb_id, gnb_args_t& gnb_args)
{
  if (gnb_id != 0) {
    scope_log.warning("gNodeB id %d not found", gnb_id);
    return SRSRAN_ERROR;
  }
  gnb.get_args(gnb_args);
  return SRSRAN_SUCCESS;
}

int scope::add_ue(const ue_args_t& ue_args_, uint32_t& ue_index)
{
  ue_args_t ue_args = ue_args_;

  scope_log.info("Adding UE with IMSI %s", ue_args.usim.imsi.c_str());

  ue_index = users.size();

  srsran::replace(ue_args.log.filename_base, "$id", std::to_string(ue_index));
  srsran::replace(ue_args.pcap.filename, "$id", std::to_string(ue_index));
  srsran::replace(ue_args.gw.tun_dev_name, "$id", std::to_string(ue_index));
  srsran::replace(ue_args.gw.netns, "$id", std::to_string(ue_index));

  srslog::sink&                  s = srslog::fetch_file_sink(ue_args.log.filename_base);
  std::unique_ptr<corescope::ue> user;
  user.reset(new corescope::ue{ue_index, &gnb, &task_sched, s});

  if (user->init(ue_args) != SRSRAN_SUCCESS) {
    scope_log.warning("Failed to init user %d", ue_index);
  } else {
    scope_log.info("Succesfully init UE %d with IMSI %s", ue_index, ue_args.usim.imsi.c_str());
  }

  // Store UE in users map
  users[ue_index] = std::move(user);

  return SRSRAN_SUCCESS;
}

int scope::get_ue_status(uint32_t ue_id, ue_metrics_t& ue_metrics)
{
  // Check if UE index already exisits
  auto user_it = users.find(ue_id);
  if (user_it == users.end()) {
    scope_log.warning("Does UE %d not exists", ue_id);
    return SRSRAN_ERROR;
  }
  user_it->second->get_metrics(ue_metrics);
  return SRSRAN_SUCCESS;
}

int scope::get_ue_config(uint32_t ue_id, ue_args_t& ue_args)
{
  // Check if UE index already exisits
  auto user_it = users.find(ue_id);
  if (user_it == users.end()) {
    scope_log.warning("Does UE %d not exists", ue_id);
    return SRSRAN_ERROR;
  }
  user_it->second->get_args(ue_args);
  return SRSRAN_SUCCESS;
}

int scope::ue_switch_on(uint32_t gnb_id, uint32_t ue_id)
{
  // Check if UE index already exisits
  auto user_it = users.find(ue_id);
  if (user_it == users.end()) {
    scope_log.warning("Does UE %d not exists", ue_id);
    return SRSRAN_ERROR;
  }
  user_it->second->switch_on();
  return SRSRAN_SUCCESS;
}

int scope::ue_switch_off(uint32_t gnb_id, uint32_t ue_id)
{
  // Check if UE index already exisits
  auto user_it = users.find(ue_id);
  if (user_it == users.end()) {
    scope_log.warning("Does UE %d not exists", ue_id);
    return SRSRAN_ERROR;
  }
  user_it->second->switch_off();
  return SRSRAN_SUCCESS;
}

int scope::delete_ue(uint32_t ue_id)
{
  return SRSRAN_SUCCESS;
}

int scope::switch_off()
{
  if (!started) {
    scope_log.warning("Switch off while not running");
    return SRSRAN_ERROR;
  }
  return SRSRAN_SUCCESS;
}

} // namespace corescope