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

#ifndef CORESCOPE_GNB_H
#define CORESCOPE_GNB_H

#include "corescope/hdr/corescope_interfaces.h"
#include "corescope/hdr/scope_cfg.h"

#include "gnb_pdcp.h"
#include "gnb_rrc.h"

using namespace srsran;

namespace corescope {
class gnb : public gnb_interface_ue
{
public:
  gnb(srsran::task_sched_handle task_sched_, srslog::sink& sink_);
  ~gnb();

  int init(gnb_args_t args_);
  int get_metrics(gnb_metrics_t& gnb_metics);
  int get_args(gnb_args_t& gnb_args_);
  int stop();

  // User plane interface
  void write_to_gtpu(uint16_t rnti, uint32_t lcid, srsran::unique_byte_buffer_t sdu)
  {
    return pdcp.write_to_gtpu(rnti, lcid, std::move(sdu));
  }

  // Control plane interface
  int nas_to_ngap(uint16_t rnti, srsran::unique_byte_buffer_t sdu) { return rrc.nas_to_ngap(rnti, std::move(sdu)); }
  int initial_ue(ue_interface_gnb*                ue_,
                 srsran::nr_establishment_cause_t cause,
                 srsran::unique_byte_buffer_t     sdu,
                 uint16_t&                        rnti)
  {
    return rrc.initial_ue(ue_, cause, std::move(sdu), rnti);
  }

  cell_info_t get_cell_info() { return rrc.get_cell_info(); }

private:
  srslog::basic_logger& logger;

  srslog::basic_logger& gtpu_logger;
  srslog::basic_logger& ngap_logger;
  srslog::basic_logger& rrc_logger;
  srslog::basic_logger& pdcp_logger;

  srsran::task_sched_handle task_sched;

  gnb_args_t args;

  srsran::socket_manager rx_sockets;

  srsenb::gtpu        gtpu;
  srsenb::ngap        ngap;
  corescope::gnb_pdcp pdcp;
  corescope::gnb_rrc  rrc;
};
} // namespace corescope

#endif