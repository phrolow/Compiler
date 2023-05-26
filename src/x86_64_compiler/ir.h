#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_INSTR(num, name, has_arg, len, addr_arg, byte_sequence) name = num,

typedef enum CommandEnum {
    #include "cmd_codegen"
    #undef DEF_INSTR
} cmd_type_t;

typedef struct CommandStruct {
    cmd_type_t name;

    size_t length;
    char* ip;    
    int arg;

    bool included;
    bool has_addr_arg;
} cmd_t;

typedef struct CommandsArray {
    cmd_t *cmds;
    size_t num_cmds;
    char* ip;
    size_t size_excluded;
} cmds_t;

void cmdArrayCtor(cmds_t *array, size_t num_cmds, char *ip);

void cmdArrayDtor(cmds_t *array);

void optimize(cmds_t *array);

void printArray(cmds_t *array, char *dest);

void addCmd(cmds_t *array, cmd_type_t name, int arg);

#endif