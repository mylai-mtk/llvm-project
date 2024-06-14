// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -triple riscv64 -target-cpu generic-rv64 -target-feature \
// RUN:  +experimental-zicfilp -fcf-protection=branch \
// RUN:  -mcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s

// CHECK-LABEL: define{{.*}} @_Z16indirectCallFptrPFvvE(
// CHECK-NEXT:    entry:
// CHECK:           call void @llvm.riscv.set.lpad.label.i64
//
void indirectCallFptr(void (*Fptr)()) { Fptr(); }

class Class;
// CHECK-LABEL: define{{.*}} @_Z21indirectCallMemberPtrP5ClassMS_FS0_vE(
// CHECK-NEXT:    entry:
// CHECK:         memptr.virtual:
// CHECK:           call void @llvm.riscv.set.lpad.label.i64
// CHECK:         memptr.nonvirtual:
// CHECK:           call void @llvm.riscv.set.lpad.label.i64
//
// we pick `Class*` as the return type to show that the labels can be different
// for virtual and non-virtual branches
void indirectCallMemberPtr(Class *ClsPtr, Class *(Class::*Fptr)()) {
  (ClsPtr->*Fptr)();
}
