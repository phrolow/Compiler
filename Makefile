.PHONY: all
all: 
	@g++ src/main.cpp src/Tree/tree.cpp src/Reader/reader.cpp src/Tree/debug.cpp src/Tokens/tokens.cpp src/Tokens/debug.cpp src/Onegin/onegin.cpp src/config.h src/TextToTokens/analyzer.cpp src/TextToTokens/analyzer.h src/Tokens/token_stk.cpp src/List/Functions/insertion.cpp src/List/Functions/indexes.cpp src/List/Functions/pointers.cpp src/List/Functions/otheropers.cpp src/List/Struct/list.cpp src/x86_64_compiler/call.cpp src/x86_64_compiler/compiler_main.cpp src/x86_64_compiler/compiler.cpp src/x86_64_compiler/elf_generate.cpp src/x86_64_compiler/expressions.cpp src/x86_64_compiler/mark.cpp src/x86_64_compiler/nametable_opers.cpp src/x86_64_compiler/x86_64_generate.cpp -fsanitize=address -o Compiler
	@nasm -f bin src/Libs/in.asm
	@nasm -f bin src/Libs/decimal.asm

test:
	@g++ -DTEST src/main.cpp src/Tests/testing.cpp src/Tree/tree.cpp src/Reader/reader.cpp src/Tree/debug.cpp src/Tokens/tokens.cpp src/Tokens/debug.cpp src/Onegin/onegin.cpp src/config.h src/TextToTokens/analyzer.cpp src/TextToTokens/analyzer.h src/Tokens/token_stk.cpp src/List/Functions/insertion.cpp src/List/Functions/indexes.cpp src/List/Functions/pointers.cpp src/List/Functions/otheropers.cpp src/List/Struct/list.cpp src/x86_64_compiler/call.cpp src/x86_64_compiler/compiler_main.cpp src/x86_64_compiler/compiler.cpp src/x86_64_compiler/elf_generate.cpp src/x86_64_compiler/expressions.cpp src/x86_64_compiler/mark.cpp src/x86_64_compiler/nametable_opers.cpp src/x86_64_compiler/x86_64_generate.cpp -fsanitize=address -o Compiler
	@nasm -f bin src/Libs/in.asm
	@nasm -f bin src/Libs/decimal.asm
	@./Compiler

compile:
	@./Compiler Code/fib.txt
	@ndisasm -b 64 binary > binary.asm

compile_out:
	@nasm -f bin src/Libs/decimal.asm
	@ndisasm src/Libs/decimal

compile_in:
	@nasm -f bin src/Libs/in.asm
	@ndisasm src/Libs/in

bytecode:
	@g++ src/main.cpp src/Tree/tree.cpp src/Reader/reader.cpp src/Tree/debug.cpp src/Compiler/compiler_main.cpp src/Tokens/tokens.cpp src/Tokens/debug.cpp src/Onegin/onegin.cpp src/Compiler/asm_generate.cpp src/config.h src/TextToTokens/analyzer.cpp src/TextToTokens/analyzer.h src/Tokens/token_stk.cpp src/List/Functions/insertion.cpp src/List/Functions/indexes.cpp src/List/Functions/pointers.cpp src/List/Functions/otheropers.cpp src/List/Struct/list.cpp src/Compiler/compiler.cpp -fsanitize=address -o Compiler