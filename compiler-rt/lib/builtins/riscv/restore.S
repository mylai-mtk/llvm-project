//===-- restore.S - restore up to 12 callee-save registers ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Multiple entry points depending on number of registers to restore
//
//===----------------------------------------------------------------------===//

// All of the entry points are in the same section since we rely on many of
// them falling through into each other and don't want the linker to
// accidentally split them up, garbage collect, or reorder them.
//
// The entry points are grouped up into 2s for rv64 and 4s for rv32 since this
// is the minimum grouping which will maintain the required 16-byte stack
// alignment.

#include "lpad.h"

  .text

#if __riscv_xlen == 32

  .globl  __riscv_restore_12
  .type   __riscv_restore_12,@function
__riscv_restore_12:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  lw      s11, 12(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_11/10/9/8

  .globl  __riscv_restore_11
  .type   __riscv_restore_11,@function
  .globl  __riscv_restore_10
  .type   __riscv_restore_10,@function
  .globl  __riscv_restore_9
  .type   __riscv_restore_9,@function
  .globl  __riscv_restore_8
  .type   __riscv_restore_8,@function
__riscv_restore_11:
__riscv_restore_10:
__riscv_restore_9:
__riscv_restore_8:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  lw      s10, 0(sp)
  lw      s9,  4(sp)
  lw      s8,  8(sp)
  lw      s7,  12(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_7/6/5/4

  .globl  __riscv_restore_7
  .type   __riscv_restore_7,@function
  .globl  __riscv_restore_6
  .type   __riscv_restore_6,@function
  .globl  __riscv_restore_5
  .type   __riscv_restore_5,@function
  .globl  __riscv_restore_4
  .type   __riscv_restore_4,@function
__riscv_restore_7:
__riscv_restore_6:
__riscv_restore_5:
__riscv_restore_4:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  lw      s6,  0(sp)
  lw      s5,  4(sp)
  lw      s4,  8(sp)
  lw      s3,  12(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_3/2/1/0

  .globl  __riscv_restore_3
  .type   __riscv_restore_3,@function
  .globl  __riscv_restore_2
  .type   __riscv_restore_2,@function
  .globl  __riscv_restore_1
  .type   __riscv_restore_1,@function
  .globl  __riscv_restore_0
  .type   __riscv_restore_0,@function
__riscv_restore_3:
__riscv_restore_2:
__riscv_restore_1:
__riscv_restore_0:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  lw      s2,  0(sp)
  lw      s1,  4(sp)
  lw      s0,  8(sp)
  lw      ra,  12(sp)
  addi    sp, sp, 16
  ret

#elif __riscv_xlen == 64

  .globl  __riscv_restore_12
  .type   __riscv_restore_12,@function
__riscv_restore_12:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  ld      s11, 8(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_11/10

  .globl  __riscv_restore_11
  .type   __riscv_restore_11,@function
  .globl  __riscv_restore_10
  .type   __riscv_restore_10,@function
__riscv_restore_11:
__riscv_restore_10:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  ld      s10, 0(sp)
  ld      s9,  8(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_9/8

  .globl  __riscv_restore_9
  .type   __riscv_restore_9,@function
  .globl  __riscv_restore_8
  .type   __riscv_restore_8,@function
__riscv_restore_9:
__riscv_restore_8:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  ld      s8,  0(sp)
  ld      s7,  8(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_7/6

  .globl  __riscv_restore_7
  .type   __riscv_restore_7,@function
  .globl  __riscv_restore_6
  .type   __riscv_restore_6,@function
__riscv_restore_7:
__riscv_restore_6:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  ld      s6,  0(sp)
  ld      s5,  8(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_5/4

  .globl  __riscv_restore_5
  .type   __riscv_restore_5,@function
  .globl  __riscv_restore_4
  .type   __riscv_restore_4,@function
__riscv_restore_5:
__riscv_restore_4:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  ld      s4,  0(sp)
  ld      s3,  8(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_3/2

  .globl  __riscv_restore_3
  .type   __riscv_restore_3,@function
  .globl  __riscv_restore_2
  .type   __riscv_restore_2,@function
__riscv_restore_3:
__riscv_restore_2:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  ld      s2,  0(sp)
  ld      s1,  8(sp)
  addi    sp, sp, 16
  // fallthrough into __riscv_restore_1/0

  .globl  __riscv_restore_1
  .type   __riscv_restore_1,@function
  .globl  __riscv_restore_0
  .type   __riscv_restore_0,@function
__riscv_restore_1:
__riscv_restore_0:
  LPAD(0) # TODO(mylai-mtk): Use correct label
  ld      s0,  0(sp)
  ld      ra,  8(sp)
  addi    sp, sp, 16
  ret

#else
# error "xlen must be 32 or 64 for save-restore implementation
#endif

RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
