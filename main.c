#include "apple-arm-counter.h"

#include <assert.h>
#include <libkern/OSCacheControl.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/mman.h>
#include <sys/qos.h>

#define L(name_) __asm__ volatile("stub_" #name_ ":\n\t.global stub_" #name_:)
#define _(x_) __asm__ volatile(x_ "\n\t")

__attribute__((noinline, naked)) void stub() {
  L(prologueS);
  _("sub sp, sp, #128");
  _("stp x29, x30, [sp, #0]");
  _("stp x28, x27, [sp, #16]");
  _("stp x26, x25, [sp, #32]");
  _("stp x24, x23, [sp, #48]");
  _("stp x22, x21, [sp, #64]");
  _("stp x20, x19, [sp, #80]");
  _("stp x18, x17, [sp, #96]");
  _("stp x16, x15, [sp, #112]");
  L(prologueE);

  L(epilogueS);
  _("ldp x16, x15, [sp, #112]");
  _("ldp x18, x17, [sp, #96]");
  _("ldp x20, x19, [sp, #80]");
  _("ldp x22, x21, [sp, #64]");
  _("ldp x24, x23, [sp, #48]");
  _("ldp x26, x25, [sp, #32]");
  _("ldp x28, x27, [sp, #16]");
  _("ldp x29, x30, [sp, #0]");
  _("add sp, sp, #128");
  _("ret");
  L(epilogueE);

  L(vprologueS);
  _("sub sp, sp, #80");
  _("stp d15, d14, [sp, #0]");
  _("stp d13, d12, [sp, #16]");
  _("stp d11, d10, [sp, #32]");
  _("stp d9, d8, [sp, #48]");
  _("stp x29, x30, [sp, #64]");
  L(vprologueE);

  L(vepilogueS);
  _("ldp d15, d14, [sp, #0]");
  _("ldp d13, d12, [sp, #16]");
  _("ldp d11, d10, [sp, #32]");
  _("ldp d9, d8, [sp, #48]");
  _("ldp x29, x30, [sp, #64]");
  _("add sp, sp, #80");
  _("ret");
  L(vepilogueE);

  L(prepS);
  _("mov x5, #0");
  _("mov x6, #0");
  _("mov x7, #0");
  _("mov x8, #0");
  _("mov x9, #0");
  _("mov x10, #0");
  _("mov x11, #0");
  _("mov x12, #0");
  _("mov x13, #0");
  _("mov x14, #0");
  _("mov x15, #0");
  _("mov x16, #0");
  _("mov x17, #0");
  _("mov x18, #0");
  _("mov x19, #0");
  _("mov x20, #0");
  _("mov x21, #0");
  _("mov x22, #0");
  _("mov x23, #0");
  _("mov x24, #0");
  _("mov x25, #0");
  _("mov x26, #0");
  _("mov x27, #0");
  _("mov x28, #0");
  _("mov x29, #0");
  _("mov x30, #0");
  L(prepE);

  L(vprepS);
  _("mov.16b v0, v1");
  _("mov.16b v1, v1");
  _("mov.16b v2, v1");
  _("mov.16b v3, v1");
  _("mov.16b v4, v1");
  _("mov.16b v5, v1");
  _("mov.16b v6, v1");
  _("mov.16b v7, v1");
  _("mov.16b v8, v1");
  _("mov.16b v9, v1");
  _("mov.16b v10, v1");
  _("mov.16b v11, v1");
  _("mov.16b v12, v1");
  _("mov.16b v13, v1");
  _("mov.16b v14, v1");
  _("mov.16b v15, v1");
  _("mov.16b v16, v1");
  _("mov.16b v17, v1");
  _("mov.16b v18, v1");
  _("mov.16b v19, v1");
  _("mov.16b v20, v1");
  _("mov.16b v21, v1");
  _("mov.16b v22, v1");
  _("mov.16b v23, v1");
  _("mov.16b v24, v1");
  _("mov.16b v25, v1");
  _("mov.16b v26, v1");
  _("mov.16b v27, v1");
  _("mov.16b v28, v1");
  _("mov.16b v29, v1");
  _("mov.16b v30, v1");
  _("mov.16b v31, v1");
  L(vprepE);

  L(nopS);
  _("nop");
  L(nopE);

  L(movS);
  _("mov x5, #0");
  L(movE);

  L(ldrS);
  _("ldr x5, [x2]");
  L(ldrE);

  L(strS);
  _("str x5, [x2, #8]");
  L(strE);

  L(add_x5_11_11S);
  _("add x5, x11, x11");
  L(add_x5_11_11E);

  L(add_v0_1_2S);
  _("add v0.16b, v1.16b, v2.16b");
  L(add_v0_1_2E);

  L(add_x5_0_1S);
  _("add x5, x0, x1");
  L(add_x5_0_1E);

  L(ldrlistS);
  _("ldr x0, [x2, x0, lsl #3]");
  L(ldrlistE);

  L(ldrlist2S);
  _("ldr x1, [x3, x1, lsl #3]");
  L(ldrlist2E);

  L(fenceS);
  _("dmb ish");
  _("isb");
  L(fenceE);

  L(loopnS);
  _("subs w4, w4, #1");
  _("b.ne #0");
  L(loopnE);

  L(loopaltS);
  _("sub w4, w4, #1");
  _("cbnz w4, #0");
  L(loopaltE);
}

