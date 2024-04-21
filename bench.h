#ifndef BENCH_H
#define BENCH_H

#include "event-counter.h"
#include <atomic>
#include <tuple>

static event_collector collector;

template <class function_type>
std::tuple<event_aggregate, event_aggregate> bench(const function_type &function, size_t min_repeat = 10,
                      size_t min_cycles = 2500000000,
                      size_t max_repeat = 1000000) {
  event_aggregate calibration{}, aggregate{};
  size_t N = min_repeat;
  if (N == 0)
    N = 1;
  for (size_t i = 0; i < 100; i++) {
    std::atomic_thread_fence(std::memory_order_acquire);
    collector.start();
    std::atomic_thread_fence(std::memory_order_release);
    event_count allocate_count = collector.end();
    calibration << allocate_count;
  }
  for (size_t i = 0; i < N; i++) {
    std::atomic_thread_fence(std::memory_order_acquire);
    collector.start();
    function();
    std::atomic_thread_fence(std::memory_order_release);
    event_count allocate_count = collector.end();
    aggregate << allocate_count;
    if ((i + 1 == N) && (aggregate.total_cycles() < min_cycles) &&
        (N < max_repeat)) {
      N *= 10;
    }
  }
  return {calibration, aggregate};
}

#endif
