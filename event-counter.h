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
  std::valarray<u64> event_counts;
  event_count() : event_counts((u64)0, profile_event_count) {}
  event_count(const std::valarray<u64> &_event_counts)
      : event_counts(_event_counts) {}
  event_count(const event_count &other) : event_counts(other.event_counts) {}
  event_count &operator=(const event_count &other) {
    this->event_counts = other.event_counts;
    return *this;
  }
  u128 operator[](size_t i) { return event_counts[i]; }
  double event_of(size_t i) const { return event_counts[i]; }
  event_count operator+(const event_count &other) const {
    return event_count(event_counts + other.event_counts);
  }
  void operator+=(const event_count &other) {
    event_counts += other.event_counts;
  }
};

struct event_aggregate {
  int iterations = 0;
  event_count total{};
  event_count best{};
  event_count worst{};

  event_aggregate() = default;

  u64 total_cycles() { return total[EventCycles]; }

  void operator<<(const event_count &other) {
    if (iterations == 0 ||
        other.event_counts[EventCycles] < best.event_counts[EventCycles])
      best = other;
    if (iterations == 0 ||
        other.event_counts[EventCycles] > worst.event_counts[EventCycles])
      worst = other;
    iterations++;
    total += other;
  }
  void print() const {
    if (iterations == 0) {
      printf("No events collected\n");
      return;
    }
    printf("Iterations: %d\n", iterations);
    printf("Event counts:\n");
    for (size_t i = 0; i < total.event_counts.size(); i++) {
      printf("[%zu] %s Avg. %f Best %f Worst %f\n", i, profile_event_names[i],
             total.event_of(i) / iterations, best.event_of(i),
             worst.event_of(i));
    }
  }
};

struct event_collector {
  event_count count{};

  AppleEvents apple_events;
  performance_counters diff;
  event_collector() : diff(0) { apple_events.setup_performance_counters(); }
  bool has_events() { return apple_events.setup_performance_counters(); }

  inline void start() {
    if (has_events())
      diff = apple_events.get_counters();
  }
  inline event_count &end() {
    if (has_events()) {
      performance_counters end = apple_events.get_counters();
      diff = end - diff;
    }
    count = event_count{diff.counters};
    return count;
  }
};

#endif