#define DL(name_)                                                              \
  extern u32 _stub_##name_##S __asm__("stub_" #name_ "S");                     \
  u32 *stub_##name_##S = &_stub_##name_##S;                                    \
  extern u32 _stub_##name_##E __asm__("stub_" #name_ "E");                     \
  u32 *stub_##name_##E = &_stub_##name_##E

DL(prologue);
DL(epilogue);
DL(vepilogue);
DL(vprologue);
DL(prep);
DL(vprep);
DL(nop);
DL(mov);
DL(ldr);
DL(str);
DL(add_x5_11_11);
DL(add_v0_1_2);
DL(add_x5_0_1);
DL(ldrlist);
DL(ldrlist2);
DL(fence);
DL(loopn);

static void shuffle(unsigned *array, usize n) {
  if (n > 1) {
    usize i;
    for (i = 0; i < n - 1; i++) {
      usize j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

static void init_dbufs(u64 **out_data1, u64 **out_data2) {
  usize size = 256 * 1024 * 1024;
  usize cache_line_size = 64;
  usize count = size / cache_line_size;
  usize stride = cache_line_size / sizeof(void *);
  unsigned *numbers = malloc(count * sizeof(int));
  for (unsigned i = 0; i < count; i++) {
    numbers[i] = i;
  }
  shuffle(numbers, count);

  u64 *data1 = calloc(size, 1);
  u64 *data2 = (u64 *)((char *)calloc(size + 64, 1) + 64);
  int next = numbers[count - 1];
  for (unsigned i = 0; i < count; i++) {
    int n = numbers[i];
    data1[stride * n] = next * stride;
    data2[stride * n] = next * stride;
    next = n;
  }

  *out_data1 = data1;
  *out_data2 = data2;
  free(numbers);
}

enum {
  MaxOOOWindow,
  MaxRename,
  LoadBufferSize,
  StoreBufferSize,
  GPRFileSize,
  VRFileSize,
  SchedulerSize,
};

void codegen(u32 *ibuf, int seed, int unroll, int ty) {
  (void)unroll;
  pthread_jit_write_protect_np(0);
  int o = 0;

#define PUSH_IBUF(name_)                                                       \
  memcpy(ibuf + o, stub_##name_##S,                                            \
         (char *)stub_##name_##E - (char *)stub_##name_##S);                   \
  o += stub_##name_##E - stub_##name_##S;

  if (ty == VRFileSize) {
    PUSH_IBUF(vprologue);
    PUSH_IBUF(vprep);
  } else if (ty == GPRFileSize) {
    PUSH_IBUF(prologue);
    PUSH_IBUF(prep);
  } else {
    PUSH_IBUF(prologue);
  }

  int start = o;

  const int lc = 1;
  for (int i = 0; i < lc; i++)
    PUSH_IBUF(ldrlist);

  for (int i = 0; i < seed; i++) {
    switch (ty) {
    case MaxOOOWindow:
      PUSH_IBUF(nop);
      break;
    case MaxRename:
      PUSH_IBUF(mov);
      break;
    case LoadBufferSize:
      PUSH_IBUF(ldr);
      break;
    case StoreBufferSize:
      PUSH_IBUF(str);
      break;
    case GPRFileSize:
      PUSH_IBUF(add_x5_11_11);
      break;
    case VRFileSize:
      PUSH_IBUF(add_v0_1_2);
      break;
    case SchedulerSize:
      PUSH_IBUF(add_x5_0_1);
      break;
    }
  }

  for (int i = 0; i < lc; i++)
    PUSH_IBUF(ldrlist2);

  PUSH_IBUF(fence);

  PUSH_IBUF(loopn);

  int offset = start - o + 1;
  assert(offset < 0 && offset > -0x40000);
  ibuf[o - 1] |= (offset & 0x7ffff) << 5;

  if (ty == VRFileSize) {
    PUSH_IBUF(vepilogue);
  } else {
    PUSH_IBUF(epilogue);
  }

  pthread_jit_write_protect_np(1);
  sys_icache_invalidate(ibuf, o * 4);
}

static int get_current_core() {
  uint64_t out = 0;
  __asm__ volatile("isb\n\tmrs %0, tpidr_el0\n\tisb\n\t" : "=r"(out));
  return out & 0xfff;
}

static void *expensive(void *arg) {
  (void) arg;
  pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
  int v = get_current_core();
  while (1) {
    while (get_current_core() == v)
      ;
    v = get_current_core();
  }
  return NULL;
}

int main(int argc, char **argv) {
  const int test_high_perf_cores = 1;
  const int NUM_BIG_CORES = 10;
  if (test_high_perf_cores) {
    pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
  } else {
    pthread_t ids[NUM_BIG_CORES];
    for (int i = 0; i < NUM_BIG_CORES; i++) {
      if (pthread_create(&ids[i], NULL, expensive, NULL) != 0) {
        perror("pthread_create() error");
        exit(1);
      }
    }
    pthread_set_qos_class_self_np(QOS_CLASS_BACKGROUND, QOS_MIN_RELATIVE_PRIORITY);
  }

  if (!lib_init()) {
    printf("Error: %s\n", lib_err_msg);
    return -1;
  }

  struct perf_counters counters;
  if (!init_perf_counters(&counters))
    return -1;

  u64 *data1, *data2;
  init_dbufs(&data1, &data2);

  if (argc < 7) {
    printf("Usage: %s <start_seed> <end_seed> <stride> <repeat> <iteration> "
           "<unroll> <type>\n",
           argv[0]);
    return -1;
  }

  int start_seed = strtol(argv[1], NULL, 0);
  int end_seed = strtol(argv[2], NULL, 0);
  int stride = strtol(argv[3], NULL, 0);
  int repeat = strtol(argv[4], NULL, 0);
  int iteration = strtol(argv[5], NULL, 0);
  int unroll = strtol(argv[6], NULL, 0);
  int type = strtol(argv[7], NULL, 0);

  printf("start_seed: %d, end_seed: %d, stride: %d, repeat: %d, iteration: %d, "
         "unroll: %d, type: %d\n",
         start_seed, end_seed, stride, repeat, iteration, unroll, type);

  u32 *ibuf = mmap(NULL, 0x400000, PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_ANON | MAP_PRIVATE | MAP_JIT, -1, 0);

  u64 next = 0;
  for (int seed = start_seed; seed < end_seed; seed += stride) {
    u64 min_diff = 0x7fffffffffffffffULL;
    u64 max_diff = 0;
    u64 sum_diff = 0;

    codegen(ibuf, seed, 1, type);
    u64 (*pfn)(u64, u64, void *, void *, u64) = (void *)ibuf;

    next = pfn(next, next, data1, data2, iteration);

    for (int n = 0; n < repeat; n++) {
      config_perf_counters(&counters);

      int start_core = get_current_core();
      atomic_thread_fence(memory_order_acquire);
      u64 start = read_perf_counters_before(&counters);

      next = pfn(next, next, data1, data2, iteration);

      atomic_thread_fence(memory_order_release);
      u64 end = read_perf_counters_after(&counters);
      int end_core = get_current_core();

      if (start_core != end_core || (start_core < NUM_BIG_CORES ) != test_high_perf_cores) {
        n--;
        continue;
      }

      u64 diff = end - start;
      sum_diff += diff;
      if (diff < min_diff)
        min_diff = diff;
      if (diff > max_diff)
        max_diff = diff;
    }

    printf("[%d]\t%.4f\t%.4f\t%.4f\n", seed,
           1.0 * min_diff / (iteration * unroll),
           1.0 * sum_diff / (iteration * unroll * repeat),
           1.0 * max_diff / (iteration * unroll));
  }

  pthread_set_qos_class_self_np(QOS_CLASS_DEFAULT, 0);
}
