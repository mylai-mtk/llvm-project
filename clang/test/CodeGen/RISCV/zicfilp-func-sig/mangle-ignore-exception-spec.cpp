// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -fcxx-exceptions -fexceptions -emit-llvm -o - -x c++ %s | FileCheck %s

// test - `<exception-spec>` should be ignored
// CHECK-LABEL: define{{.*}} @_Z9funcThrowv()
// CHECK-SAME: {{.* !cfi_type}} [[THROW_CFI_MD:![0-9]+]] {{.*}}{
///
void funcThrow() { throw 0; }
// CHECK-LABEL: define{{.*}} @_Z12funcNoExceptv()
// CHECK-SAME: {{.* !cfi_type}} [[THROW_CFI_MD]] {{.*}}{
//
void funcNoExcept() noexcept {}
