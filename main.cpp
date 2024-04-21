#include "bench.h"

int main() {
  auto [calib, aggr] = bench([]() {
    asm volatile("; ------------ BenchBegin ------------");
    for (size_t i = 0; i < 1000; i++) {
      asm volatile("");
    }
    asm volatile("; ------------ BenchEnd --------------");
  });
  calib.print();
  aggr.print();
}
