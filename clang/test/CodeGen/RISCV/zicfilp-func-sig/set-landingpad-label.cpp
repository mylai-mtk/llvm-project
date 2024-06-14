// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -mzicfilp-label-scheme=func_sig \
// RUN:  -emit-llvm -o - -x c++ %s | FileCheck %s

// CHECK-LABEL: define{{.*}} @_Z16indirectCallFptrPFvvE(
// CHECK-NEXT:    entry:
// CHECK:           call void @llvm.riscv.set.lpad.label.i64(i64 308871)
//
void indirectCallFptr(void (*Fptr)()) {
  Fptr();
}

class Class;
// CHECK-LABEL: define{{.*}} @_Z21indirectCallMemberPtrP5ClassMS_FS0_vE(
// CHECK-NEXT:    entry:
// CHECK:         memptr.virtual:
// CHECK:           call void @llvm.riscv.set.lpad.label.i64(i64 126408)
// CHECK:         memptr.nonvirtual:
// CHECK:           call void @llvm.riscv.set.lpad.label.i64(i64 789674)
//
// we pick `Class*` as the return type to show that the labels can be different
// for virtual and non-virtual branches
void indirectCallMemberPtr(Class *ClsPtr, Class* (Class::*Fptr)()) {
  (ClsPtr->*Fptr)();
}
