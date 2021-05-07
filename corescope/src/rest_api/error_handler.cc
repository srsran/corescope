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

#include "corescope/hdr/rest_api/error_handler.h"
namespace corescope {

error_handler::error_handler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper) :
  m_objectMapper(objectMapper)
{}

error_handler::~error_handler() {}

std::shared_ptr<error_handler::OutgoingResponse>
error_handler::handleError(const Status& status, const oatpp::String& message, const Headers& headers)
{
  auto error     = StatusDto::createShared();
  error->status  = "ERROR";
  error->code    = status.code;
  error->message = message;

  auto response = ResponseFactory::createResponse(status, error, m_objectMapper);

  for (const auto& pair : headers.getAll()) {
    response->putHeader(pair.first.toString(), pair.second.toString());
  }

  return response;
}
} // namespace corescope