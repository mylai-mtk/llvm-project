// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 -target-feature \
// RUN:  +experimental-zicfilp -fcf-protection=branch \
// RUN:  -fcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s

class Class {
public:
  // test - virtual methods with return type that can possibly be covariant
  //        mangle return type as `void *`
  // CHECK-LABEL: define{{.*}} @_ZN5Class32virtualMethodWithCovariantReturnEv
  // CHECK-SAME: ({{.*}}){{.* !riscv_lpad_label}} [[MD:![0-9]+]] {{.*}} {
  // CHECK: [[MD]] = !{i32 126408}
  //
  virtual Class *virtualMethodWithCovariantReturn() { return this; }
};

void use() { Class C; }
