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
#ifndef CORESCOPE_GNB_UP_PDCP_H
#define CORESCOPE_GNB_UP_PDCP_H

#include "srsran/interfaces/enb_gtpu_interfaces.h"
#include "srsran/interfaces/enb_interfaces.h"
#include "srsran/interfaces/enb_pdcp_interfaces.h"

#include "corescope/hdr/corescope_interfaces.h"
#include "corescope/hdr/ue/ue_pdcp.h"

#include "srsran/asn1/rrc.h"
#include "srsran/common/bcd_helpers.h"
#include "srsran/common/buffer_pool.h"
#include "srsran/common/int_helpers.h"
#include "srsran/common/mac_pcap.h"
#include "srsran/common/threads.h"

#include <atomic>

using namespace srsran;

namespace corescope {

class gnb_pdcp : public srsenb::pdcp_interface_gtpu, public pdcp_gnb_interface_rrc_gnb, public gnb_pdcp_interface_ue
{
public:
  gnb_pdcp(srslog::basic_logger& logger_);
  ~gnb_pdcp();
  gnb_pdcp(const gnb_pdcp&) = delete;
  gnb_pdcp& operator=(const gnb_pdcp&) = delete;

  int  init(srsenb::gtpu_interface_pdcp* gtpu_);
  void stop();

  /* pdcp_interface_gtpu */
  void write_sdu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu, int pdcp_sn = -1);
  std::map<uint32_t, srsran::unique_byte_buffer_t> get_buffered_pdus(uint16_t rnti, uint32_t lcid)
  {
    return std::map<uint32_t, srsran::unique_byte_buffer_t>{};
  };
  /* gnb_up_pdcp_interface_ue_up_pdcp */
  void write_to_gtpu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu);
  int  register_ue(uint16_t rnti, ue_pdcp_interface_gnb* ue_pdcp_);

private:
  srsenb::gtpu_interface_pdcp* gtpu;
  srslog::basic_logger&        logger;

  pthread_rwlock_t users_rwlock;
  /* array of active users, mapping <RNTI, ue_pdcp> */
  std::map<uint16_t, ue_pdcp_interface_gnb*> users;
};
} // namespace corescope

#endif // SRSCoreScope_GNB_UP_PDCP_H