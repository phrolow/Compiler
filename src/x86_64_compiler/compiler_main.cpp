#include "compiler.h"
#include "../config.h"

int language_compile(const char *in, const char *out) {
    tree *expression;

    text txt = textFromFile(in);

    expression = ReadExpression(txt.content);

    textDtor(&txt);

    char *buf = (char*) aligned_alloc(BUF_ALIGNMENT, BUFSIZE * sizeof(char));

    memset(buf, 0, HEADER_SIZE);
    
    memset(buf + HEADER_SIZE, 0xC3, BUFSIZE - HEADER_SIZE);                         // С3 - ret

    Compiler *compiler = newCompiler(buf);              // save labels

    TreeDump(expression);

    generateBinary(expression, compiler);

    FILE *bin = fopen(out, "wb");

    fwrite(buf, sizeof(char), BUFSIZE, bin);

    fclose(bin);

    TreeDtor(expression);

    free(buf);

    CompilerDtor(compiler);

    return 0;
}