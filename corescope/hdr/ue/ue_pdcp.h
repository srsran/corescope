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
#ifndef CORESCOPE_UE_PDCP_H
#define CORESCOPE_UE_PDCP_H

#include "corescope/hdr/corescope_interfaces.h"
#include "corescope/hdr/gnb/gnb_pdcp.h"

#include "srsran/asn1/rrc.h"
#include "srsran/common/bcd_helpers.h"
#include "srsran/common/buffer_pool.h"
#include "srsran/common/int_helpers.h"
#include "srsran/common/mac_pcap.h"
#include "srsran/interfaces/ue_gw_interfaces.h"
#include "srsran/interfaces/ue_interfaces.h"
#include "srsran/interfaces/ue_pdcp_interfaces.h"
#include "srsran/srslog/srslog.h"

using namespace srsran;

namespace corescope {

class ue_pdcp : public ue_pdcp_interface_gnb, public ue_pdcp_interface_ue_rrc
{
public:
  ue_pdcp(srslog::basic_logger& logger_, gnb_pdcp_interface_ue* _enb_pdcp);
  ~ue_pdcp();

  int init(srsue::gw_interface_pdcp* gw_);

  void write_sdu(uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu);

  bool is_pdu_session_id_enabled(uint32_t pdu_session_id);

  //  UE PDCP interface for gNodeB PDCP: ue_pdcp_interface_gnb
  void write_to_gw(uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu);

  // UE PDCP interface for UE RRC: ue_pdcp_interface_ue_rrc
  void set_rnti(uint16_t _rnti);
  void add_bearer(uint16_t                                           rnti,
                  uint32_t                                           eps_bearer_id,
                  uint32_t                                           lcid,
                  const asn1::bounded_bitstring<1, 160, true, true>& addr);
  void add_bearer(uint32_t eps_bearer_id, uint32_t lcid);
  void release_connection();

private:
  srsue::gw_interface_pdcp* gw;
  gnb_pdcp_interface_ue*    gnb_pdcp;
  srslog::basic_logger&     logger;

  // LCID, PDU Session ID
  std::map<uint32_t, uint32_t> bearers;

  uint16_t rnti;
  bool     has_rnti;
};

} // namespace corescope

#endif // CORESCOPE_UE_PDCP_H