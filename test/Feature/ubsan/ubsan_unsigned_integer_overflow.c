// RUN: %clang %s -fsanitize=unsigned-integer-overflow -emit-llvm -g %O0opt -c -o %t.bc
// RUN: rm -rf %t.klee-out
// RUN: %klee --output-dir=%t.klee-out --emit-all-errors --ubsan-runtime --runtime-build=Debug+Asserts %t.bc 2>&1 | FileCheck %s
// RUN: ls %t.klee-out/ | grep .ktest | wc -l | grep 5
// RUN: ls %t.klee-out/ | grep .overflow.err | wc -l | grep 4

#include "klee/klee.h"

int main() {
  unsigned int x;
  unsigned int y;
  volatile unsigned int result;

  klee_make_symbolic(&x, sizeof(x), "x");
  klee_make_symbolic(&y, sizeof(y), "y");

  // CHECK: KLEE: ERROR: {{.*}}runtime/Sanitizer/ubsan/ubsan_handlers.cpp:{{[0-9]+}}: unsigned-integer-overflow
  result = x + y;

  // CHECK: KLEE: ERROR: {{.*}}runtime/Sanitizer/ubsan/ubsan_handlers.cpp:{{[0-9]+}}: unsigned-integer-overflow
  result = x - y;

  // CHECK: KLEE: ERROR: {{.*}}runtime/Sanitizer/ubsan/ubsan_handlers.cpp:{{[0-9]+}}: unsigned-integer-overflow
  result = x * y;

  // CHECK: KLEE: ERROR: {{.*}}runtime/Sanitizer/ubsan/ubsan_handlers.cpp:{{[0-9]+}}: unsigned-integer-overflow
  result = -x;

  return 0;
}
