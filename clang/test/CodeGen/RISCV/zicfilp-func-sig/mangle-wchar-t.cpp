// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

// test - `wchar_t` in C++ should be mangled to `wchar_t` in C
// CHECK-LABEL: define{{.*}} @_Z14funcWithWCharTw({{.*}})
// CHECK-SAME: {{.* !cfi_type}} [[CFI_MD:![0-9]+]] {{.*}}{
// CHECK: [[CFI_MD]] = !{i32 374765}
//
void funcWithWCharT(wchar_t) {}
