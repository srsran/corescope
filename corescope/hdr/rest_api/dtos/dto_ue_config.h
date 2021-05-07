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

#ifndef CORESCOPE_DTO_UE_CONFIG_H
#define CORESCOPE_DTO_UE_CONFIG_H

#include "corescope/hdr/scope_cfg.h"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "srsran/config.h"
#include OATPP_CODEGEN_BEGIN(DTO)

namespace corescope {
class dto_ue_usim_args : public oatpp::DTO
{
public:
  int to_corescope_args(srsue::usim_args_t& usim_args)
  {
    usim_args.mode     = mode->std_str();
    usim_args.algo     = algo->std_str();
    usim_args.using_op = using_op;
    usim_args.op       = op->std_str();
    usim_args.opc      = opc->std_str();
    usim_args.imsi     = imsi->std_str();
    usim_args.imei     = imei->std_str();
    usim_args.k        = k->std_str();
    usim_args.pin      = pin->std_str();
    usim_args.reader   = reader->std_str();
    return SRSRAN_SUCCESS;
  }

  int fill_with(const srsue::usim_args_t& usim_args)
  {
    mode     = usim_args.mode.c_str();
    algo     = usim_args.algo.c_str();
    using_op = usim_args.using_op;
    op       = usim_args.op.c_str();
    opc      = usim_args.opc.c_str();
    imsi     = usim_args.imsi.c_str();
    imei     = usim_args.imei.c_str();
    k        = usim_args.k.c_str();
    pin      = usim_args.pin.c_str();
    reader   = usim_args.reader.c_str();
    return SRSRAN_SUCCESS;
  }

private:
  DTO_INIT(dto_ue_usim_args, DTO);
  DTO_FIELD(String, mode)      = "soft";
  DTO_FIELD(String, algo)      = "mil";
  DTO_FIELD(Boolean, using_op) = false;
  DTO_FIELD(String, op)        = "";
  DTO_FIELD(String, opc)       = "63bfa50ee6523365ff14c1f45f88737d";
  DTO_FIELD(String, imsi)      = "001010123456780";
  DTO_FIELD(String, imei)      = "353490069873310";
  DTO_FIELD(String, k)         = "00112233445566778899aabbccddeeff";
  DTO_FIELD(String, pin)       = "";
  DTO_FIELD(String, reader)    = "";
};

class dto_ue_nas_pdu_session_args : public oatpp::DTO
{
public:
  int to_corescope_args(srsue::pdu_session_cfg_t& pdu_session_args)
  {
    pdu_session_args.apn_name = apn_name->std_str();
    pdu_session_args.apn_pass = apn_pass->std_str();
    pdu_session_args.apn_type = srsue::apn_types::ipv4;
    pdu_session_args.apn_user = apn_user->std_str();
    return SRSRAN_SUCCESS;
  }
  DTO_INIT(dto_ue_nas_pdu_session_args, DTO);
  DTO_FIELD(String, apn_name)     = "internet";
  DTO_FIELD(String, apn_protocol) = "ipv4";
  DTO_FIELD(String, apn_user)     = "srsuser";
  DTO_FIELD(String, apn_pass)     = "srspass";
};

class dto_ue_nas_5g_args : public oatpp::DTO
{
public:
  int to_corescope_args(srsue::nas_5g_args_t& nas_args)
  {
    // TODO Update for new PDU session strucutre
    nas_args.force_imsi_attach = force_imsi_attach;
    for (auto session : *pdu_sessions) {
      srsue::pdu_session_cfg_t pdu_session_cfg;
      session.get()->to_corescope_args(pdu_session_cfg);
      nas_args.pdu_session_cfgs.push_back(pdu_session_cfg);
    }
    nas_args.eia  = eia->std_str();
    nas_args.eea  = eea->std_str();
    nas_args.ia5g = ia5g->std_str();
    nas_args.ea5g = ea5g->std_str();
    return SRSRAN_SUCCESS;
  }

  int fill_with(const srsue::nas_5g_args_t& nas_args)
  {
    // TODO Update for new PDU session strucutre
    force_imsi_attach = nas_args.force_imsi_attach;
    eia               = nas_args.eia.c_str();
    eea               = nas_args.eea.c_str();
    ia5g              = nas_args.ia5g.c_str();
    ea5g              = nas_args.ea5g.c_str();
    return SRSRAN_SUCCESS;
  }

  DTO_INIT(dto_ue_nas_5g_args, DTO);

  DTO_FIELD(Boolean, force_imsi_attach)                               = false;
  DTO_FIELD(String, eia)                                              = "1, 2";
  DTO_FIELD(String, eea)                                              = "0, 1, 2";
  DTO_FIELD(List<Object<dto_ue_nas_pdu_session_args> >, pdu_sessions) = {};
  DTO_FIELD(String, ia5g)                                             = "1, 2";
  DTO_FIELD(String, ea5g)                                             = "0, 1, 2";
};

class dto_ue_gw_args : public oatpp::DTO
{
public:
  int to_corescope_args(srsue::gw_args_t& gw_args)
  {
    gw_args.netns           = netns->std_str();
    gw_args.tun_dev_name    = tun_dev_name->std_str();
    gw_args.tun_dev_netmask = tun_dev_netmask->std_str();
    return SRSRAN_SUCCESS;
  }

