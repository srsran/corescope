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

#include "corescope/hdr/gnb/gnb.h"

namespace corescope {

gnb::gnb(srsran::task_sched_handle task_sched_, srslog::sink& sink_) :
  logger(fetch_basic_logger("GNB", sink_, false)),
  task_sched(task_sched_),
  gtpu_logger(fetch_basic_logger("GTPU", sink_, false)),
  ngap_logger(fetch_basic_logger("NGAP", sink_, false)),
  rrc_logger(fetch_basic_logger("RRC", sink_, false)),
  pdcp_logger(fetch_basic_logger("PDCP", sink_, false)),
  gtpu(task_sched_, gtpu_logger, &rx_sockets),
  ngap(task_sched_, ngap_logger, &rx_sockets),
  rrc(task_sched_, rrc_logger),
  pdcp(pdcp_logger)
{}
gnb::~gnb(){};

int gnb::init(gnb_args_t args_)
{
  args = args_;

  pdcp_logger.set_level(srslog::str_to_basic_level(args.log.pdcp_level));
  pdcp_logger.set_hex_dump_max_size(args.log.pdcp_hex_limit);
  rrc_logger.set_level(srslog::str_to_basic_level(args.log.rrc_level));
  rrc_logger.set_hex_dump_max_size(args.log.rrc_hex_limit);
  gtpu_logger.set_level(srslog::str_to_basic_level(args.log.gtpu_level));
  gtpu_logger.set_hex_dump_max_size(args.log.gtpu_hex_limit);

  if (pdcp.init(&gtpu) != SRSRAN_SUCCESS) {
    logger.error("Couldn't initialize PDCP");
  }

  if (gtpu.init(args.gtp, &pdcp) != SRSRAN_SUCCESS) {
    logger.error("Couldn't initialize GTPU");
  }

  if (ngap.init(args.ngap, &rrc, &gtpu) != SRSRAN_SUCCESS) {
    logger.error("Couldn't initialize NGAP");
  }

  if (rrc.init(args.rrc_nr, &ngap, &pdcp) != SRSRAN_SUCCESS) {
    logger.error("Couldn't initialize GNB RRC");
  }

  return SRSRAN_SUCCESS;
}

int gnb::get_metrics(gnb_metrics_t& gnb_metics)
{
  ngap.get_metrics(gnb_metics.ngap);
  return SRSRAN_SUCCESS;
}

int gnb::get_args(gnb_args_t& gnb_args_)
{
  gnb_args_ = args;
  return SRSRAN_SUCCESS;
}

int gnb::stop()
{
  rx_sockets.stop();
  ngap.stop();
  gtpu.stop();
  return SRSRAN_SUCCESS;
}

} // namespace corescope