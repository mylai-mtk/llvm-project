//===- MCSymbolELF.h -  -----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_MC_MCSYMBOLELF_H
#define LLVM_MC_MCSYMBOLELF_H

#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCSymbolTableEntry.h"
#include <optional>

namespace llvm {
class MCSymbolELF : public MCSymbol {
  /// An expression describing how to calculate the size of a symbol. If a
  /// symbol has no size this field will be NULL.
  const MCExpr *SymbolSize = nullptr;

  struct SymTabFieldsT {
    uint32_t StName = 0;
    unsigned char StInfo = 0;
    unsigned char StOther = 0;
    uint16_t StShndx = 0;
    uint64_t StValue = 0;
    uint64_t StSize = 0;
  };
  mutable std::optional<SymTabFieldsT> SymTabFields;

public:
  MCSymbolELF(const MCSymbolTableEntry *Name, bool isTemporary)
      : MCSymbol(SymbolKindELF, Name, isTemporary) {}
  void setSize(const MCExpr *SS) { SymbolSize = SS; }

  const MCExpr *getSize() const { return SymbolSize; }

  void setVisibility(unsigned Visibility);
  unsigned getVisibility() const;

  void setOther(unsigned Other);
  unsigned getOther() const;

  void setType(unsigned Type) const;
  unsigned getType() const;

  void setBinding(unsigned Binding) const;
  unsigned getBinding() const;

  bool isBindingSet() const;

  void setIsWeakrefUsedInReloc() const;
  bool isWeakrefUsedInReloc() const;

  void setIsSignature() const;
  bool isSignature() const;

  void setMemtag(bool Tagged);
  bool isMemtag() const;

  /// Check if this symbol is already finalized and written out to .symtab
  bool isFinalizedInSymTab() const { return SymTabFields.has_value(); }

  /// Set .symtab fields as written out to ELF file
  void setFinalSymTabFields(const uint32_t Name, unsigned char Info,
                            unsigned char Other, uint16_t Shndx, uint64_t Value,
                            uint64_t Size) const {
    assert(!SymTabFields.has_value() &&
           "Should not need to set final symbol table fields more than once!");
    SymTabFields = SymTabFieldsT{Name, Info, Other, Shndx, Value, Size};
  }

  uint32_t getStName() const { return SymTabFields.value().StName; }
  unsigned char getStInfo() const { return SymTabFields.value().StInfo; }
  unsigned char getStOther() const { return SymTabFields.value().StOther; }
  uint16_t getStShndx() const { return SymTabFields.value().StShndx; }
  uint64_t getStValue() const { return SymTabFields.value().StValue; }
  uint64_t getStSize() const { return SymTabFields.value().StSize; }

  static bool classof(const MCSymbol *S) { return S->isELF(); }

private:
  void setIsBindingSet() const;
};
}

#endif
