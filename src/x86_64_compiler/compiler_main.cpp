#include "compiler.h"
#include "../config.h"

int language_compile(const char *in, const char *out) {
    tree *expression;

    text txt = textFromFile(in);

    expression = ReadExpression(txt.content);

    textDtor(&txt);

    char *buf = (char*) calloc(BUFSIZE, sizeof(char));

    memset(buf, 0xC3, BUFSIZE);                         // С3 - ret
    buf[0] = 0xCC;                                      // debug (int 03)

    Compiler *compiler = newCompiler(buf + 1);

    TreeDump(expression);

    generateBinary(expression, compiler);

    mprotect(buf, BUFSIZE, PROT_EXEC);

    void (*func)() = (void (*)()) buf;

    (*func)();

    TreeDtor(expression);

    FILE *fp = fopen(out, "w");

    fwrite(buf, BUFSIZE, sizeof(char), fp);

    fclose(fp);

    free(buf);

    CompilerDtor(compiler);

    return 0;
}