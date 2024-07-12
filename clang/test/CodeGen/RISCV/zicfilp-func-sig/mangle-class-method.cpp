// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

class Class {
public:
  // test - C++ member functions should use "pointer-to-member" types, with
  //        `<class type>` being `1v` (i.e. using a dummy class named `v`)
// CHECK-LABEL: define{{.*}} @_ZN5Class14instanceMethodEv({{.*}})
// CHECK-SAME: {{.* !cfi_type}} [[CFI_MD:![0-9]+]] {{.*}}{
// CHECK: [[CFI_MD]] = !{i32 974748}
//
  void instanceMethod() {}
};

void use(Class &C) {
  C.instanceMethod();
}
