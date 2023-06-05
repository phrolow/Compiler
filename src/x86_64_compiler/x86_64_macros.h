#ifndef X86_64_H_PROTECTED
#define X86_64_H_PROTECTED

#define BYTE(c) *(compiler->ip) = (unsigned char) c; \
                compiler->ip++;

#define BYTE1(c1)                           BYTE(c1);
#define BYTE2(c1, c2)                       BYTE(c1); BYTE(c2);
#define BYTE3(c1, c2, c3)                   BYTE(c1); BYTE(c2); BYTE(c3);
#define BYTE4(c1, c2, c3, c4)               BYTE(c1); BYTE(c2); BYTE(c3); BYTE(c4);
#define BYTE5(c1, c2, c3, c4, c5)           BYTE(c1); BYTE(c2); BYTE(c3); BYTE(c4); BYTE(c5);
#define BYTE6(c1, c2, c3, c4, c5, c6)       BYTE(c1); BYTE(c2); BYTE(c3); BYTE(c4); BYTE(c5); BYTE(c6);
#define BYTE7(c1, c2, c3, c4, c5, c6, c7)   BYTE(c1); BYTE(c2); BYTE(c3); BYTE(c4); BYTE(c5); BYTE(c6); BYTE(c7);

#define INT(val) *((int *) (compiler->ip)) = (int) (val); compiler->ip += sizeof(int);

#endif