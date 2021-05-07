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

#include "corescope/hdr/gnb/gnb_rrc.h"
#include "corescope/hdr/gnb/gnb_rrc_ue.h"
#include "srsran/phy/common/phy_common.h"
namespace corescope {

gnb_rrc::gnb_rrc(srsran::task_sched_handle task_sched_, srslog::basic_logger& logger_) :
  logger(logger_), task_sched(task_sched_)
{
  gnb_rrc_task_queue = task_sched.make_task_queue();
};
gnb_rrc::~gnb_rrc(){};

int gnb_rrc::init(gnb_rrc_nr_t args_, srsenb::ngap_interface_rrc_nr* ngap_, pdcp_gnb_interface_rrc_gnb* pdcp_)
{
  ngap      = ngap_;
  pdcp      = pdcp_;
  cell_info = args_.cell_info;

  return SRSRAN_SUCCESS;
}

int gnb_rrc::nas_to_ngap(uint16_t rnti, srsran::unique_byte_buffer_t sdu)
{
  ngap->write_pdu(rnti, std::move(sdu));
  return SRSRAN_SUCCESS;
}

int gnb_rrc::initial_ue(ue_interface_gnb*                ue_,
                        srsran::nr_establishment_cause_t cause,
                        srsran::unique_byte_buffer_t     sdu,
                        uint16_t&                        rnti)
{
  int rtn = add_user(ue_, rnti);

  if (rtn != SRSRAN_SUCCESS) {
    logger.error("Adding user failed, cannot allocate RNTI");
    return SRSRAN_ERROR;
  }
  asn1::ngap_nr::rrcestablishment_cause_e ngap_cause;
  ngap_cause.value = static_cast<asn1::ngap_nr::rrcestablishment_cause_opts::options>(cause);
  logger.debug(
      "Sending Setup Request to NGAP. RNTI=0x%x, cause: %s, %i Bytes", rnti, ngap_cause.to_string(), sdu->N_bytes);
  ngap->initial_ue(rnti, 0, ngap_cause, std::move(sdu));
  pdcp->register_ue(rnti, ue_);
  return SRSRAN_SUCCESS;
}

cell_info_t gnb_rrc::get_cell_info()
{
  return cell_info;
}

int gnb_rrc::add_user(ue_rrc_interface_gnb* ue_rrc_nr, uint16_t& rnti_)
{
  uint16_t tmp_rnti = 0;
  for (uint16_t tmp_rnti = SRSRAN_CRNTI_START; tmp_rnti <= SRSRAN_CRNTI_END; tmp_rnti++) {
    if (users.count(tmp_rnti) == 0) {
      rnti_ = tmp_rnti;
      break;
    }
  }
  srsenb::unique_rnti_ptr<ue> u = srsenb::make_rnti_obj<ue>(rnti_, logger, rnti_, ue_rrc_nr, this);
  logger.info("Added user with RNTI %x", rnti_);
  users.insert(std::make_pair(rnti_, std::move(u)));
  return SRSRAN_SUCCESS;
}

int gnb_rrc::ue_set_security_cfg_key(uint16_t rnti, const asn1::fixed_bitstring<256, false, true>& key)
{
  return SRSRAN_SUCCESS;
};
int gnb_rrc::ue_set_bitrates(uint16_t rnti, const asn1::ngap_nr::ue_aggregate_maximum_bit_rate_s& rates)
{
  return SRSRAN_SUCCESS;
};
int gnb_rrc::ue_set_security_cfg_capabilities(uint16_t rnti, const asn1::ngap_nr::ue_security_cap_s& caps)
{
  return SRSRAN_SUCCESS;
};
int gnb_rrc::start_security_mode_procedure(uint16_t rnti)
{
  // TODO: Needs to be moved to corresponding RRC Reconfiguration procedure once RRC progresses
  ngap->ue_notify_rrc_reconf_complete(rnti, true);
  return SRSRAN_SUCCESS;
};

int gnb_rrc::establish_rrc_bearer(uint16_t                rnti,
                                  uint16_t                pdu_session_id,
                                  srsran::const_byte_span nas_pdu,
                                  uint32_t                lcid)
{
  auto user_it = users.find(rnti);
  if (user_it == users.end()) {
    logger.error("Couldn't find a user with RNTI 0x%x", rnti);
    return SRSRAN_ERROR;
  }
  user_it->second->establish_rrc_bearer(pdu_session_id, nas_pdu, lcid);
  return SRSRAN_SUCCESS;
}

int gnb_rrc::release_bearers(uint16_t rnti)
{
  auto user_it = users.find(rnti);
  if (user_it == users.end()) {
    logger.error("Couldn't find a user with RNTI 0x%x", rnti);
    return SRSRAN_ERROR;
  }
  user_it->second->release_bearers();
  return SRSRAN_SUCCESS;
}

void gnb_rrc::write_dl_info(uint16_t rnti, srsran::unique_byte_buffer_t sdu)
{
  logger.debug("Incoming data from NGAP, writing to UE RRC. RNTI=0x%x, %i Bytes.", rnti, sdu->N_bytes);
  auto user_it = users.find(rnti);
  if (user_it == users.end()) {
    logger.error("Couldn't find a user with RNTI 0x%x", rnti);
    return;
  }
  user_it->second->write_nas(std::move(sdu));
  return;
};

void gnb_rrc::write_ul_info(uint16_t rnti, srsran::unique_byte_buffer_t sdu)
{
  return;
};

int gnb_rrc::set_aggregate_max_bitrate(uint16_t rnti, const asn1::ngap_nr::ue_aggregate_maximum_bit_rate_s& rates)
{
  return SRSRAN_SUCCESS;
}

int gnb_rrc::allocate_lcid(uint16_t rnti)
{
  auto user_it = users.find(rnti);
  if (user_it == users.end()) {
    logger.error("Couldn't find a user with RNTI 0x%x", rnti);
    return -1;
  }
  return user_it->second->allocate_lcid();
}

} // namespace corescope
