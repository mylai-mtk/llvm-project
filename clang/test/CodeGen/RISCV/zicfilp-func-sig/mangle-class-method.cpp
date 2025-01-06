// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 -target-feature \
// RUN:  +experimental-zicfilp -fcf-protection=branch \
// RUN:  -mcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s

class Class {
public:
  // test - C++ member functions should use "pointer-to-member-type" mangling
  //        rule, with `<class type>` being `1v` instead of the real class
  //        (i.e. use a dummy class named `v`)
  // CHECK-LABEL: define{{.*}} @_ZN5Class14instanceMethodEv({{.*}})
  // CHECK-SAME: {{.* !riscv_lpad_func_sig}} [[MD:![0-9]+]] {{.*}}{
  // CHECK: [[MD]] = !{!"M1vFvvE"}
  //
  void instanceMethod() {}
};

void use(Class &C) { C.instanceMethod(); }
