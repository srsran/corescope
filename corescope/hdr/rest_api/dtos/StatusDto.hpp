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
#ifndef CORESCOPE_STATUSDTO_H
#define CORESCOPE_STATUSDTO_H

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class StatusDto : public oatpp::DTO
{
  DTO_INIT(StatusDto, DTO)

  DTO_FIELD_INFO(status) { info->description = "Short status text"; }
  DTO_FIELD(String, status);

  DTO_FIELD_INFO(code) { info->description = "Status code"; }
  DTO_FIELD(Int32, code);

  DTO_FIELD_INFO(message) { info->description = "Verbose message"; }
  DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)

#endif // CORESCOPE_STATUSDTO_H
