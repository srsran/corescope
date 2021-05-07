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

#include "corescope/hdr/ue/ue.h"

namespace corescope {
ue::ue(uint32_t id_, gnb_interface_ue* gnb_, srsran::task_sched_handle task_sched_, srslog::sink& sink_) :
  user_logger(fetch_basic_logger("USER" + std::to_string(id_), sink_, false)),
  usim_logger(fetch_basic_logger("USIM " + std::to_string(id_), sink_, false)),
  gw_logger(fetch_basic_logger("GW " + std::to_string(id_), sink_, false)),
  nas_5g_logger(fetch_basic_logger("NAS " + std::to_string(id_), sink_, false)),
  rrc_nr_logger(fetch_basic_logger("RRC " + std::to_string(id_), sink_, false)),
  pdcp_logger(fetch_basic_logger("PDCP " + std::to_string(id_), sink_, false)),
  usim(usim_logger),
  task_sched(task_sched_),
  gw(gw_logger),
  pdcp(pdcp_logger, gnb_),
  index(id_),
  gnb_ptr(gnb_),
  nas_5g(nas_5g_logger, task_sched_),
  rrc_nr(rrc_nr_logger, gnb_, this)
{
  ue_task_queue = task_sched.make_task_queue();
}

ue::~ue()
{
  reset();
}

int ue::init(const ue_args_t& _args)
{
  int rtn = SRSRAN_SUCCESS;
  args    = _args;

  pdcp_logger.set_level(srslog::str_to_basic_level(args.log.pdcp_level));
  pdcp_logger.set_hex_dump_max_size(args.log.pdcp_hex_limit);
  pdcp_logger.set_level(srslog::str_to_basic_level(args.log.pdcp_level));
  pdcp_logger.set_hex_dump_max_size(args.log.pdcp_hex_limit);
  rrc_nr_logger.set_level(srslog::str_to_basic_level(args.log.rrc_level));
  rrc_nr_logger.set_hex_dump_max_size(args.log.rrc_hex_limit);
  usim_logger.set_level(srslog::str_to_basic_level(args.log.usim_level));
  usim_logger.set_hex_dump_max_size(args.log.usim_hex_limit);
  nas_5g_logger.set_level(srslog::str_to_basic_level(args.log.nas_level));
  nas_5g_logger.set_hex_dump_max_size(args.log.nas_hex_limit);

  /* initialize components */
  if (rrc_nr.init(&nas_5g, &pdcp) != SRSRAN_SUCCESS) {
    user_logger.error("Failed to init RRC");
    rtn = SRSRAN_ERROR;
  }

  if (pdcp.init(&gw) != SRSRAN_SUCCESS) {
    user_logger.error("Failed to init PDCP");
    rtn = SRSRAN_ERROR;
  }

  if (usim.init(&args.usim) != SRSRAN_SUCCESS) {
    user_logger.error("Failed to init usim");
    rtn = SRSRAN_ERROR;
  }

  if (nas_5g.init(&usim, &rrc_nr, &gw, args.nas_5g) != SRSRAN_SUCCESS) {
    user_logger.error("Failed to init nas 5G");
    rtn = SRSRAN_ERROR;
  }

  if (args.pcap.enable) {
    if (ue_nas_5g_pcap.open(args.pcap.filename.c_str(), 0, srsran_rat_t::nr) == SRSRAN_SUCCESS) {
      ue_nas_5g_pcap.enable();
      nas_5g.start_pcap(&ue_nas_5g_pcap);
    } else {
      user_logger.warning("Failed to open NAS pcap %s", args.pcap.filename.c_str());
    }
  }

  if (gw.init(args.gw, this) != SRSRAN_SUCCESS) {
    user_logger.error("Failed to init gw");
    rtn = SRSRAN_ERROR;
  }
  initiated = true;
  return rtn;
}

void ue::run_tti(uint32_t tti)
{
  if (active == true) {
    callbacks.run();
    nas_5g.run_tti();
  }
  if (switch_off_triggered) {
    active = false;
  }
}

void ue::get_metrics(ue_metrics_t& ue_metrics_)
{
  nas_5g.get_metrics(ue_metrics_.nas_5g);
  ue_metrics_.initiated = initiated;
  ue_metrics_.active    = active;
}

void ue::get_args(ue_args_t& ue_args_)
{
  ue_args_ = args;
}

// Used to stop/pause the entity
void ue::reset()
{
  if (initiated) {
    user_logger.debug("Reset UE %d", index);
  }
  initiated = false;
  active    = false;
}

void ue::stop()
{
  usim.stop();
  gw.stop();
  nas_5g.stop();
}

void ue::switch_on()
{
  if (initiated == false) {
    user_logger.warning("UE is not correctly initiated. Can not be switched on.");
  }
  active               = true;
  switch_off_triggered = false;
  ue_task_queue.try_push([this]() { nas_5g.switch_on(); });
}

void ue::switch_off()
{
  // generate deregistration request with switch-off flag
  switch_off_triggered = true;
  ue_task_queue.try_push([this]() { nas_5g.switch_off(); });
}

} // namespace corescope