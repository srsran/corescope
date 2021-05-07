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
#ifndef CORESCOPE_UE_RRC_NR_H
#define CORESCOPE_UE_RRC_NR_H

#include "corescope/hdr/corescope_interfaces.h"

#include "srsran/interfaces/ue_interfaces.h"
#include "srsran/interfaces/ue_nas_interfaces.h"

#include "srsran/asn1/rrc.h"
#include "srsran/common/bcd_helpers.h"
#include "srsran/common/buffer_pool.h"
#include "srsran/common/int_helpers.h"
#include "srsran/srslog/srslog.h"

using namespace srsran;

namespace corescope {

class ue_rrc_nr : public srsue::rrc_nr_interface_nas_5g, public ue_rrc_interface_gnb
{
public:
  ue_rrc_nr(srslog::basic_logger& logger_, gnb_rrc_interface_ue* gnb_rrc_, ue_interface_gnb* ue_itf_gnb_);
  ~ue_rrc_nr()                = default;
  ue_rrc_nr(const ue_rrc_nr&) = delete;
  ue_rrc_nr& operator=(const ue_rrc_nr&) = delete;

  int init(srsue::nas_5g_interface_rrc_nr* nas_, ue_pdcp_interface_ue_rrc* ue_pdcp_);

  // Interface for NAS
  int      write_sdu(unique_byte_buffer_t sdu);
  int      connection_request(srsran::nr_establishment_cause_t, srsran::unique_byte_buffer_t);
  bool     is_connected() { return connected; }
  int      add_bearer(uint32_t pdu_session_id, uint32_t lcid);
  uint16_t get_mcc();
  uint16_t get_mnc();

  // Interface for GNB RRC
  int write_nas(unique_byte_buffer_t sdu);

private:
  srslog::basic_logger& logger;

  srsue::nas_5g_interface_rrc_nr* nas_5g;

  gnb_rrc_interface_ue*     gnb_rrc;
  ue_pdcp_interface_ue_rrc* pdcp;

  // Interface used during the registration
  ue_interface_gnb* ue_itf_gnb;

  uint16_t rnti      = SRSRAN_INVALID_RNTI;
  bool     connected = false;

  // LCID <-> PDU sessions
  std::map<uint32_t, uint32_t> bearers;
};
} // namespace corescope

#endif // CORESCOPE_UE_RRC_NR_H