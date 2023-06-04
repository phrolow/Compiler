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

    languageCompile(argv[1], argv[2]);
    printf("Compiled\n");

    #else
    printf("TESTING FACT\n");

    languageCompile("Code/fact.txt", BINPATH);
    
    runTests("Tests/fact_tests.txt");

    measure();

    printf("\nTESTING FIB\n");

    languageCompile("Code/fib.txt", BINPATH);
    
    runTests("Tests/fib_tests.txt");

    measure();

    #endif

    return 0;
}