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
#include "corescope/hdr/ue/ue_pdcp.h"

namespace corescope {

ue_pdcp::ue_pdcp(srslog::basic_logger& logger_, gnb_pdcp_interface_ue* gnb_pdcp_) :
  gnb_pdcp(gnb_pdcp_), logger(logger_), gw(nullptr), has_rnti(false), rnti(0x0)
{}

ue_pdcp::~ue_pdcp() = default;

int ue_pdcp::init(srsue::gw_interface_pdcp* gw_)
{
  gw       = gw_;
  has_rnti = false;
  rnti     = SRSRAN_INVALID_RNTI;
  return SRSRAN_SUCCESS;
}

void ue_pdcp::write_sdu(uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu)
{
  logger.debug(
      "Incoming data from GW, moving to eNB PDCP. RNTI=0x%x, lcid=%i, %i bytes", rnti, pdu_session_id, sdu->N_bytes);
  if (!is_pdu_session_id_enabled(pdu_session_id)) {
    logger.error("Received data from GW, but data bearer not set up. Discarding data. lcid=%i, %i bytes",
                 pdu_session_id,
                 sdu->N_bytes);
    return;
  }
  gnb_pdcp->write_to_gtpu(rnti, pdu_session_id, std::move(sdu));
};

bool ue_pdcp::is_pdu_session_id_enabled(uint32_t pdu_session_id)
{
  logger.info("Has RNTI: %s; pdu_session %d", has_rnti ? "true" : "false", bearers.count(pdu_session_id));
  return has_rnti && bearers.count(pdu_session_id) > 0;
};

void ue_pdcp::write_to_gw(uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu)
{
  if (!is_pdu_session_id_enabled(pdu_session_id)) {
    logger.debug("Discarding data for inactive bearer: pdu_session_id=%i, %i bytes", pdu_session_id, sdu->N_bytes);
    return;
  }
  logger.debug("Incoming data from eNB PDCP, writing to GW. pdu_session_id=%i, %i bytes", pdu_session_id, sdu->N_bytes);

  gw->write_pdu(pdu_session_id, std::move(sdu));
}

void ue_pdcp::set_rnti(uint16_t rnti_)
{
  logger.info("Set RNTI=0x%x", rnti_);

  rnti = rnti_;
  if (rnti_ == SRSRAN_INVALID_RNTI) {
    has_rnti = false;
  } else {
    has_rnti = true;
  }
}

void ue_pdcp::add_bearer(uint32_t pdu_session_id, uint32_t lcid)
{
  logger.info("Adding bearer: RNTI=0x%x, lcid=%i, pdu_session_id=%i, \n", rnti, lcid, pdu_session_id);

  if (bearers.count(pdu_session_id) == 0) {
    bearers[pdu_session_id] = lcid;
  } else {
    logger.info(
        "Cannot add bearer, already registered. RNTI=0x%x, lcid=%i, pdu_session_id=%i\n", rnti, lcid, pdu_session_id);
  }
}

void ue_pdcp::release_connection()
{
  logger.info("Releasing Connection");

  rnti     = 0;
  has_rnti = false;
}
} // namespace corescope