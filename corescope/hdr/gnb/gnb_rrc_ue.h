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

#ifndef CORESCOPE_GNB_RRC_UE_H
#define CORESCOPE_GNB_RRC_UE_H

#include "corescope/hdr/gnb/gnb_rrc.h"

using namespace srsran;

namespace corescope {
class gnb_rrc::ue
{
public:
  ue(srslog::basic_logger& logger_, uint16_t rnti_, ue_rrc_interface_gnb* ue_rrc_nr_, gnb_rrc* parent_);
  ~ue(){};

  void write_nas(srsran::unique_byte_buffer_t sdu);
  void establish_rrc_bearer(uint16_t                    pdu_session_id,
                            srsran::const_span<uint8_t> nas_pdu,
                            uint32_t                    lcid); // TODO QoS parameter
  void release_connection();

  // Bearer handling
  int allocate_lcid();
  int add_bearer_setup(uint32_t lcid, uint32_t pdu_session_id);
  int release_bearers();

private:
  srslog::basic_logger& logger;
  uint16_t              rnti;
  gnb_rrc*              parent;
  ue_rrc_interface_gnb* ue_rrc_nr;
  // mapping of lcid to bearer
  std::map<uint32_t, uint32_t> bearers;
};
} // namespace corescope
#endif