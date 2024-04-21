#ifndef EVENT_COUNTER_H
#define EVENT_COUNTER_H

#include <cctype>
#include <valarray>
#ifndef _MSC_VER
#include <dirent.h>
#endif
#include <cinttypes>

#include <cstring>

#include <chrono>
#include <vector>

#include "apple-arm-events.h"

struct event_count {
  std::chrono::duration<double> elapsed;
  std::valarray<u64> event_counts;
  event_count() : elapsed(0), event_counts((u64)0, profile_event_count) {}
  event_count(const std::chrono::duration<double> _elapsed,
              const std::valarray<u64> _event_counts)
      : elapsed(_elapsed), event_counts(_event_counts) {}
  event_count(const event_count &other)
      : elapsed(other.elapsed), event_counts(other.event_counts) {}

  double elapsed_sec() const {
    return std::chrono::duration<double>(elapsed).count();
  }
  double elapsed_ns() const {
    return std::chrono::duration<double, std::nano>(elapsed).count();
  }
  double event_of(size_t i) const { return event_counts[i]; }
  event_count &operator=(const event_count &other) {
    this->elapsed = other.elapsed;
    this->event_counts = other.event_counts;
    return *this;
  }
  event_count operator+(const event_count &other) const {
    return event_count(elapsed + other.elapsed,
                       event_counts + other.event_counts);
  }
  void operator+=(const event_count &other) {
    elapsed += other.elapsed;
    event_counts += other.event_counts;
  }
};

struct event_aggregate {
  int iterations = 0;
  event_count total{};
  event_count best{};
  event_count worst{};

  event_aggregate() = default;

  void operator<<(const event_count &other) {
    if (iterations == 0 || other.elapsed < best.elapsed) {
      best = other;
    }
    if (iterations == 0 || other.elapsed > worst.elapsed) {
      worst = other;
    }
    iterations++;
    total += other;
  }
  double elapsed_sec() const { return total.elapsed_sec() / iterations; }
  double total_elapsed_ns() const { return total.elapsed_ns(); }
  double elapsed_ns() const { return total.elapsed_ns() / iterations; }
  void print() const {
    if (iterations == 0) {
      printf("No events collected\n");
      return;
    }
    printf("Iterations: %d\n", iterations);
    printf("Average:\t%fns\t\t%fs\n", elapsed_ns(), elapsed_sec());
    printf("Best:\t\t%fns\t\t%fs\n", best.elapsed_ns(), best.elapsed_sec());
    printf("Worst:\t\t%fns\t\t%fs\n", worst.elapsed_ns(), worst.elapsed_sec());
    printf("Event counts:\n");
    for (size_t i = 0; i < total.event_counts.size(); i++) {
      printf("[%zu] %s Avg. %f Best %f Worst %f\n", i, profile_event_names[i],
             total.event_of(i) / iterations, best.event_of(i), worst.event_of(i));
    }
  }
};

struct event_collector {
  event_count count{};
  std::chrono::time_point<std::chrono::steady_clock> start_clock{};

  AppleEvents apple_events;
  performance_counters diff;
  event_collector() : diff(0) { apple_events.setup_performance_counters(); }
  bool has_events() { return apple_events.setup_performance_counters(); }

  inline void start() {
    if (has_events())
      diff = apple_events.get_counters();
    start_clock = std::chrono::steady_clock::now();
  }
  inline event_count &end() {
    if (has_events()) {
      performance_counters end = apple_events.get_counters();
      diff = end - diff;
    }
    const auto end_clock = std::chrono::steady_clock::now();
    count.event_counts = diff.counters;
    count.elapsed = end_clock - start_clock;
    return count;
  }
};

#endif
