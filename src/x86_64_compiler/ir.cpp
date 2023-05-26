#include "ir.h"

void cmdArrayCtor(cmds_t *array, size_t num_cmds, char *ip) {
    array->cmds = (cmd_t*) calloc(num_cmds, sizeof(cmd_t));

    array->ip = ip;
    array->num_cmds = 0;
    array->size_excluded = 0;
}

void cmdArrayDtor(cmds_t *array) {
    array->ip = NULL;
    array->num_cmds = 0;
    array->size_excluded = -1;

    free(array->cmds);
    free(array);
}

void optimize(cmds_t *array) {

}

                        /* CODEGEN MACRO*/

#define DEF_INSTR(num, name, has_arg, len, addr_arg, ...)                           \
    case name:                                                                      \
        size_t sequence_len = len - has_arg * sizeof(array->cmds[i].arg);           \
        strncpy(dest, {__VA_ARGS__}, sequence_len);                                 \
        dest += sequence_len;                                                       \
        if(has_arg) {                                                               \
            *((u_int32_t *) dest) = array->cmds[i].arg;                             \
            dest += (len - sequence_len);                                           \
        }                                                                           \
        break;

                        /* ------------ */

// TODO: optimization for enshorting arg - such as short jmps and arithms

void printArray(cmds_t *array, char *dest) {
    for(size_t i = 0; i < array->num_cmds; i++) {
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

#define DEF_INSTR(num, name, has_arg, len, addr_arg, ...)   \
    case name:                                              \
        cmd->has_addr_arg = (bool) addr_arg;                \
        cmd->length = len;                                  \
        break;

                        /* ------------ */

cmd_t *addCmd(cmds_t *array, cmd_type_t name, int arg) {
    cmd_t *cmd = array->cmds + array->num_cmds;

    cmd->name = name;

    switch(name) {
        #include "cmd_codegen"
        #undef DEF_INSTR
        default:
            exit(1);
            break;
    }

    cmd->arg = arg;
    cmd->included = true;
    cmd->ip = array->ip;

    array->ip += cmd->length;
    array->num_cmds++;

    if(cmd->has_addr_arg) {
        cmd->arg -= cmd->length;                    // proper addressation
    }
}