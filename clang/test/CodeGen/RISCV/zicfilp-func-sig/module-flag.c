// REQUIRES: riscv-registered-target
// RUN: %clang -march=rv64gczicfilp0p4 -menable-experimental-extensions \
// RUN:   -mzicfilp-label-scheme=simple -S -emit-llvm -o - %s \
// RUN:   | FileCheck --check-prefix=SIMPLE %s

// RUN: %clang -march=rv64gczicfilp0p4 -menable-experimental-extensions \
// RUN:   -mzicfilp-label-scheme=func_sig -S -emit-llvm -o - %s \
// RUN:   | FileCheck --check-prefix=FUNC-SIG %s

// SIMPLE-DAG: [[FLAG:![0-9]+]] = !{i32 4, !"zicfilp-cfi-simple", i32 1}
// SIMPLE-DAG: !llvm.module.flags = !{{[{].*}}[[FLAG]]{{(,.+)?[}]}}
//
// FUNC-SIG-DAG: [[FLAG:![0-9]+]] = !{i32 4, !"zicfilp-cfi-func-sig", i32 1}
// FUNC-SIG-DAG: !llvm.module.flags = !{{[{].*}}[[FLAG]]{{(,.+)?[}]}}
//
int main() { return 0; }
