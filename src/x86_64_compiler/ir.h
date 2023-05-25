#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

#include <stdlib.h>

#define DEF_INSTR(name, has_arg, len, addr_arg, ...) name,

typedef enum CommandEnum {
    #include "cmd_codegen"
    #undef DEF_INSTR
} cmd_type_t;

typedef struct CommandStruct {
    cmd_type_t name;

    size_t length;
    size_t ip;    
    int arg;

    bool included;
    bool has_addr_arg;
} cmd_t;

typedef struct CommandsArray {
    cmd_t *cmds;
    size_t num_cmds;
    size_t ip;
    size_t size_excluded;
} cmds_t;

void cmdArrayCtor(cmds_t *array, size_t num_cmds);

void cmdArrayDtor(cmds_t *array);

void optimize(cmds_t *array);

void printArray(cmds_t *array, char *dest);

cmd_t *addCmd(cmds_t *array, cmd_type_t name, size_t length, bool has_addr_arg);

#endif