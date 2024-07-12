// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

class Class {
public:
  // test - destructors should use `void (*)(void*)`
// CHECK-LABEL: define{{.*}} @_ZN5ClassD1Ev({{.*}})
// CHECK-SAME: {{.* !cfi_type}} [[CFI_MD:![0-9]+]] {{.*}}{
// CHECK: [[CFI_MD]] = !{i32 408002}
//
  ~Class() {}
};

void use() {
  Class C;
}
