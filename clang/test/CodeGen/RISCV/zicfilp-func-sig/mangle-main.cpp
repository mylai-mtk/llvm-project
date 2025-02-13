// REQUIRES: riscv-registered-target
// RUN: %clang_cc1 -DNO_PARAM -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -fcf-protection=branch \
// RUN:  -mcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s
// RUN: %clang_cc1 -DONE_PARAM -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -fcf-protection=branch \
// RUN:  -mcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s
// RUN: %clang_cc1 -DTWO_PARAMS -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -fcf-protection=branch \
// RUN:  -mcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s
// RUN: %clang_cc1 -DTWO_PARAMS -triple riscv64 -target-cpu generic-rv64 \
// RUN:  -target-feature +experimental-zicfilp -fcf-protection=branch \
// RUN:  -mcf-branch-label-scheme=func-sig -emit-llvm -o - -x c++ %s \
// RUN:  | FileCheck %s

// test - main functions should use `int (*)(int, char**)`
// CHECK-LABEL: define{{.*}} @main({{.*}})
// CHECK-SAME: {{.* !riscv_lpad_func_sig}} [[MD:![0-9]+]] {{.*}}{
// CHECK: [[MD]] = !{!"FiiPPcE"}
//

#ifdef NO_PARAM
int main() { return 0; }
#endif

#ifdef ONE_PARAM
int main(int argc) { return argc; }
#endif

#ifdef TWO_PARAMS
int main(int argc, char **argv) { return argc; }
#endif

#ifdef THREE_PARAMS
int main(int argc, char **argv, char **envp) { return argc; }
#endif
