// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

// test - main functions should use `int (*)(int, char**)`
// CHECK-LABEL: define{{.*}} @main()
// CHECK-SAME: {{.* !cfi_type}} [[CFI_MD:![0-9]+]] {{.*}}{
// CHECK: [[CFI_MD]] = !{i32 853561}
//
int main() { return 0; }
