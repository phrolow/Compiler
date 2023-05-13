#include "compiler.h"
#include "../config.h"

int language_compile(const char *in, const char *out) {
    tree *expression;

    text txt = textFromFile(in);

    expression = ReadExpression(txt.content);

    textDtor(&txt);

    char *buf = (char*) aligned_alloc(BUF_ALIGNMENT, BUFSIZE * sizeof(char));

    memset(buf + HEADER_SIZE + 1, 0xC3, BUFSIZE - HEADER_SIZE - 1);                         // С3 - ret

    Compiler *compiler = newCompiler(buf);              // save labels

    TreeDump(expression);

    generateBinary(expression, compiler);

    // int shit = mprotect(buf, BUFSIZE, PROT_EXEC | PROT_WRITE | PROT_READ);

    // void (* func) (void) = (void (*) (void)) (buf);

    // func();

    FILE *bin = fopen("binary", "wb");

    fwrite(buf, sizeof(char), BUFSIZE, bin);

    fclose(bin);

    TreeDtor(expression);

    FILE *fp = fopen(out, "w");

    fwrite(buf, BUFSIZE, sizeof(char), fp);

    fclose(fp);

    free(buf);

    CompilerDtor(compiler);

    return 0;
}