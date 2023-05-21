#include "testing.h"

#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wnonnull"

int runTest(const char *test) {
    char cmd[STR_LENGTH + 1] = {};

    sprintf(cmd, "./%s < %s", BINPATH, test);

    FILE *elf = popen(cmd, "r");

    int result = 0;

    fscanf(elf, "%d", &result);
    pclose(elf);

    return result;
}

void runTests(const char *tests) {
    FILE *tests_file = fopen(tests, "r");

    char line[STR_LENGTH + 1] = {};

    int test_in = -1;
    int test_out = -1;

    int counter = 1;

    while(fgets(line, STR_LENGTH + 1, tests_file)) {
        sscanf(line, "%d %d", &test_in, &test_out);

        FILE *tmp = fopen(TMP_PATH, "w");

        fprintf(tmp, "%d\n", test_in);

        fclose(tmp);

        int real_out = runTest(TMP_PATH);

        if(real_out == test_out) {
            printf("#%d: SUCCESS\n", counter++);
        }
        else {
            printf("#%d: FAILED, got %d instead of %d\n", counter++, real_out, test_out);
        }
    }

    fclose(tests_file);
}

void measure() {
    int status = 0;

    char cmd[STR_LENGTH + 1] = {};

    sprintf(cmd, "./%s < %s", BINPATH, TMP_PATH);

    auto begin = std::chrono::high_resolution_clock::now();

    FILE *elf = popen(cmd, "r");
    pclose(elf);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    printf("Working time: %ld ms\n", elapsed);
}