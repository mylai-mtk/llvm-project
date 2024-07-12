// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 -target-feature \
// RUN:  +experimental-zicfilp -fcf-protection=branch \
// RUN:  -fcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s

// test - main functions should use `int (*)(int, char**)`
// CHECK-LABEL: define{{.*}} @main()
// CHECK-SAME: {{.* !riscv_lpad_label}} [[MD:![0-9]+]] {{.*}}{
// CHECK: [[MD]] = !{i32 853561}
//
int main() { return 0; }
