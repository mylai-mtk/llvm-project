; RUN: llc -mtriple riscv32 -mattr=+experimental-zicfilp < %s | FileCheck %s
; RUN: llc -mtriple riscv64 -mattr=+experimental-zicfilp < %s | FileCheck %s
define dso_local signext i32 @non_local_func() !cfi_type !1 {
; CHECK-LABEL: non_local_func:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lpad 796407
entry:
  ret i32 0
}

!llvm.module.flags = !{!0}

!0 = !{i32 4, !"zicfilp-cfi-func-sig", i32 1}
!1 = !{i32 796407}
