#ifndef __LIB_BUILTINS_RISCV_LPAD_H
#define __LIB_BUILTINS_RISCV_LPAD_H

#if defined(__riscv_zicfilp) && defined(__riscv_landing_pad)
# define GNU_PROPERTY_RISCV_FEATURE_1_AND (0xc0000000)

# define GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_UNLABELED  (1 << 0)
# define GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_FUNC_SIG   (1 << 2)

# if defined(__riscv_landing_pad_unlabeled)
#   define LPAD(label) .align 4; lpad 0
#   define RISCV_ZICFILP_FEATURE_1_AND                                         \
           GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_UNLABELED
# elif defined(__riscv_landing_pad_func_sig)
#   define LPAD(label) .align 4; lpad label
#   define RISCV_ZICFILP_FEATURE_1_AND                                         \
           GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_FUNC_SIG
# else
#   error "Unsupported RISC-V Zicfilp CFI scheme"
# endif

# define RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION                               \
         .pushsection .note.gnu.property, "a"   ;                              \
         .p2align 3                             ;                              \
         .word 4                                ;                              \
         .word 16                               ;                              \
         .word 5                                ;                              \
         .asciz "GNU"                           ;                              \
         .word GNU_PROPERTY_RISCV_FEATURE_1_AND ;                              \
         .word 4                                ;                              \
         .word RISCV_ZICFILP_FEATURE_1_AND      ;                              \
         .word 0                                ;                              \
         .popsection                            ;
#else
# define LPAD(label)
# define RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
#endif

#endif // # ifndef __LIB_BUILTINS_RISCV_LPAD_H
