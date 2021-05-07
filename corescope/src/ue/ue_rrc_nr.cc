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
#include "corescope/hdr/ue/ue_rrc_nr.h"

namespace corescope {

ue_rrc_nr::ue_rrc_nr(srslog::basic_logger& logger_, gnb_rrc_interface_ue* gnb_rrc_, ue_interface_gnb* ue_itf_gnb_) :
  logger(logger_), gnb_rrc(gnb_rrc_), ue_itf_gnb(ue_itf_gnb_)
{}

int ue_rrc_nr::init(srsue::nas_5g_interface_rrc_nr* nas_5g_, ue_pdcp_interface_ue_rrc* ue_pdcp_)
{
  pdcp   = ue_pdcp_;
  nas_5g = nas_5g_;
  return SRSRAN_SUCCESS;
}

int ue_rrc_nr::write_sdu(srsran::unique_byte_buffer_t sdu)
{
  if (is_connected() == false) {
    logger.error("Not connected to eNB component, cannot send SDU %i Bytes to eNB.", sdu->N_bytes);
    return SRSRAN_ERROR;
  }

  logger.debug("Incoming data from NAS, writing to eNB RRC. RNTI=0x%x, %i Bytes", rnti, sdu->N_bytes);
  gnb_rrc->nas_to_ngap(rnti, std::move(sdu));

  return SRSRAN_SUCCESS;
}

int ue_rrc_nr::connection_request(srsran::nr_establishment_cause_t cause, unique_byte_buffer_t sdu)
{
  if (is_connected()) {
    logger.error("UE already connected to gNodeB. Abort RRC connection request");
    return SRSRAN_ERROR;
  }

  logger.debug("Forwarding NAS Registration Request to gNodeB.");

  // registering at gNodeB
  uint16_t rnti_ = 0;
  int      rtn   = gnb_rrc->initial_ue(ue_itf_gnb, cause, std::move(sdu), rnti_);

  if (rtn != SRSRAN_SUCCESS) {
    logger.error("Failed to register at the gNodeB");
    return SRSRAN_ERROR;
  }
  rnti      = rnti_;
  connected = true;
  // Setup pdcp with RNTI
  pdcp->set_rnti(rnti);

  logger.info("Successful RRC connection setup assigned RNTI=0x%x.", rnti);
  return SRSRAN_SUCCESS;
}

uint16_t ue_rrc_nr::get_mnc()
{
  uint16_t mnc{};

  cell_info_t cell_info = gnb_rrc->get_cell_info();
  bytes_to_mnc(cell_info.plmn_id.mnc, &mnc, cell_info.plmn_id.nof_mnc_digits);
  return mnc;
};

uint16_t ue_rrc_nr::get_mcc()
{
  uint16_t mcc{};

  cell_info_t cell_info = gnb_rrc->get_cell_info();
  bytes_to_mcc(cell_info.plmn_id.mcc, &mcc);
  return mcc;
};

// Interface to the GNB RRC
int ue_rrc_nr::write_nas(unique_byte_buffer_t sdu)
{
  if (nas_5g == nullptr) {
    logger.error("NAS 5G interface is not initialized");
    return SRSRAN_ERROR;
  }
  nas_5g->write_pdu(std::move(sdu));
  return SRSRAN_SUCCESS;
};

int ue_rrc_nr::add_bearer(uint32_t pdu_session_id, uint32_t lcid)
{
  logger.info("Adding pdu session: pdu_session_id=%i lcid=%i,\n", pdu_session_id, lcid);
  pdcp->add_bearer(pdu_session_id, lcid);
  return SRSRAN_SUCCESS;
}

} // namespace corescope