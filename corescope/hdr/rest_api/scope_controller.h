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

#ifndef CORESCOPE_SCOPE_CONTROLLER_H
#define CORESCOPE_SCOPE_CONTROLLER_H

#include "app_component.h"
#include "corescope/hdr/corescope_interfaces.h"
#include "dtos/dto_gnb_config.h"
#include "dtos/dto_gnb_status.h"
#include "dtos/dto_ue_config.h"
#include "dtos/dto_ue_status.h"
#include "srsenb/hdr/stack/ngap/ngap_metrics.h"
#include "srsran/common/standard_streams.h"
#include "srsran/srslog/srslog.h"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

namespace corescope {

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class scope_controller : public oatpp::web::server::api::ApiController
{
public:
  scope_controller(srslog::basic_logger& logger_,
                   scope_interface_rest* scope_,
                   OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) :
    logger(logger_), scope(scope_), oatpp::web::server::api::ApiController(objectMapper)
  {}

public:
  ENDPOINT_INFO(getGnbByIdStatus)
  {
    info->summary     = "Get gNodeB status by gnbID";
    info->description = "Only gnbID 0 is allowed yet";
    info->addResponse<Object<dto_gnb_status> >(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->pathParams["gnbID"].description = "gNodeB identifier";
  }

  ENDPOINT("GET", "corescope/api/gnb/{gnbId}/status", getGnbByIdStatus, PATH(UInt32, gnbId))
  {
    gnb_metrics_t gnb_metric;
    int           rtn = scope->get_gnb_status(gnbId, gnb_metric);
    auto          dto = dto_ngap_status::createShared();
    dto->fill_with(gnb_metric.ngap);
    OATPP_ASSERT_HTTP(rtn == SRSRAN_SUCCESS, Status::CODE_404, "gNodeB not found");
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT_INFO(getGnbByIdConfig)
  {
    info->summary     = "Get gNodeB current configuration by gnbID";
    info->description = "Only gnbID 0 is allowed yet";
    info->addResponse<Object<dto_ngap_config> >(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->pathParams["gnbID"].description = "gnodeB identifier";
  }

  ENDPOINT("GET", "corescope/api/gnb/{gnbId}/config", getGnbByIdConfig, PATH(UInt32, gnbId))
  {
    gnb_args_t gnb_args;
    int        rtn = scope->get_gnb_config(gnbId, gnb_args);
    auto       dto = dto_ngap_config::createShared();
    dto->fill_with(gnb_args.ngap);
    OATPP_ASSERT_HTTP(rtn == SRSRAN_SUCCESS, Status::CODE_404, "gNodeB not found");
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT_INFO(createUe)
  {
    info->summary     = "Create a UE at a gNodeB.";
    info->description = "Only gnbID 0 is allowed yet. Returns UE ID of the created UE.";
    info->addResponse<Object<dto_ue_id> >(Status::CODE_201, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->pathParams["gnbID"].description       = "gnodeB identifier";
    info->pathParams["ue_args_dto"].description = "UE parameters";
  }

  ENDPOINT("POST",
           "/corescope/api/gnb/{gnbId}/ue",
           createUe,
           PATH(UInt32, gnbId),
           BODY_DTO(Object<dto_ue_args>, ue_args_dto))
  {
    if (gnbId != 0) {
      return createResponse(Status::CODE_404, "gNodeB not found");
    }
    ue_args_t ue_args;
    uint32_t  ue_id = 0; // UE ID which is created by the CoreScope
    int       rtn;
    ue_args_dto->to_corescope_args(ue_args);
    rtn = scope->add_ue(ue_args, ue_id);
    OATPP_ASSERT_HTTP(rtn == SRSRAN_SUCCESS, Status::CODE_404, "gNodeB not found");
    logger.info("Adding: at gnb id %u imsi=%s and ue id %d", gnbId, ue_args_dto->usim->imsi->std_str(), ue_id);
    auto dto   = dto_ue_id::createShared();
    dto->ue_id = ue_id;
    // Fetch the current config
    return createDtoResponse(Status::CODE_201, dto);
  }

  ENDPOINT_INFO(getUe)
  {
    info->summary     = "Gets the current UE configuration";
    info->description = "Only gnbID 0 is allowed yet.";
    info->addResponse<Object<dto_ue_args> >(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->pathParams["gnbID"].description = "gnodeB identifier";
    info->pathParams["ueId"].description  = "UE identifier";
  }

  ENDPOINT("GET", "/corescope/api/gnb/{gnbId}/ue/{ueId}", getUe, PATH(UInt32, gnbId), PATH(UInt32, ueId))
  {
    if (gnbId != 0) {
      return createResponse(Status::CODE_404, "gNodeB not found");
    }
    int       rtn;
    ue_args_t ue_args_out = {};
    auto      dto         = dto_ue_args::createShared();
    rtn                   = scope->get_ue_config(ueId, ue_args_out);
    OATPP_ASSERT_HTTP(rtn == SRSRAN_SUCCESS, Status::CODE_404, "UE not found");
    dto->fill_with(ue_args_out);
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT_INFO(getUeStatus)
  {
    info->summary     = "Gets the current UE status";
    info->description = "Only gnbID 0 is allowed yet.";
    info->addResponse<Object<dto_ue_status> >(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->pathParams["gnbID"].description = "gnodeB identifier";
    info->pathParams["ueId"].description  = "UE identifier";
  }

  ENDPOINT("GET", "/corescope/api/gnb/{gnbId}/ue/{ueId}/status", getUeStatus, PATH(UInt32, gnbId), PATH(UInt32, ueId))
  {
    if (gnbId != 0) {
      return createResponse(Status::CODE_404, "gNodeB not found");
    }
    int          rtn;
    ue_metrics_t ue_metrics = {};
    auto         dto        = dto_ue_status::createShared();
    rtn                     = scope->get_ue_status(ueId, ue_metrics);
    OATPP_ASSERT_HTTP(rtn == SRSRAN_SUCCESS, Status::CODE_404, "UE not found");
    dto->fill_with(ue_metrics);
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT_INFO(updateUeState)
  {
    info->summary     = "Update UE Status";
    info->description = "Updates the connection status of the UE. Switched the UE on to trigger the registration "
                        "procedure or switches the UE off to deregister the UE. Only gnbID 0 is allowed yet.";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->pathParams["gnbID"].description = "gNodeB identifier";
    info->pathParams["ueId"].description  = "UE identifier";
  }

  ENDPOINT("PUT",
           "/corescope/api/gnb/{gnbId}/ue/{ueId}/status",
           updateUeState,
           PATH(UInt32, gnbId),
           PATH(UInt32, ueId),
           BODY_DTO(Object<dto_ue_status_update>, ue_status_update_dto))
  {
    if (gnbId != 0) {
      return createResponse(Status::CODE_404, "gNodeB not found");
    }
    int rtn;
    if (ue_status_update_dto->switch_on == true) {
      rtn = scope->ue_switch_on(gnbId, ueId);
    } else {
      rtn = scope->ue_switch_off(gnbId, ueId);
    }
    OATPP_ASSERT_HTTP(rtn == SRSRAN_SUCCESS, Status::CODE_404, "UE not found");
    return createResponse(Status::CODE_200, "Succesful");
  }

private:
  srslog::basic_logger& logger;
  scope_interface_rest* scope;
};
#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
} // namespace corescope

#endif //!__SCOPE_CONTROLLER__H__
