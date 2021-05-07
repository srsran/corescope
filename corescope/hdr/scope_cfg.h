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

#ifndef CORESCOPE_SCOPE_CFG_H
#define CORESCOPE_SCOPE_CFG_H

#include "srsenb/hdr/stack/ngap/ngap.h"
#include "srsenb/hdr/stack/s1ap/s1ap.h"
#include "srsenb/hdr/stack/upper/gtpu.h"
#include "srsue/hdr/stack/upper/gw.h"
#include "srsue/hdr/stack/upper/nas.h"
#include "srsue/hdr/stack/upper/nas_5g.h"
#include "srsue/hdr/stack/upper/usim.h"

#include <string.h>

namespace corescope {

/* Configuration */
class pcap_args_t
{
public:
  std::string filename = "";
  bool        enable;
};

class ue_log_args_t
{
public:
  /* each UE adds suffix to filename_base, e.g., UE-1.log */
  std::string filename_base = "";
  std::string pdcp_level    = "";
  int         pdcp_hex_limit;
  std::string rrc_level = "";
  int         rrc_hex_limit;
  std::string nas_level = "";
  int         nas_hex_limit;
  std::string usim_level = "";
  int         usim_hex_limit;
};

class ue_args_t
{
public:
  ue_log_args_t        log;
  srsue::nas_5g_args_t nas_5g;
  srsue::usim_args_t   usim;
  srsue::gw_args_t     gw;
  pcap_args_t          pcap;
};

class ue_metrics_t
{
public:
  srsue::nas_5g_metrics_t nas_5g;
  srsue::nas_metrics_t    nas;
  bool                    initiated;
  bool                    active;
};

/* to emulate the cell-search */
typedef struct cell_info_s {
  bool              enabled = false;
  srsran::plmn_id_t plmn_id;
  uint16_t          tac;
} cell_info_t;

typedef struct {
  std::string pdcp_level;
  int         pdcp_hex_limit;
  std::string rrc_level;
  int         rrc_hex_limit;
  std::string gtpu_level;
  int         gtpu_hex_limit;
  std::string ngap_level;
  int         ngap_hex_limit;
} gnb_log_args_t;

typedef struct {
  std::string all_level;
  int         all_hex_limit;
  std::string main_level;
  int         main_hex_limit;
  int         file_max_size;
  std::string filename;
} log_args_t;

typedef struct {
  cell_info_t cell_info;
} gnb_rrc_nr_t;

typedef struct {
  srsenb::ngap_args_t ngap;
  gnb_rrc_nr_t        rrc_nr;
  srsenb::gtpu_args_t gtp;
  gnb_log_args_t      log;
} gnb_args_t;

typedef struct {
  srsenb::ngap_metrics_t ngap;
} gnb_metrics_t;

typedef struct {
  std::string log_level;
  int         log_hex_limit;
  std::string bind_addr;
  uint16_t    bind_port;
} rest_args_t;

typedef struct all_args_s {
  log_args_t  log;
  gnb_args_t  gnb;
  rest_args_t rest;
} all_args_t;

} // namespace corescope

#endif // CORESCOPE_SCOPE_CFG_H