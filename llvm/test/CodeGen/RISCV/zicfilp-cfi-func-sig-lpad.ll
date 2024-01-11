; RUN: llc -mtriple riscv32 -mattr=+experimental-zicfilp < %s | FileCheck %s
; RUN: llc -mtriple riscv64 -mattr=+experimental-zicfilp < %s | FileCheck %s
define dso_local signext i32 @non_local_func() !zicfilp_func_sig_label !2 {
; CHECK-LABEL: non_local_func:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    lpad 796407
entry:
  ret i32 0
}

!llvm.module.flags = !{!0, !1}

!0 = !{i32 8, !"cf-protection-branch", i32 1}
!1 = !{i32 1, !"cf-branch-label-scheme", !"func-sig"}
!2 = !{i32 796407}
