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
#include "corescope/hdr/gnb/gnb_rrc_ue.h"

namespace corescope {

gnb_rrc::ue::ue(srslog::basic_logger& logger_, uint16_t rnti_, ue_rrc_interface_gnb* ue_rrc_nr_, gnb_rrc* parent_) :
  logger(logger_), rnti(rnti_), ue_rrc_nr(ue_rrc_nr_), parent(parent_){};

void gnb_rrc::ue::write_nas(srsran::unique_byte_buffer_t sdu)
{
  if (ue_rrc_nr == nullptr) {
    logger.error("RRC not initialized");
    return;
  }
  ue_rrc_nr->write_nas(std::move(sdu));
  return;
};

void gnb_rrc::ue::release_connection()
{
  release_bearers();
  return;
};

void gnb_rrc::ue::establish_rrc_bearer(uint16_t pdu_session_id, srsran::const_span<uint8_t> nas_pdu, uint32_t lcid)
{
  // Send NAS message to UE
  srsran::unique_byte_buffer_t pdu = srsran::make_byte_buffer();
  if (pdu == nullptr) {
    logger.error("Couldn't allocate PDU in %s().", __FUNCTION__);
    return;
  }
  pdu.get()->clear();
  pdu.get()->append_bytes((uint8_t*)nas_pdu.data(), nas_pdu.size());

  uint16_t tmp_rnti = rnti;
  auto     task     = [this, tmp_rnti](srsran::unique_byte_buffer_t& pdu_) {
    parent->write_dl_info(tmp_rnti, std::move(pdu_));
  };
  parent->gnb_rrc_task_queue.push(std::bind(task, std::move(pdu)));

  if (add_bearer_setup(lcid, pdu_session_id) != SRSRAN_SUCCESS) {
    logger.error("Failed to add pdu session");
    return;
  }

  // Establishing UE bearer
  ue_rrc_nr->add_bearer(pdu_session_id, lcid);
  return;
};

int gnb_rrc::ue::add_bearer_setup(uint32_t lcid, uint32_t pdu_session_id)
{
  if (bearers.find(lcid) != bearers.end()) {
    return SRSRAN_ERROR;
  }
  bearers.insert(std::make_pair(lcid, pdu_session_id));
  return SRSRAN_SUCCESS;
}

int gnb_rrc::ue::release_bearers()
{
  bearers.clear();
  return SRSRAN_SUCCESS;
}

int gnb_rrc::ue::allocate_lcid()
{
  for (uint32_t i = 1; i <= UINT32_MAX; i++) {
    if (bearers.find(i) == bearers.end()) {
      return i;
    }
  }
  return -1;
}

} // namespace corescope
