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

#ifndef CORESCOPE_USER_H
#define CORESCOPE_USER_H

#include "corescope/hdr/clock.h"
#include "corescope/hdr/corescope_interfaces.h"
#include "srsran/common/stack_procedure.h"
#include "srsran/common/task_scheduler.h"
#include "srsue/hdr/stack/upper/gw.h"
#include "srsue/hdr/stack/upper/nas.h"
#include "srsue/hdr/stack/upper/nas_config.h"
#include "srsue/hdr/stack/upper/usim.h"

#include "ue_pdcp.h"
#include "ue_rrc_nr.h"

namespace corescope {

// internal representation of a single UE

class ue : public ue_interface_gnb, public srsue::stack_interface_gw
{
public:
  ue(uint32_t id_, gnb_interface_ue* gnb_, srsran::task_sched_handle task_sched_, srslog::sink& sink_);
  ~ue();

  int init(const ue_args_t& _args);

  void get_args(ue_args_t& args);
  void get_metrics(ue_metrics_t& ue_metrics_);
  void switch_on();
  void switch_off();

  // Stack Interface for GW stack_interface_gw
  bool is_registered() { return nas_5g.is_registered(); }
  bool start_service_request() { return nas_5g.start_service_request(); }
  void write_sdu(uint32_t eps_bearer_id, srsran::unique_byte_buffer_t sdu)
  {
    pdcp.write_sdu(eps_bearer_id, std::move(sdu));
  }
  bool has_active_radio_bearer(uint32_t eps_bearer_id) { return pdcp.is_pdu_session_id_enabled(eps_bearer_id); }

  // ue_interface_gnb
  // UE RRC NR interface for GNB RRC: ue_rrc_interface_gnb
  int write_nas(srsran::unique_byte_buffer_t sdu) { return rrc_nr.write_nas(std::move(sdu)); }
  int add_bearer(uint32_t pdu_session_id, uint32_t lcid) { return rrc_nr.add_bearer(pdu_session_id, lcid); }

  // UE PDCP interface for GNB PDCP: ue_pdcp_interface_gnb
  void write_to_gw(uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu)
  {
    pdcp.write_to_gw(pdu_session_id, std::move(sdu));
  }

  void stop();

  void run_tti(uint32_t tti);

private:
  /* logs */
  srslog::basic_logger& user_logger;
  srslog::basic_logger& usim_logger;
  srslog::basic_logger& pdcp_logger;
  srslog::basic_logger& gw_logger;
  srslog::basic_logger& nas_5g_logger;
  srslog::basic_logger& rrc_nr_logger;

  /* config */
  uint32_t  index                = 0;
  ue_args_t args                 = {};
  bool      initiated            = false;
  bool      active               = false;
  bool      switch_off_triggered = false;

  /* stack components */
  srsue::nas_5g nas_5g;
  srsue::gw     gw;
  srsue::usim   usim;
  ue_rrc_nr     rrc_nr;
  ue_pdcp       pdcp;

  gnb_interface_ue* gnb_ptr;

  /* tasks that are resumed on run_tti() */
  srsran::proc_manager_list_t           callbacks;
  srsran::task_sched_handle             task_sched;
  srsran::task_multiqueue::queue_handle ue_task_queue;

  srsran::nas_pcap ue_nas_5g_pcap;
  void             reset();
};
} // namespace corescope
#endif // CORESCOPE_USER_H