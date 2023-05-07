#include "Compiler/compiler.h"
#include "config.h"

#include <unistd.h>

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Too few arguments!\n");
        exit(1);
    }

    language_compile(argv[1], ASM_PATH);
    printf("Compiled\n");
    system("Y:/gthb/Language/Exes/asm.exe ../asm.asm ../bin1.bin");
    system("Y:/gthb/Language/Exes/cpu.exe ../bin1.bin");

    return 0;
}