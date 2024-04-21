#include "bench.h"

[[gnu::always_inline, gnu::unused]]
static void f1() {
  asm volatile("bl call_target\n"
               "b next\n"
               "call_target:\n"
               "ret\n"
               "next:\n");
}

[[gnu::always_inline, gnu::unused]]
static void f2() {
  asm volatile("adr x30, #8\n"
               "b call_target\n"
               "b next\n"
               "call_target:\n"
               "ret\n"
               "next:\n");
}

[[gnu::always_inline, gnu::unused]]
static void f3() {
  asm volatile("bl call_target\n"
               "b next\n"
               "call_target:\n"
               "br x30\n"
               "next:\n");
}

[[gnu::always_inline, gnu::unused]]
static void f4() {
  asm volatile("adr x30, #8\n"
               "b call_target\n"
               "b next\n"
               "call_target:\n"
               "br x30\n"
               "next:\n");
}

int main() {
  auto [calib, aggr] = bench([]() {
    asm volatile("sub sp, sp, #0x10\n"
                 "str x30, [sp, #8]\n");
    asm volatile("; ------------ BenchBegin ------------");
    for (size_t i = 0; i < 1000; i++) {
      f4();
    }
    asm volatile("; ------------ BenchEnd --------------");
    asm volatile("ldr x30, [sp, #8]\n"
                 "add sp, sp, #0x10\n");
  });
  calib.print();
  aggr.print();
}
