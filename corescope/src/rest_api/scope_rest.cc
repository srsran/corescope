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

// Workaround for warnings in Controller.hpp
#pragma GCC diagnostic ignored "-Wcatch-value"

#include "corescope/hdr/rest_api/scope_rest.h"
#include "corescope/hdr/rest_api/app_component.h"
#include "corescope/hdr/rest_api/scope_controller.h"
#include "corescope/hdr/rest_api/static_controller.h"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"
#include "srsran/common/standard_streams.h"
#include "srsran/config.h"

#include <iostream>

namespace corescope {

std::atomic_bool rest_server_should_continue(true);

scope_rest::scope_rest(srslog::basic_logger& logger_, scope_interface_rest* scope_) :
  thread("API THREAD"), logger(logger_), scope(scope_)
{}

scope_rest::~scope_rest() {}

int scope_rest::init(rest_args_t args_)
{
  args    = args_;
  running = true;
  start();
  return SRSRAN_SUCCESS;
}

void scope_rest::stop()
{
  if (running == true) {
    /* Signal the stop condition */
    rest_server_should_continue.store(false);

    wait_thread_finish();
  }
}

void scope_rest::run_thread()
{
  AppComponent components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  auto docEndpoints = oatpp::swagger::Controller::Endpoints::createShared();

  auto staticController = StaticController::createShared();
  staticController->addEndpointsToRouter(router);

  /* Create MyController and add all of its endpoints to router */
  auto myController = std::make_shared<scope_controller>(logger, scope);
  myController->addEndpointsToRouter(router);

  docEndpoints->pushBackAll(myController->getEndpoints());

  auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);
  swaggerController->addEndpointsToRouter(router);
  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Print info about server port */
  logger.info("Server running on port %s", connectionProvider->getProperty("port").getData());
  srsran::console("API server running on port %s\n", connectionProvider->getProperty("port").getData());
  /* Run server */
  std::function<bool()> condition = []() { return rest_server_should_continue.load(); };
  server.run(condition);
  connectionProvider->stop();
  connectionHandler->stop();
}

} // namespace corescope