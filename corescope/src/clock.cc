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

#include "corescope/hdr/clock.h"

namespace corescope {

/*
 * tick_src regularly calls run_tti() for all registered subscribers
 */
clock::clock(std::chrono::microseconds _interval) : interval(_interval), thread("Tick")
{
  thread::start(DEFAULT_PRIORITY);
  running = true;
};
clock::~clock()
{
  subscribers.clear();
};

/*
 * tick_provider
 */

void clock::register_subscriber(std::shared_ptr<tick_subscriber> _subscriber)
{
  std::lock_guard<std::mutex> lock(subscribers_mutex);
  subscribers.push_back(_subscriber);
}

void clock::deregister_subscriber(std::shared_ptr<tick_subscriber> _subscriber)
{
  std::lock_guard<std::mutex> lock(subscribers_mutex);

  auto it = std::find(subscribers.begin(), subscribers.end(), _subscriber);
  if (it != subscribers.end()) {
    subscribers.erase(it);
  }
}

/* thread */
void clock::run_thread()
{
  uint32_t tti = 0;
  while (running) {
    std::this_thread::sleep_for(interval);
    std::lock_guard<std::mutex> lock(subscribers_mutex);
    for (auto subscriber : subscribers) {
      subscriber->run_tti(tti);
    }
    tti++;
  }
}

void clock::stop()
{
  running = false;
  wait_thread_finish();
  subscribers.clear();
}

} // namespace corescope
