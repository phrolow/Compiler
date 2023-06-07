#ifndef LANGUAGE_CONFIG_H
#define LANGUAGE_CONFIG_H

#include <stdlib.h>
#include <stdint.h>

// #define DOUBLES

const char * const BINPATH = "binary";

const double EPSILON = 0.001;

const size_t BUF_ALIGNMENT = 0x1000;
const size_t ELF_SIZE = 0x3000;
const size_t DATA_START = 0x800;
const size_t IN_SIZE = 0x61;
const size_t LIB_ALIGNMENT = 0x400; 
const size_t LIBS_SIZE = 0x800;     
const size_t LIBS_BUFS_OFFSET = 0x40;              
const size_t NUM_CMDS = 0x800;                               
const size_t OUT_SIZE = 0x8D;
const size_t TEXT_START = 0x1000;
const size_t X64_VA_START = 0x400000;

const uint64_t ABS_MASK = 0x7fffffffffffffff;

#endif //LANGUAGE_CONFIG_H
