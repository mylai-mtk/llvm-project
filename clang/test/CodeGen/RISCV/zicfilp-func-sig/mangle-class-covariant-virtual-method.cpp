// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 -target-feature \
// RUN:  +experimental-zicfilp -fcf-protection=branch \
// RUN:  -mcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s --check-prefixes=PTR,LREF,RREF

class Class {
public:
  // test - virtual methods with return type that can possibly be covariant
  //        mangle return class as `class v`
  // PTR-LABEL: define{{.*}} @_ZN5Class35virtualMethodWithCovariantPtrReturnEv
  // PTR-SAME: ({{.*}}){{.* !riscv_lpad_func_sig}} [[MD_PTR:![0-9]+]] {{.*}}{
  //
  virtual Class *virtualMethodWithCovariantPtrReturn() { return this; }

  // LREF-LABEL: define{{.*}} @_ZN5Class36virtualMethodWithCovariantLRefReturnEv
  // LREF-SAME: ({{.*}}){{.* !riscv_lpad_func_sig}} [[MD_LREF:![0-9]+]] {{.*}}{
  //
  virtual Class &virtualMethodWithCovariantLRefReturn() { return *this; }

  // RREF-LABEL: define{{.*}} @_ZN5Class36virtualMethodWithCovariantRRefReturnEv
  // RREF-SAME: ({{.*}}){{.* !riscv_lpad_func_sig}} [[MD_RREF:![0-9]+]] {{.*}}{
  //
  virtual Class &&virtualMethodWithCovariantRRefReturn() {
    return static_cast<Class&&>(*this);
  }
};

// PTR-DAG: [[MD_PTR]] = !{!"M1vFP1vvE"}
// LREF-DAG: [[MD_LREF]] = !{!"M1vFR1vvE"}
// RREF-DAG: [[MD_RREF]] = !{!"M1vFO1vvE"}

void use() { Class C; }
