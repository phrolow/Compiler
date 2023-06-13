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

    char *out = argv[2];

    if(!out) {
        int len = strchr(argv[1], '.') - argv[1];

        out = (char *) calloc(len, sizeof(char));
    }

    languageCompile(argv[1], argv[2]);
    printf("Compiled\n");

    if(!argv[2]) {
        free(out);
    }

    #else

    printf("\nTESTING FIB\n");

    languageCompile("Code/fib.txt", BINPATH);
    
    runTests("Tests/fib_tests.txt");

    measure();

    printf("TESTING FACT\n");

    languageCompile("Code/fact.txt", BINPATH);
    
    runTests("Tests/fact_tests.txt");

    measure();
    #endif

    return 0;
}