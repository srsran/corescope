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

#ifndef CORESCOPE_GNB_RRC_H
#define CORESCOPE_GNB_RRC_H

#include "corescope/hdr/corescope_interfaces.h"
#include "corescope/hdr/scope_cfg.h"
#include "srsRAN/srsenb/hdr/common/rnti_pool.h"
#include "srsran/interfaces/gnb_rrc_nr_interfaces.h"

namespace corescope {
class gnb_rrc : public gnb_rrc_interface_ue, public srsenb::rrc_interface_ngap_nr
{
public:
  gnb_rrc(srsran::task_sched_handle task_sched_, srslog::basic_logger& logger_);
  ~gnb_rrc();

  int         init(gnb_rrc_nr_t args_, srsenb::ngap_interface_rrc_nr* ngap_, pdcp_gnb_interface_rrc_gnb* pdcp_);
  int         nas_to_ngap(uint16_t rnti, srsran::unique_byte_buffer_t sdu);
  int         initial_ue(ue_interface_gnb*                ue_,
                         srsran::nr_establishment_cause_t cause,
                         srsran::unique_byte_buffer_t     sdu,
                         uint16_t&                        rnti);
  cell_info_t get_cell_info();

  // NGAP Interface
  int  ue_set_security_cfg_key(uint16_t rnti, const asn1::fixed_bitstring<256, false, true>& key);
  int  ue_set_bitrates(uint16_t rnti, const asn1::ngap_nr::ue_aggregate_maximum_bit_rate_s& rates);
  int  ue_set_security_cfg_capabilities(uint16_t rnti, const asn1::ngap_nr::ue_security_cap_s& caps);
  int  start_security_mode_procedure(uint16_t rnti);
  int  establish_rrc_bearer(uint16_t rnti, uint16_t pdu_session_id, srsran::const_byte_span nas_pdu, uint32_t lcid);
  int  release_bearers(uint16_t rnti);
  int  set_aggregate_max_bitrate(uint16_t rnti, const asn1::ngap_nr::ue_aggregate_maximum_bit_rate_s& rates);
  int  allocate_lcid(uint16_t rnti);
  void write_dl_info(uint16_t rnti, srsran::unique_byte_buffer_t sdu);
  void write_ul_info(uint16_t rnti, srsran::unique_byte_buffer_t sdu);

private:
  class ue; // forward declation of private ue class

  srslog::basic_logger& logger;

  srsran::task_sched_handle task_sched;
  srsran::task_queue_handle gnb_rrc_task_queue;

  // RRC NR Users
  std::map<uint16_t, srsenb::unique_rnti_ptr<ue> > users;

  int add_user(ue_rrc_interface_gnb* ue_rrc_nr, uint16_t& rnti_);

  // NGAP interface
  srsenb::ngap_interface_rrc_nr*         ngap;
  corescope::pdcp_gnb_interface_rrc_gnb* pdcp;

  cell_info_t cell_info;
};
} // namespace corescope
#endif