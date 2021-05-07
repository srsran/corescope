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

#ifndef CORESCOPE_SCOPE_REST_H
#define CORESCOPE_SCOPE_REST_H

#include "corescope/hdr/corescope_interfaces.h"
#include "corescope/hdr/rest_api/app_component.h"
#include "corescope/hdr/rest_api/dtos/dto_gnb_status.h"
#include "corescope/hdr/rest_api/scope_controller.h"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "srsran/common/threads.h"
#include "srsran/srslog/srslog.h"

namespace corescope {
class scope_rest : public srsran::thread
{
public:
  scope_rest(srslog::basic_logger& logger, scope_interface_rest* scope_);
  ~scope_rest();
  int  init(rest_args_t args_);
  void stop();
  void run_thread();
  bool stop_function();

private:
  srslog::basic_logger& logger;
  scope_interface_rest* scope;
  bool                  running;
  rest_args_t           args;
};

} // namespace corescope

#endif