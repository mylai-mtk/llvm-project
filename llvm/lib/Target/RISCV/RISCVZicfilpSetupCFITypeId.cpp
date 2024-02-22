//===---- RISCVZicfilpSetupCFITypeId.cpp - 
//                          Setup CFI at call sites according to Zicfilp --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass picks up the CFI type ID attached to call insns and sets up
// register X7 according to the Zicfilp extension specification.
//
//===----------------------------------------------------------------------===//

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "RISCVSubtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/InitializePasses.h"

using namespace llvm;

#define DEBUG_TYPE "riscv-zicfilp-cfi"
#define PASS_NAME "Setup Zicfilp CFI call site type ID"

namespace {
class RISCVZicfilpSetupCFITypeId : public MachineFunctionPass {
public:
  static char ID;
  RISCVZicfilpSetupCFITypeId() : MachineFunctionPass(ID) {}
  void getAnalysisUsage(AnalysisUsage &AU) const override;
  bool runOnMachineFunction(MachineFunction &MF) override;
  StringRef getPassName() const override { return PASS_NAME; }

private:
  bool setupCFITypeId(MachineBasicBlock &MBB,
                      MachineBasicBlock::instr_iterator MBBI) const;

  const RISCVSubtarget *STI = nullptr;
  const RISCVInstrInfo *TII = nullptr;
};

char RISCVZicfilpSetupCFITypeId::ID = 0;
} // end anonymous namespace

INITIALIZE_PASS(RISCVZicfilpSetupCFITypeId, DEBUG_TYPE, PASS_NAME,
                false, false)

FunctionPass *llvm::createRISCVZicfilpSetupCFITypeIdPass() {
  return new RISCVZicfilpSetupCFITypeId();
}

void RISCVZicfilpSetupCFITypeId::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
  MachineFunctionPass::getAnalysisUsage(AU);
}

bool RISCVZicfilpSetupCFITypeId::setupCFITypeId(
    MachineBasicBlock &MBB,
    MachineBasicBlock::instr_iterator CallIt) const {
  // Since 1) this pass is a pre-RA pass, 2) bundles do not collect
  // regmask operands, 3) call insns have regmask operands, and 4) RA does not
  // look into bundles for register analysis, it should not have bundled calls
  assert(!CallIt->isBundled() && "Cannot setup CFI type ID for a bundled call");

  LLVM_DEBUG(dbgs() << "Setup CFIType: " << CallIt->getCFIType() << '\n');

  BuildMI(MBB, CallIt, MBB.findDebugLoc(CallIt), TII->get(RISCV::LUI))
          .addDef(RISCV::X7)
          .addImm(CallIt->getCFIType());

  // Attach the use of X7 to call explicitly
  MachineInstrBuilder(*MBB.getParent(), CallIt)
                      .addUse(RISCV::X7, RegState::ImplicitKill);

  CallIt->setCFIType(*MBB.getParent(), 0);

  return true;
}

bool RISCVZicfilpSetupCFITypeId::runOnMachineFunction(MachineFunction &MF) {
  const Module *M = MF.getMMI().getModule();
  // Don't need to setup X7 for simple labelling scheme
  if (!M->getModuleFlag("zicfilp-cfi-func-sig"))
    return false;

  LLVM_DEBUG(
      dbgs() << "********** RISCV Setup Zicfilp CFI Call Site Type ID *******\n"
             << "********** Function: " << MF.getName() << '\n');

  STI = static_cast<const RISCVSubtarget *>(&(MF.getSubtarget()));
  TII = static_cast<const RISCVInstrInfo *>(STI->getInstrInfo());

  bool Changed = false;
  for (MachineBasicBlock &MBB : MF) {
    // Use instr_iterator because we don't want to skip bundles.
    for (MachineBasicBlock::instr_iterator MII = MBB.instr_begin(),
                                           MIE = MBB.instr_end();
         MII != MIE; ++MII) {
      if (MII->isCall() && MII->getCFIType())
        Changed |= setupCFITypeId(MBB, MII);
    }
  }

  return Changed;
}
