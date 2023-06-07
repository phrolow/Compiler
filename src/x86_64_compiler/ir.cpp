#include "ir.h"

void cmdArrayCtor(cmds_t *array, size_t num_cmds, char *ip) {
    array->cmds = (cmd_t*) calloc(num_cmds, sizeof(cmd_t));

    array->ip = ip;
    array->num_cmds = 0;
}

void cmdArrayDtor(cmds_t *array) {
    array->ip = NULL;
    array->num_cmds = 0;

    free(array->cmds);
    free(array);
}

                        /* CODEGEN MACRO*/

#define DEF_INSTR(num, name, has_arg, len, addr_arg, byte_sequence)                 \
    case name:                                                                      \
        sequence_len = strlen(byte_sequence);                                       \
        strcpy(dest, byte_sequence);                                                \
        dest += sequence_len;                                                       \
        if(has_arg) {                                                               \
            if(cmd->is_short) {                                                     \
                *dest = (signed char) cmd->arg;                                     \
            }                                                                       \
            else {                                                                  \
                *((u_int32_t *) dest) = cmd->arg;                                   \
            }                                                                       \
            dest += (len - sequence_len);                                           \
        }                                                                           \
        break;

                        /* ------------ */

#define COMMA ,

// TODO: optimization for enshorting arg - such as short jmps and arithms

void printArray(cmds_t *array, char *dest) {
    for(size_t i = 0; i < array->num_cmds; i++) {
        size_t sequence_len = 0;
        cmd_t *cmd = array->cmds + i;

        switch (cmd->name)
        {
        #include "cmd_codegen"
        #undef DEF_INSTR
        default:
            exit(1);
            break;
        }
    }
}

                        /* CODEGEN MACRO*/

#define DEF_INSTR(num, name, has_arg, len, addr_arg, byte_sequence)     \
    case name:                                                          \
        cmd->has_addr_arg = (bool) addr_arg;                            \
        if(addr_arg) {                                                  \
            arg -= len;                                                 \
        }                                                               \
        cmd->length = len;                                              \
        break;

                        /* ------------ */

void addInstruction(cmds_t *array, int name, int arg) {
    size_t cmd_index = array->num_cmds;

    cmd_t *cmd = array->cmds + cmd_index;

    #ifdef DOUBLES

    if(cmd_index && (name - array->cmds[cmd_index - 1].name == PUSH_POP_DIFF)) {        // push pop case
        array->num_cmds-=2;
        array->ip -= (array->cmds[cmd_index - 1].length + array->cmds[cmd_index - 2].length);

        return;
    }

    #else

    if(cmd_index && (name - array->cmds[cmd_index - 1].name == PUSH_POP_DIFF)) {        // push pop case
        array->num_cmds--;
        array->ip -= array->cmds[cmd_index - 1].length;

        return;
    }

    #endif

    if(cmd->arg < (int) (SCHAR_MAX - INT_SCHAR_SIZE_DIFF) 
        && arg > (int) (SCHAR_MIN + INT_SCHAR_SIZE_DIFF)
        && name != MOV_RAX_IMM 
        && name != CALL) 
    {
            cmd->name += SHORT_DIFF;
            cmd->is_short = true;

    }

    switch(name) {
        #include "cmd_codegen"
        #undef DEF_INSTR
        default:
            exit(1);
            break;
    }

    cmd->name = name;
    cmd->arg = arg;
    cmd->ip = array->ip;

    cmd->is_short = false;

    array->ip += cmd->length;
    array->num_cmds++;
}