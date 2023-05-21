#ifndef TESTING_H_INCLUDED
#define TESTING_H_INCLUDED

#include "../config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <chrono>
#include <unistd.h>

const size_t STR_LENGTH = 0x20;

const char * const FACT_PATH = "Code/fact.txt";
const char * const FIB_PATH  = "Code/fib.txt";

const char * const TMP_PATH  = "tmp";

const char * const FACT_TESTS = "Tests/fact_tests.txt";
const char * const FIB_TESTS  = "Tests/fib_tests.txt";

int  runTest (const char *test);
void runTests(const char * tests);
void measure ();

#endif