#include "compiler.h"
#include "../config.h"

int language_compile(const char *in, const char *out) {
    tree *expression;

    text txt = textFromFile(in);

    expression = ReadExpression(txt.content);

    textDtor(&txt);

    char *buf = (char*) calloc(BUFSIZE, sizeof(char));

    memset(buf, 0xC3, BUFSIZE);                         // С3 - ret

    Compiler *compiler = newCompiler(buf);

    TreeDump(expression);

    generateBinary(expression, compiler);

    TreeDtor(expression);

    FILE *fp = fopen(out, "w");

    fwrite(buf, BUFSIZE, sizeof(char), fp);

    fclose(fp);

    free(buf);

    CompilerDtor(compiler);

    return 0;
}