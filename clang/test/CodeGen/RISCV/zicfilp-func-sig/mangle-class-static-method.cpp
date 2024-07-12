// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

// CHECK-LABEL: define{{.*}} @_Z13nonMemberFuncv()
// CHECK-SAME: {{.* !cfi_type}} [[NON_MEMBER_FUNC_CFI_MD:![0-9]+]] {{.*}}{
//
void nonMemberFunc() {}

class Class {
public:
  // test - static methods are mangled as non-member function
// CHECK-LABEL: define{{.*}} @_ZN5Class12staticMethodEv()
// CHECK-SAME: {{.* !cfi_type}} [[NON_MEMBER_FUNC_CFI_MD]] {{.*}}{
//
  static void staticMethod() {}
};

void use() {
  Class::staticMethod();
}
