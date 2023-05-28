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
            *((u_int32_t *) dest) = array->cmds[i].arg;                             \
            dest += (len - sequence_len);                                           \
        }                                                                           \
        break;

                        /* ------------ */

#define COMMA ,

// TODO: optimization for enshorting arg - such as short jmps and arithms

void printArray(cmds_t *array, char *dest) {
    for(size_t i = 0; i < array->num_cmds; i++) {
        size_t sequence_len = 0;

        switch (array->cmds[i].name)
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
        cmd->length = len;                                              \
        break;

                        /* ------------ */

void addCmd(cmds_t *array, cmd_type_t name, int arg) {
    size_t cmd_index = array->num_cmds;

    cmd_t *cmd = array->cmds + cmd_index;

    if(cmd_index && (name - array->cmds[cmd_index - 1].name == 0x40)) {        // push pop case
        array->num_cmds--;
        array->ip -= array->cmds[cmd_index - 1].length;

        return;
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

    array->ip += cmd->length;
    array->num_cmds++;

    if(cmd->has_addr_arg) {
        cmd->arg -= cmd->length;                    // proper addressation
    }
}