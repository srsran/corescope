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

#ifndef CORESCOPE_DTO_UE_STATUS_H
#define CORESCOPE_DTO_UE_STATUS_H

#include "corescope/hdr/scope_cfg.h"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "srsue/hdr/stack/upper/nas_5g_metrics.h"

#include OATPP_CODEGEN_BEGIN(DTO)
namespace corescope {

class dto_ue_nas_5g_status : public oatpp::DTO
{
public:
  void fill_with(srsue::nas_5g_metrics_t metrics)
  {
    nas_state             = "TEST";
    nof_active_5g_bearers = metrics.nof_active_pdu_sessions;
  }

private:
  DTO_INIT(dto_ue_nas_5g_status, DTO);
  DTO_FIELD(String, nas_state);
  DTO_FIELD(UInt32, nof_active_5g_bearers);
};

class dto_ue_status : public oatpp::DTO
{
public:
  void fill_with(ue_metrics_t metrics)
  {
    nas_5g = dto_ue_nas_5g_status::createShared();
    nas_5g->fill_with(metrics.nas_5g);
    active    = metrics.active;
    initiated = metrics.initiated;
  }

private:
  DTO_INIT(dto_ue_status, DTO);
  DTO_FIELD(Boolean, active);
  DTO_FIELD(Boolean, initiated);
  DTO_FIELD(Object<dto_ue_nas_5g_status>, nas_5g);
};

class dto_ue_status_update : public oatpp::DTO
{
public:
private:
  DTO_INIT(dto_ue_status_update, DTO);
  DTO_FIELD(Boolean, switch_on) = false;
};
#include OATPP_CODEGEN_END(DTO)
} // namespace corescope
#endif // CORESCOPE_DTO_GNB_STATUS_H
