//===------ RISCVIndirectBranchTracking.cpp - Enables lpad mechanism ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The pass adds LPAD (AUIPC with rs1 = X0) machine instructions at the
// beginning of each basic block or function that is referenced by an indrect
// jump/call instruction.
//
//===----------------------------------------------------------------------===//

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "RISCVSubtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Module.h"

using namespace llvm;

cl::opt<uint32_t> PreferredLandingPadLabel(
    "riscv-landing-pad-label", cl::ReallyHidden,
    cl::desc("Use preferred fixed label for all labels"));

namespace {
class RISCVIndirectBranchTrackingPass : public MachineFunctionPass {
public:
  RISCVIndirectBranchTrackingPass() : MachineFunctionPass(ID) {}

  StringRef getPassName() const override {
    return "RISC-V Indirect Branch Tracking";
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  static char ID;
  const Align LpadAlign = Align(4);
};

} // end anonymous namespace

char RISCVIndirectBranchTrackingPass::ID = 0;

FunctionPass *llvm::createRISCVIndirectBranchTrackingPass() {
  return new RISCVIndirectBranchTrackingPass();
}

static void emitLpad(MachineBasicBlock &MBB, const RISCVInstrInfo *TII,
                     uint32_t Label) {
  auto I = MBB.begin();
  MachineInstr &MI =
      *BuildMI(MBB, I, MBB.findDebugLoc(I), TII->get(RISCV::AUIPC), RISCV::X0)
          .addImm(Label);
  MI.getOperand(1).setTargetFlags(RISCVII::MO_LPAD_LABEL);
}

bool RISCVIndirectBranchTrackingPass::runOnMachineFunction(
    MachineFunction &MF) {
  const auto &Subtarget = MF.getSubtarget<RISCVSubtarget>();
  const RISCVInstrInfo *TII = Subtarget.getInstrInfo();

  const Module *const M = MF.getFunction().getParent();
  if (!M)
    return false;
  if (const Metadata *const CFProtectionBranchFlag =
          M->getModuleFlag("cf-protection-branch");
      !CFProtectionBranchFlag ||
      mdconst::extract<ConstantInt>(CFProtectionBranchFlag)->isZero())
    return false;

  StringRef CFBranchLabelScheme;
  if (const Metadata *const MD = M->getModuleFlag("cf-branch-label-scheme"))
    CFBranchLabelScheme = cast<MDString>(MD)->getString();
  else
    report_fatal_error(
        "module should hold its cf-branch-label-scheme=unlabeled|func-sig in "
        "module flag");

  const bool UseFixedLabelForFuncEntry = CFBranchLabelScheme == "unlabeled";
  uint32_t FixedLabel = 0;
  if (UseFixedLabelForFuncEntry &&
      PreferredLandingPadLabel.getNumOccurrences() > 0) {
    if (!isUInt<20>(PreferredLandingPadLabel))
      report_fatal_error("riscv-landing-pad-label=<val>, <val> needs to fit in "
                         "unsigned 20-bits");
    FixedLabel = PreferredLandingPadLabel;
  }

  bool Changed = false;
  for (MachineBasicBlock &MBB : MF) {
    if (&MBB == &MF.front()) {
      Function &F = MF.getFunction();
      // When trap is taken, landing pad is not needed.
      if (F.hasFnAttribute("interrupt"))
        continue;

      if (F.hasAddressTaken() || !F.hasLocalLinkage()) {
        uint32_t Label;
        if (UseFixedLabelForFuncEntry || !F.getMetadata("riscv_lpad_label"))
          Label = FixedLabel;
        else {
          const MDNode &MD = *F.getMetadata("riscv_lpad_label");
          Label =
              mdconst::extract<ConstantInt>(MD.getOperand(0))->getZExtValue();
        }

        emitLpad(MBB, TII, Label);
        if (MF.getAlignment() < LpadAlign)
          MF.setAlignment(LpadAlign);
        Changed = true;
      }
      continue;
    }

    if (MBB.hasAddressTaken()) {
      emitLpad(MBB, TII, FixedLabel);
      if (MBB.getAlignment() < LpadAlign)
        MBB.setAlignment(LpadAlign);
      Changed = true;
    }
  }

  return Changed;
}
