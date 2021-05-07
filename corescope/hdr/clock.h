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

#ifndef CORESCOPE_CLOCK_H
#define CORESCOPE_CLOCK_H

#include "corescope/hdr/corescope_interfaces.h"
#include "srsran/common/threads.h"

#include <algorithm>
#include <atomic>
#include <chrono> // std::chrono::system_clock
#include <ctime>  // std::time_t, std::tm, std::localtime, std::mktime
#include <mutex>
#include <thread> // std::this_thread::sleep_until
#include <vector>

namespace corescope {

class clock : public tick_provider, public srsran::thread
{
public:
  clock(std::chrono::microseconds _interval);
  ~clock();
  clock(const clock&) = delete;
  clock& operator=(const clock&) = delete;

  /* tick_provider */
  void register_subscriber(std::shared_ptr<tick_subscriber> _subscriber);
  void deregister_subscriber(std::shared_ptr<tick_subscriber> _subscriber);

  /* thread */
  void start();
  void run_thread();
  void stop();

private:
  std::atomic<bool> running;

  const std::chrono::microseconds interval;

  std::mutex                                     subscribers_mutex;
  std::vector<std::shared_ptr<tick_subscriber> > subscribers;
};
} // namespace corescope
#endif // CORESCOPE_CLOCK_H