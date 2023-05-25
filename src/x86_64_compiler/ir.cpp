#include "ir.h"

void cmdArrayCtor(cmds_t *array, size_t num_cmds) {
    array->cmds = (cmd_t*) calloc(num_cmds, sizeof(cmd_t));

    array->ip = 0;
    array->num_cmds = 0;
    array->size_excluded = 0;
}

void cmdArrayDtor(cmds_t *array) {
    array->ip = 0;
    array->num_cmds = 0;
    array->size_excluded = -1;

    free(array->cmds);
    free(array);
}

void optimize(cmds_t *array) {

}

void printArray(cmds_t *array, char *dest) {

}

cmd_t *addCmd(cmds_t *array, cmd_type_t name, size_t length, int arg, bool has_addr_arg) {
    cmd_t *cmd = array->cmds + array->num_cmds;

    cmd->arg = arg;
    cmd->has_addr_arg = has_addr_arg;
    cmd->included = true;
    cmd->ip = array->ip;
    cmd->length = length;
    cmd->name = name;

    array->ip += length;
    array->num_cmds++;
}