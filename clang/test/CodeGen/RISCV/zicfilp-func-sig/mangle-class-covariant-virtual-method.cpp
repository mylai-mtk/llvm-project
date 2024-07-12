// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

class Class {
public:
  // test - virtual methods with return type that can possibly be covariant
  //        mangle return type as `void *`
// CHECK-LABEL: define{{.*}} @_ZN5Class32virtualMethodWithCovariantReturnEv
// CHECK-SAME: ({{.*}}){{.* !cfi_type}} [[CFI_MD:![0-9]+]] {{.*}}{
// CHECK: [[CFI_MD]] = !{i32 126408}
//
  virtual Class* virtualMethodWithCovariantReturn() { return this; }
};

void use() {
  Class C;
}
