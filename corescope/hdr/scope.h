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

#ifndef CORESCOPE_SCOPE_H
#define CORESCOPE_SCOPE_H

#include "scope_cfg.h"

#include "corescope/hdr/gnb/gnb.h"
#include "corescope/hdr/rest_api/scope_rest.h"
#include "corescope/hdr/ue/ue.h"

#include "srsran/common/bcd_helpers.h"
#include "srsran/common/block_queue.h"
#include "srsran/common/common.h"
#include "srsran/common/int_helpers.h"
#include "srsran/common/log_helper.h"
#include "srsran/common/stack_procedure.h"
#include "srsran/common/standard_streams.h"
#include "srsran/interfaces/enb_gtpu_interfaces.h"
#include "srsran/interfaces/enb_interfaces.h"
#include "srsran/interfaces/ue_interfaces.h"

#include <atomic>
#include <chrono>
#include <fstream>
#include <mutex>
#include <thread>

#include <boost/algorithm/string.hpp>

namespace corescope {

class scope : public thread, public tick_subscriber, public scope_interface_rest
{
public:
  scope(srslog::sink& log_sink);
  ~scope();
  scope(const scope&) = delete;
  scope& operator=(const scope&) = delete;

  int init(all_args_t _args);

  int switch_off();

  /* thread */
  void run_thread();
  void stop();
  void stop_impl();

  void run_tti(uint32_t tti);
  void tti_clock_impl(uint32_t tti);

  /* Rest API */
  int get_gnb_status(uint32_t gnb_id, gnb_metrics_t& gnb_metic);
  int get_gnb_config(uint32_t gnb_id, gnb_args_t& gnb_args);
  int add_ue(const ue_args_t& ue_args, uint32_t& ue_id);
  int get_ue_status(uint32_t ue_id, ue_metrics_t& ue_metrics);
  int get_ue_config(uint32_t ue_id, ue_args_t& ue_args);
  int ue_switch_on(uint32_t gnb_id, uint32_t ue_id);
  int ue_switch_off(uint32_t gnb_id, uint32_t ue_id);
  int delete_ue(uint32_t ue_id);

private:
  /* logs */
  srslog::basic_logger& scope_log;
  srslog::basic_logger& rest_log;

  /* config */
  all_args_t args;

  srsran::byte_buffer_pool* pool;
  std::atomic<bool>         started;

  // task handling
  srsran::task_scheduler    task_sched;
  srsran::task_queue_handle scope_task_queue, gtpu_task_queue, sync_task_queue;

  /* REST API */
  scope_rest rest_api;

  // gnb
  corescope::gnb gnb;

  // users
  std::map<uint32_t, std::shared_ptr<ue> > users;
};

} // namespace corescope

#endif // CORESCOPE_SCOPE_H