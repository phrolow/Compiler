#include "compiler.h"
#include "../config.h"

int languageCompile(const char *in, const char *out) {
    tree *expression;

    text txt = textFromFile(in);

    expression = ReadExpression(txt.content);

    textDtor(&txt);

    char *buf = (char*) aligned_alloc(BUF_ALIGNMENT, ELF_SIZE * sizeof(char));

    memset(buf, 0, HEADER_SIZE);
    
    memset(buf + HEADER_SIZE, 0xC3, ELF_SIZE - HEADER_SIZE);                         // С3 - ret

    Compiler *compiler = newCompiler(buf);                                          // save labels

    TreeDump(expression);

    generateBinary(expression, compiler);

    FILE *bin = fopen(out, "wb");

    fwrite(buf, sizeof(char), ELF_SIZE, bin);

    fclose(bin);

    TreeDtor(expression);

    free(buf);

    compilerDtor(compiler);

    return 0;
}