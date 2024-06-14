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
