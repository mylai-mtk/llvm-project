// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

class Class {
public:
  // test - virtual methods with return type that cannot be covariant mangle
  //        return type as it is declared
// CHECK-LABEL: define{{.*}} @_ZN5Class34virtualMethodWithIncovariantReturnEv
// CHECK-SAME: ({{.*}}){{.* !cfi_type}} [[CFI_MD:![0-9]+]] {{.*}}{
// CHECK: [[CFI_MD]] = !{i32 910118}
//
  virtual int virtualMethodWithIncovariantReturn() { return 0; }
};

void use() {
  Class C;
}
