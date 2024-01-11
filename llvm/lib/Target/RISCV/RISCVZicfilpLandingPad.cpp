//===-- RISCVZicfilpLandingPad.cpp -- Insert Zicfilp lpad landing pads -----==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass inserts lpad instructions at the start of every function and basic
// block which could be indirectly called. The hardware will (when enabled)
// fault when an indirect branch instruction targets an instruction which is
// not a valid and matching lpad instruction. This is intended to guard against
// control-flow hijacking attacks.
//
//===----------------------------------------------------------------------===//

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "MCTargetDesc/RISCVMCTargetDesc.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

#define DEBUG_TYPE "riscv-zicfilp-cfi"
#define PASS_NAME "Insert RISCV Zicfilp CFI callee site landing pad"

namespace {
class RISCVZicfilpLandingPad : public MachineFunctionPass {
public:
  static char ID;
  RISCVZicfilpLandingPad() : MachineFunctionPass(ID), LpadAlign(4ULL) {}
  void getAnalysisUsage(AnalysisUsage &AU) const override;
  bool runOnMachineFunction(MachineFunction &MF) override;
  StringRef getPassName() const override { return PASS_NAME; }

private:
  bool addLpad(MachineBasicBlock &MBB, const uint32_t Label = 0);
  inline bool isLpad(const MachineInstr &MI);
  inline bool isLpad(const MachineInstr &MI, const uint32_t Label);

  const RISCVInstrInfo * TII = nullptr;
  const Align LpadAlign;
};
} // end anonymous namespace

char RISCVZicfilpLandingPad::ID = 0;

INITIALIZE_PASS(RISCVZicfilpLandingPad, DEBUG_TYPE, PASS_NAME, false, false)

void RISCVZicfilpLandingPad::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
  MachineFunctionPass::getAnalysisUsage(AU);
}

FunctionPass *llvm::createRISCVZicfilpLandingPadPass() {
  return new RISCVZicfilpLandingPad();
}

bool RISCVZicfilpLandingPad::runOnMachineFunction(MachineFunction &MF) {
  const Module * const M = MF.getMMI().getModule();
  if (!M->getModuleFlag("zicfilp-cfi-simple") &&
      !M->getModuleFlag("zicfilp-cfi-func-sig"))
    return false;

  LLVM_DEBUG(
      dbgs() << "********** RISCV Zicfilp Landing Pad **********\n"
             << "********** Function: " << MF.getName() << '\n');
  TII = static_cast<const RISCVInstrInfo *>(MF.getSubtarget().getInstrInfo());
  bool Changed = false;

  if (const Function &F = MF.getFunction();
      F.hasAddressTaken() || !F.hasLocalLinkage()) {
    uint32_t Label = 0;
    if (auto MD = F.getMetadata(LLVMContext::MD_cfi_type))
      Label = mdconst::extract<ConstantInt>(MD->getOperand(0))->getZExtValue();

    if (addLpad(*MF.begin(), Label)) {
      MF.ensureAlignment(LpadAlign);
      Changed = true;
    }
  }

  for (MachineBasicBlock &MBB : MF) {
    if (MBB.hasAddressTaken()) {
      Changed |= addLpad(MBB);
    }
  }

  return Changed;
}

bool RISCVZicfilpLandingPad::addLpad(MachineBasicBlock &MBB, const uint32_t Label) {
  assert(Label < (1u << 20) && "lpad label can hold only 20 bits");
  LLVM_DEBUG(dbgs() << "Add lpad " << Label << " to " << MBB.getName() << '\n');

  if (!MBB.empty() && isLpad(*MBB.instr_begin(), Label)) {
    return false;
  }

  BuildMI(MBB, MBB.begin(), MBB.findDebugLoc(MBB.begin()),
          TII->get(RISCV::AUIPC)).addReg(RISCV::X0).addImm(Label);
  if (MBB.getAlignment() < LpadAlign) {
    MBB.setAlignment(LpadAlign);
  }
  return true;
}

inline bool RISCVZicfilpLandingPad::isLpad(const MachineInstr &MI) {
  return MI.getOpcode() == RISCV::AUIPC &&
    MI.getOperand(0).getReg() == RISCV::X0;
}

inline bool RISCVZicfilpLandingPad::isLpad(const MachineInstr &MI, const uint32_t Label) {
  return isLpad(MI) && MI.getOperand(1).getImm() == Label;
}
