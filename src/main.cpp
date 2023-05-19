#include "x86_64_compiler/compiler.h"
#include "config.h"

#ifdef TEST

#include <time.h>
#include "Tests/testing.h"

#endif

int main(int argc, char **argv) {
    #ifndef TEST

    if(argc < 2) {
        printf("Too few arguments!\n");
        exit(1);
    }

    language_compile(argv[1], BINPATH);
    printf("Compiled\n");

    #else
    printf("\nTESTING FACT\n");

    language_compile("Code/fact.txt", BINPATH);
    
    runTests("Tests/fact_tests.txt");

    measure();

    printf("TESTING FIB\n");

    language_compile("Code/fib.txt", BINPATH);
    
    runTests("Tests/fib_tests.txt");

    measure();

    #endif

    return 0;
}