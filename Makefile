SRC = src/main.cpp src/Tree/tree.cpp src/Reader/reader.cpp src/Tree/debug.cpp src/Tokens/tokens.cpp src/Tokens/debug.cpp src/Onegin/onegin.cpp src/config.h src/TextToTokens/analyzer.cpp src/TextToTokens/analyzer.h src/Tokens/token_stk.cpp src/List/Functions/insertion.cpp src/List/Functions/indexes.cpp src/List/Functions/pointers.cpp src/List/Functions/otheropers.cpp src/List/Struct/list.cpp src/x86_64_compiler/call.cpp src/x86_64_compiler/compiler_main.cpp src/x86_64_compiler/compiler.cpp src/x86_64_compiler/expressions.cpp src/x86_64_compiler/mark.cpp src/x86_64_compiler/nametable_opers.cpp src/x86_64_compiler/x86_64_generate.cpp src/x86_64_compiler/ir.cpp src/x86_64_compiler/elf.cpp

all: 
	@g++ $(SRC) -fsanitize=address -o Compiler
	@nasm -f bin src/Libs/in.asm
	@nasm -f bin src/Libs/decimal.asm

test:
	@g++ -DTEST $(SRC) src/Tests/testing.cpp -fsanitize=address -o Compiler
	@nasm -f bin src/Libs/in.asm
	@nasm -f bin src/Libs/decimal.asm
	@./Compiler
	@ndisasm -b 64 binary > binary.asm

compile:
	@./Compiler Code/fib.txt binary
	@ndisasm -b 64 binary > binary.asm

compile_out:
	@nasm -f bin src/Libs/decimal.asm
	@ndisasm src/Libs/decimal

compile_in:
	@nasm -f bin src/Libs/in.asm
	@ndisasm src/Libs/in