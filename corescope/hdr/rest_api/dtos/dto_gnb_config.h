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

#ifndef CORESCOPE_DTO_GNB_CONFIG_H
#define CORESCOPE_DTO_GNB_CONFIG_H

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "srsenb/hdr/stack/ngap/ngap_metrics.h"

#include OATPP_CODEGEN_BEGIN(DTO)
namespace corescope {

class dto_ngap_config : public oatpp::DTO
{
public:
  void fill_with(srsenb::ngap_args_t args)
  {
    gnb_id  = args.gnb_id;
    cell_id = args.cell_id;
    tac     = args.tac;
    std::string mcc_string;
    srsran::mcc_to_string(args.mcc, &mcc_string);
    mcc = mcc_string.c_str();
    std::string mnc_string;
    srsran::mnc_to_string(args.mnc, &mnc_string);
    mnc                = mnc_string.c_str();
    amf_addr           = args.amf_addr.c_str();
    gtp_bind_addr      = args.gtp_bind_addr.c_str();
    gtp_advertise_addr = args.gtp_advertise_addr.c_str();
    ngc_bind_addr      = args.ngc_bind_addr.c_str();
    gnb_name           = args.gnb_name.c_str();
  };

private:
  DTO_INIT(dto_ngap_config, DTO);
  DTO_FIELD(UInt32, gnb_id);
  DTO_FIELD(UInt8, cell_id);
  DTO_FIELD(UInt16, tac);
  DTO_FIELD(String, mcc);
  DTO_FIELD(String, mnc);
  DTO_FIELD(String, amf_addr);
  DTO_FIELD(String, gtp_bind_addr);
  DTO_FIELD(String, gtp_advertise_addr);
  DTO_FIELD(String, ngc_bind_addr);
  DTO_FIELD(String, gnb_name);
};

} // namespace corescope
#include OATPP_CODEGEN_END(DTO)

#endif // CORESCOPE_DTO_GNB_CONFIG_H
