# REQUIRES: riscv
# RUN: echo '.globl foo; .type foo, @function; foo:' > %t1.s

# RUN: llvm-mc -filetype=obj -triple=riscv32 %t1.s -o %t1.32.o
# RUN: ld.lld -shared %t1.32.o -soname=t1.32.so -o %t1.32.so
# RUN: llvm-mc -filetype=obj -triple=riscv32 %s -o %t.32.o
# RUN: ld.lld %t.32.o %t1.32.so -z force-zicfilp=simple -o %t.32
# RUN: llvm-readelf -S %t.32 | FileCheck --check-prefixes=SEC32 %s
# RUN: llvm-objdump -d --no-show-raw-insn --mattr=experimental-zicfilp %t.32 \
# RUN:   | FileCheck --check-prefixes=DIS,DIS32 %s

# RUN: llvm-mc -filetype=obj -triple=riscv64 %t1.s -o %t1.64.o
# RUN: ld.lld -shared %t1.64.o -soname=t1.64.so -o %t1.64.so
# RUN: llvm-mc -filetype=obj -triple=riscv64 %s -o %t.64.o
# RUN: ld.lld %t.64.o %t1.64.so -z force-zicfilp=simple -o %t.64
# RUN: llvm-readelf -S %t.64 | FileCheck --check-prefixes=SEC64 %s
# RUN: llvm-objdump -d --no-show-raw-insn --mattr=experimental-zicfilp %t.64 \
# RUN:   | FileCheck --check-prefixes=DIS,DIS64 %s

# SEC32: .plt     PROGBITS {{0*}}00011210
# SEC32: .got.plt PROGBITS {{0*}}000132b8

# SEC64: .plt     PROGBITS {{0*}}00011330
# SEC64: .got.plt PROGBITS {{0*}}00013440

# DIS:      Disassembly of section .plt:
# DIS:      <.plt>:
# DIS-NEXT:     lpad 0x0
# DIS-NEXT:     auipc t2, 0x2
# DIS-NEXT:     sub t1, t1, t3
# DIS32-NEXT:   lw t3, 0xa4(t2)
# DIS64-NEXT:   ld t3, 0x10c(t2)
# DIS-NEXT:     addi t1, t1, -0x40
# DIS32-NEXT:   addi t0, t2, 0xa4
# DIS64-NEXT:   addi t0, t2, 0x10c
# DIS32-NEXT:   srli t1, t1, 0x2
# DIS64-NEXT:   srli t1, t1, 0x1
# DIS32-NEXT:   lw t0, 0x4(t0)
# DIS64-NEXT:   ld t0, 0x8(t0)
# DIS-NEXT:     jr t3
# DIS-NEXT:     nop
# DIS-NEXT:     nop
# DIS-NEXT:     nop

# DIS-NEXT:     lpad 0x0
# DIS-NEXT:     auipc t3, 0x2
# DIS32-NEXT:   lw t3, 0x7c(t3)
# DIS64-NEXT:   ld t3, 0xec(t3)
# DIS-NEXT:     jalr t1, t3

.global _start, foo

_start:
  call foo@plt
