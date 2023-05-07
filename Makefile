.PHONY: all
all: 
	g++ src/main.cpp src/Tree/tree.cpp src/Reader/reader.cpp src/Tree/debug.cpp src/Compiler/compiler_main.cpp src/Tokens/tokens.cpp src/Tokens/debug.cpp src/Onegin/onegin.cpp src/Compiler/nasm_generate.cpp src/config.h src/TextToTokens/analyzer.cpp src/TextToTokens/analyzer.h src/Tokens/token_stk.cpp src/List/Functions/insertion.cpp src/List/Functions/indexes.cpp src/List/Functions/pointers.cpp src/List/Functions/otheropers.cpp src/List/Struct/list.cpp src/Compiler/compiler.cpp -fsanitize=address -o Compiler

tes:
	@./Compiler Code/fact.txt
	@nasm -f elf64 asm.asm
	@ld -m elf_x86_64 asm.o -o naprimer
