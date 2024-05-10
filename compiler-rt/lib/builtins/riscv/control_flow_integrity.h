#ifndef __LIB_BUILTINS_RISCV_CONTROL_FLOW_INTEGRITY_H
#define __LIB_BUILTINS_RISCV_CONTROL_FLOW_INTEGRITY_H

#if defined(__riscv_zicfilp) && defined(__riscv_landing_pad)
# define NT_GNU_PROPERTY_TYPE_0 (5)
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

  .pushsection ".note.gnu.property", "a"
  .balign (__riscv_xlen >> 3)
  .4byte 4                                /* n_namsz */
  .4byte desc_end - desc_begin            /* n_descsz */
  .4byte NT_GNU_PROPERTY_TYPE_0           /* n_type */
  .asciz "GNU"                            /* n_name */
desc_begin:
  .balign (__riscv_xlen >> 3)
  .4byte GNU_PROPERTY_RISCV_FEATURE_1_AND /* pr_type */
  .4byte 4                                /* pr_datasz */
  .4byte RISCV_ZICFILP_FEATURE_1_AND      /* pr_data */
  .balign (__riscv_xlen >> 3)             /* pr_padding */
desc_end:
  .popsection

#else
# define LPAD(label)
#endif

#endif // # ifndef __LIB_BUILTINS_RISCV_CONTROL_FLOW_INTEGRITY_H
