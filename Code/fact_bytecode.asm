CALL :main
HLT
:fact
PUSH rbx
PUSH 1
ADD
POP rbx
PUSH [rbx+0]
PUSH 1
JNE :END_IF_0
PUSH 1
POP rcx
PUSH rbx
PUSH 1
SUB
POP rbx
RET
:END_IF_0
PUSH [rbx+0]
PUSH [rbx+0]
PUSH 1
SUB
POP [rbx+1]
CALL :fact
PUSH rcx
MUL
POP rcx
PUSH rbx
PUSH 1
SUB
POP rbx
RET
:main
PUSH rbx
PUSH 1
ADD
POP rbx
PUSH 5
POP [rbx+0]
PUSH [rbx+0]
POP [rbx+1]
CALL :fact
PUSH rcx
POP [rbx+1]
PUSH rbx
PUSH 1
SUB
POP rbx
RET
