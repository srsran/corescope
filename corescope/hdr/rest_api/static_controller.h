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

#ifndef CORESCOPE_STATIC_CONTROLLER_H
#define CORESCOPE_STATIC_CONTROLLER_H

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

namespace corescope {
#include OATPP_CODEGEN_BEGIN(ApiController)

class StaticController : public oatpp::web::server::api::ApiController
{
public:
  StaticController(const std::shared_ptr<ObjectMapper>& objectMapper) :
    oatpp::web::server::api::ApiController(objectMapper)
  {}

public:
  static std::shared_ptr<StaticController>
      createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                   objectMapper) // Inject objectMapper component here as default parameter
      )
  {
    return std::make_shared<StaticController>(objectMapper);
  }

  ENDPOINT_INFO(root) { info->summary = "Root"; }

  ENDPOINT("GET", "/", root)
  {
    const char* html = "<html lang='en'>"
                       "  <head>"
                       "    <meta charset=utf-8/>"
                       "  </head>"
                       "  <body>"
                       "    <p>CoreScope Core Network Testing Application!</p>"
                       "    <a href='swagger/ui'>Click here for API definition with Swagger-UI</a>"
                       "  </body>"
                       "</html>";
    auto response = createResponse(Status::CODE_200, html);
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }
};

#include OATPP_CODEGEN_BEGIN(ApiController)
} // namespace corescope
#endif // CORESCOPE_STATIC_CONTROLLER_H