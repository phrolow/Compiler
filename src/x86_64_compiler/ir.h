#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_INSTR(num, name, has_arg, len, addr_arg, byte_sequence) name = num,

typedef enum CommandEnum {
    #include "cmd_codegen"
    #undef DEF_INSTR
} cmd_type_t;

typedef struct CommandStruct {
    int name;

    size_t length;
    char* ip;    
    int arg;

    bool has_addr_arg;
    bool is_short;
} cmd_t;

typedef struct CommandsArray {
    cmd_t *cmds;
    size_t num_cmds;
    char* ip;
} cmds_t;

const size_t SHORT_DIFF = 0x80;
const size_t PUSH_POP_DIFF = 0x40;
const size_t INT_SCHAR_SIZE_DIFF = sizeof(int) - sizeof(char);

void cmdArrayCtor(cmds_t *array, size_t num_cmds, char *ip);

void cmdArrayDtor(cmds_t *array);

void printArray(cmds_t *array, char *dest);

void addCmd(cmds_t *array, cmd_type_t name, int arg);

#endif