  int fill_with(const srsue::gw_args_t& gw_args)
  {
    netns           = gw_args.netns.c_str();
    tun_dev_name    = gw_args.tun_dev_name.c_str();
    tun_dev_netmask = gw_args.tun_dev_netmask.c_str();
    return SRSRAN_SUCCESS;
  }

  DTO_INIT(dto_ue_gw_args, DTO);

  DTO_FIELD(String, netns)           = "";
  DTO_FIELD(String, tun_dev_name)    = "tun_ue_$id";
  DTO_FIELD(String, tun_dev_netmask) = "255.255.255.0";
};

class dto_ue_pcap_args : public oatpp::DTO
{
public:
  int to_corescope_args(pcap_args_t& pcap_args)
  {
    pcap_args.enable   = enable;
    pcap_args.filename = filename->std_str();
    return SRSRAN_SUCCESS;
  }

  int fill_with(const pcap_args_t& pcap_args)
  {
    enable   = pcap_args.enable;
    filename = pcap_args.filename.c_str();
    return SRSRAN_SUCCESS;
  }

  DTO_INIT(dto_ue_pcap_args, DTO);

  DTO_FIELD(Boolean, enable)  = false;
  DTO_FIELD(String, filename) = "/tmp/ue_$id.pcap";
};

class dto_ue_log_args : public oatpp::DTO
{
public:
  int to_corescope_args(ue_log_args_t& log_args)
  {
    log_args.filename_base  = filename->std_str();
    log_args.pdcp_level     = pdcp_level->std_str();
    log_args.pdcp_hex_limit = pdcp_hex_limit;
    log_args.rrc_level      = rrc_level->std_str();
    log_args.rrc_hex_limit  = rrc_hex_limit;
    log_args.nas_level      = nas_level->std_str();
    log_args.nas_hex_limit  = nas_hex_limit;
    log_args.usim_level     = usim_level->std_str();
    log_args.usim_hex_limit = usim_hex_limit;
    return SRSRAN_SUCCESS;
  }

  int fill_with(const ue_log_args_t& log_args)
  {
    filename       = log_args.filename_base.c_str();
    pdcp_level     = log_args.pdcp_level.c_str();
    pdcp_hex_limit = log_args.pdcp_hex_limit;
    rrc_level      = log_args.rrc_level.c_str();
    rrc_hex_limit  = log_args.rrc_hex_limit;
    nas_level      = log_args.nas_level.c_str();
    nas_hex_limit  = log_args.nas_hex_limit;
    usim_level     = log_args.usim_level.c_str();
    usim_hex_limit = log_args.usim_hex_limit;
    return SRSRAN_SUCCESS;
  }

private:
  DTO_INIT(dto_ue_log_args, DTO);
  // if log name the same as main log file that everything gets written to the main log file
  // TODO: document this in the api
  DTO_FIELD(String, filename)      = "/tmp/ue_$id.log";
  DTO_FIELD(String, pdcp_level)    = "info";
  DTO_FIELD(Int64, pdcp_hex_limit) = 128;
  DTO_FIELD(String, rrc_level)     = "info";
  DTO_FIELD(Int64, rrc_hex_limit)  = 128;
  DTO_FIELD(String, nas_level)     = "info";
  DTO_FIELD(Int64, nas_hex_limit)  = 128;
  DTO_FIELD(String, usim_level)    = "info";
  DTO_FIELD(Int64, usim_hex_limit) = 128;
};

class dto_ue_args : public oatpp::DTO
{
public:
  int to_corescope_args(ue_args_t& ue_args)
  {
    if (log) {
      log->to_corescope_args(ue_args.log);
    }
    if (usim) {
      usim->to_corescope_args(ue_args.usim);
    }
    if (nas_5g) {
      nas_5g->to_corescope_args(ue_args.nas_5g);
    }
    if (gw) {
      gw->to_corescope_args(ue_args.gw);
    }
    if (pcap) {
      pcap->to_corescope_args(ue_args.pcap);
    }
    return SRSRAN_SUCCESS;
  }

  int fill_with(ue_args_t ue_args)
  {
    log = dto_ue_log_args::createShared();
    log->fill_with(ue_args.log);
    usim = dto_ue_usim_args::createShared();
    usim->fill_with(ue_args.usim);
    nas_5g = dto_ue_nas_5g_args::createShared();
    nas_5g->fill_with(ue_args.nas_5g);
    gw = dto_ue_gw_args::createShared();
    gw->fill_with(ue_args.gw);
    pcap = dto_ue_pcap_args::createShared();
    pcap->fill_with(ue_args.pcap);

    return SRSRAN_SUCCESS;
  }

  DTO_INIT(dto_ue_args, DTO);
  DTO_FIELD(Object<dto_ue_log_args>, log);
  DTO_FIELD(Object<dto_ue_usim_args>, usim);
  DTO_FIELD(Object<dto_ue_nas_5g_args>, nas_5g);
  DTO_FIELD(Object<dto_ue_gw_args>, gw);
  DTO_FIELD(Object<dto_ue_pcap_args>, pcap);
};

class dto_ue_id : public oatpp::DTO
{
public:
  DTO_INIT(dto_ue_id, DTO);
  DTO_FIELD(UInt32, ue_id);
};

} // namespace corescope
#include OATPP_CODEGEN_END(DTO)
#endif