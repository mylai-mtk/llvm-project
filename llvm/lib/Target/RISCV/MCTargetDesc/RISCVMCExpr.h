//===-- RISCVMCExpr.h - RISC-V specific MC expression classes----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes RISC-V specific MCExprs, used for modifiers like
// "%hi" or "%lo" etc.,
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVMCEXPR_H
#define LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVMCEXPR_H

#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCExpr.h"

namespace llvm {

class StringRef;

class RISCVMCExpr : public MCTargetExpr {
public:
  enum VariantKind {
    VK_RISCV_None,
    VK_RISCV_LO,
    VK_RISCV_HI,
    VK_RISCV_PCREL_LO,
    VK_RISCV_PCREL_HI,
    VK_RISCV_GOT_HI,
    VK_RISCV_TPREL_LO,
    VK_RISCV_TPREL_HI,
    VK_RISCV_TPREL_ADD,
    VK_RISCV_TLS_GOT_HI,
    VK_RISCV_TLS_GD_HI,
    VK_RISCV_CALL,
    VK_RISCV_CALL_PLT,
    VK_RISCV_32_PCREL,
    VK_RISCV_TLSDESC_HI,
    VK_RISCV_TLSDESC_LOAD_LO,
    VK_RISCV_TLSDESC_ADD_LO,
    VK_RISCV_TLSDESC_CALL,
    VK_RISCV_LPAD_LABEL,
    VK_RISCV_LPAD_HASH,
    VK_RISCV_Invalid // Must be the last item
  };

private:
  const MCExpr *Expr;
  const VariantKind Kind;

  int64_t evaluateAsInt64(int64_t Value) const;

protected:
  explicit RISCVMCExpr(const MCExpr *Expr, VariantKind Kind)
      : Expr(Expr), Kind(Kind) {}

public:
  static const RISCVMCExpr *create(const MCExpr *Expr, VariantKind Kind,
                                   MCContext &Ctx);

  VariantKind getKind() const { return Kind; }

  const MCExpr *getSubExpr() const { return Expr; }

  /// Get the corresponding PC-relative HI fixup that a VK_RISCV_PCREL_LO
  /// points to, and optionally the fragment containing it.
  ///
  /// \returns nullptr if this isn't a VK_RISCV_PCREL_LO pointing to a
  /// known PC-relative HI fixup.
  const MCFixup *getPCRelHiFixup(const MCFragment **DFOut) const;

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAssembler *Asm,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override {
    return getSubExpr()->findAssociatedFragment();
  }

  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override;

  bool evaluateAsConstant(int64_t &Res) const;

  static bool classof(const MCExpr *E) {
    return E->getKind() == MCExpr::Target;
  }

  static VariantKind getVariantKindForName(StringRef name);
  static StringRef getVariantKindName(VariantKind Kind);
};

class RISCVLpadHashMCExpr : public RISCVMCExpr {
  explicit RISCVLpadHashMCExpr(const MCExpr *const Expr,
                               const StringRef CFITokSrc)
      : RISCVMCExpr(Expr, VK_RISCV_LPAD_HASH), CFITokSrc(CFITokSrc) {}

  /// The hash source of the contained CFI token. If empty, it means the CFI
  /// token hash source string is empty
  const StringRef CFITokSrc;

public:
  StringRef getCFITokSrc() const { return CFITokSrc; }

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;

  static const RISCVLpadHashMCExpr *
  create(const uint32_t CFITok, const StringRef CFITokSrc, MCContext &Ctx);

  static bool classof(const MCExpr *E) {
    if (!RISCVMCExpr::classof(E))
      return false;
    const VariantKind Kind = static_cast<const RISCVMCExpr *>(E)->getKind();
    return Kind == VK_RISCV_LPAD_HASH;
  }
};

} // end namespace llvm.

#endif
