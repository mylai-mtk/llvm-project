// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c %s | FileCheck %s

// test - functions with an empty parameter list are treated as `void (*)(void)`
// CHECK-LABEL: define{{.*}} @funcWithEmptyParameterList()
// CHECK-SAME: {{.* !cfi_type}} [[EMPTY_PARAM_LIST_CFI_MD:![0-9]+]] {{.*}}{
//
void funcWithEmptyParameterList() {}
// CHECK-LABEL: define{{.*}} @funcWithVoidParameterList()
// CHECK-SAME: {{.* !cfi_type}} [[EMPTY_PARAM_LIST_CFI_MD]] {{.*}}{
//
void funcWithVoidParameterList(void) {}
