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

#ifndef CORESCOPE_ERROR_HANDLER_H
#define CORESCOPE_ERROR_HANDLER_H

#include "dtos/StatusDto.hpp"

#include "oatpp/web/protocol/http/outgoing/ResponseFactory.hpp"
#include "oatpp/web/server/handler/ErrorHandler.hpp"

namespace corescope {

class error_handler : public oatpp::web::server::handler::ErrorHandler
{
private:
  typedef oatpp::web::protocol::http::outgoing::Response        OutgoingResponse;
  typedef oatpp::web::protocol::http::Status                    Status;
  typedef oatpp::web::protocol::http::outgoing::ResponseFactory ResponseFactory;

private:
  std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_objectMapper;

public:
  error_handler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper);
  virtual ~error_handler();
  std::shared_ptr<OutgoingResponse>
  handleError(const Status& status, const oatpp::String& message, const Headers& headers) override;
};
} // namespace corescope

#endif
