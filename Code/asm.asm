CALL :main
HLT
:main
PUSH rbx
PUSH 1
ADD
POP rbx
IN
POP [rbx+0]
IN
POP [rbx+1]
IN
POP [rbx+2]
PUSH [rbx+0]
PUSH 0
JNE :END_IF_0
PUSH [rbx+1]
PUSH 0
JNE :END_IF_1
PUSH [rbx+2]
PUSH 0
JNE :END_IF_2
PUSH rbx
PUSH 13744632839234567870
SUB
POP rbx
RET
:END_IF_2
PUSH 0
OUT
PUSH rbx
PUSH 13744632839234567870
SUB
POP rbx
RET
:END_IF_1
PUSH [rbx+2]
PUSH [rbx+1]
DIV
OUT
PUSH rbx
PUSH 13744632839234567870
SUB
POP rbx
RET
:END_IF_0
PUSH [rbx+1]
PUSH [rbx+1]
MUL
PUSH 4
PUSH [rbx+0]
MUL
PUSH [rbx+2]
MUL
SUB
POP [rbx+3]
PUSH [rbx+3]
PUSH 0
JBE :END_IF_3
PUSH rbx
PUSH 13744632839234567870
SUB
POP rbx
RET
:END_IF_3
PUSH [rbx+3]
PUSH 0
JNE :END_IF_4
PUSH 0
PUSH [rbx+1]
SUB
PUSH 2
PUSH [rbx+0]
MUL
DIV
OUT
PUSH rbx
PUSH 13744632839234567870
SUB
POP rbx
RET
:END_IF_4
PUSH [rbx+3]
PUSH [rbx+3]
PUSH 10000
POP rax
ASQRT
PUSH 10000
DIV
POP [rbx+3]
PUSH [rbx+3]
PUSH [rbx+1]
SUB
PUSH 2
PUSH [rbx+0]
MUL
DIV
OUT
PUSH 0
PUSH [rbx+3]
SUB
PUSH [rbx+1]
SUB
PUSH 2
PUSH [rbx+0]
MUL
DIV
OUT
PUSH rbx
PUSH 13744632839234567870
SUB
POP rbx
RET