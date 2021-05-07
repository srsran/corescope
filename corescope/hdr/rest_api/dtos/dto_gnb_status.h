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

#ifndef CORESCOPE_DTO_GNB_STATUS_H
#define CORESCOPE_DTO_GNB_STATUS_H

#include "corescope/hdr/scope_cfg.h"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)
namespace corescope {

ENUM(STATUS_ENUM,
     v_int32,
     VALUE(ngap_ATTACHING, 0, "Attaching"), //<- you may use qualifiers here
     VALUE(ngap_CONNECTED, 1, "Connected"),
     VALUE(ngap_ERROR, 2, "Error"))
/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class dto_ngap_status : public oatpp::DTO
{
public:
  void fill_with(srsenb::ngap_metrics_t metrics)
  {
    switch (metrics.status) {
      case srsenb::ngap_status_t::ngap_attaching:
        ngap_status = STATUS_ENUM::ngap_ATTACHING;
        break;
      case srsenb::ngap_status_t::ngap_connected:
        ngap_status = STATUS_ENUM::ngap_CONNECTED;
        break;
      case srsenb::ngap_status_t::ngap_error:
        ngap_status = STATUS_ENUM::ngap_ERROR;
        break;
      default:
        ngap_status = STATUS_ENUM::ngap_ATTACHING;
        break;
    }
  };

private:
  DTO_INIT(dto_ngap_status, DTO);
  DTO_FIELD(Enum<STATUS_ENUM>, ngap_status);
};

class dto_gnb_status : public oatpp::DTO
{
public:
  void fill_with(gnb_metrics_t metrics)
  {
    ngap = dto_ngap_status::createShared();
    ngap->fill_with(metrics.ngap);
  };

private:
  DTO_INIT(dto_gnb_status, DTO);
  DTO_FIELD(Object<dto_ngap_status>, ngap);
};

#include OATPP_CODEGEN_END(DTO)
} // namespace corescope
#endif // CORESCOPE_DTO_GNB_STATUS_H
