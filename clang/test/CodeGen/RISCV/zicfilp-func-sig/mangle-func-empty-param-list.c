// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 -target-feature \
// RUN:  +experimental-zicfilp -fcf-protection=branch \
// RUN:  -fcf-branch-label-scheme=func-sig -emit-llvm -o - -x c %s \
// RUN:  | FileCheck %s

// test - functions with an empty parameter list are treated as `void (*)(void)`
// CHECK-LABEL: define{{.*}} @funcWithEmptyParameterList()
// CHECK-SAME: {{.* !riscv_lpad_label}} [[MD:![0-9]+]] {{.*}}{
//
void funcWithEmptyParameterList() {}
// CHECK-LABEL: define{{.*}} @funcWithVoidParameterList()
// CHECK-SAME: {{.* !riscv_lpad_label}} [[MD]] {{.*}}{
//
void funcWithVoidParameterList(void) {}
