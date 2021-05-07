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
#include "corescope/hdr/gnb/gnb_pdcp.h"

#include <unistd.h>

namespace corescope {

gnb_pdcp::gnb_pdcp(srslog::basic_logger& logger_) :
  logger(logger_), gtpu(nullptr), users_rwlock(PTHREAD_RWLOCK_INITIALIZER)
{
  users = std::map<uint16_t, ue_pdcp_interface_gnb*>();
  pthread_rwlock_init(&users_rwlock, nullptr);
}

gnb_pdcp::~gnb_pdcp() = default;

int gnb_pdcp::init(srsenb::gtpu_interface_pdcp* gtpu_)
{
  gtpu = gtpu_;
  return SRSRAN_SUCCESS;
}

void gnb_pdcp::stop()
{
  return;
}

/* pdcp_interface_gtpu */
void gnb_pdcp::write_sdu(uint16_t rnti, uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu, int pdcp_sn)
{
  logger.debug(
      "Incoming data from GTPU, moving to UE PDCP. RNTI=0x%x, lcid=%i, %i bytes", rnti, pdu_session_id, sdu->N_bytes);

  pthread_rwlock_rdlock(&users_rwlock);
  if (users.count(rnti) == 0) {
    logger.error("UE PDCP not registered. RNTI=0x%x", rnti);
    pthread_rwlock_unlock(&users_rwlock);
    return;
  }
  users[rnti]->write_to_gw(pdu_session_id, std::move(sdu));
  pthread_rwlock_unlock(&users_rwlock);
};

/* gnb_up_pdcp_interface_ue_up_pdcp */
void gnb_pdcp::write_to_gtpu(uint16_t rnti, uint32_t pdu_session_id, srsran::unique_byte_buffer_t sdu)
{
  logger.debug("Incoming data from UE PDCP, writing to GTPU. RNTI=0x%x, pdu_session_id=%i, %i bytes",
               rnti,
               pdu_session_id,
               sdu->N_bytes);
  gtpu->write_pdu(rnti, pdu_session_id, std::move(sdu));
}

int gnb_pdcp::register_ue(uint16_t rnti, ue_pdcp_interface_gnb* ue_pdcp_)
{
  pthread_rwlock_wrlock(&users_rwlock);
  if (users.count(rnti) != 0) {
    logger.error("UE PDCP already registered. RNTI=0x%x\n", rnti);
    pthread_rwlock_unlock(&users_rwlock);
    return SRSRAN_ERROR;
  }
  users[rnti] = ue_pdcp_;
  pthread_rwlock_unlock(&users_rwlock);
  return SRSRAN_SUCCESS;
}
} // namespace corescope