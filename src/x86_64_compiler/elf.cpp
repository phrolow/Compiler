#include "compiler.h"

void generateBinary(tree *tree, Compiler *compiler) {
    generateElfHead(compiler);

    compiler->ip = compiler->out + DATA_START;

    generateMemory(compiler);

    generateLibs(compiler);

    compiler->global_NT = newList();

    generateGS(tree->root, compiler);

    reinitCompiler(compiler, compiler->out);

    compiler->global_NT = newList();

    generateGS(tree->root, compiler);

    printArray(compiler->cmds, compiler->ip);
    cmdArrayDtor(compiler->cmds);
}

void generateElfHead(Compiler *compiler) {
    compiler->header = compiler->ip;

    Elf64_Ehdr *elf_header = (Elf64_Ehdr *) compiler->ip;

    elf_header->e_ident[EI_MAG0] = ELFMAG0;
    elf_header->e_ident[EI_MAG1] = ELFMAG1;
    elf_header->e_ident[EI_MAG2] = ELFMAG2;  
    elf_header->e_ident[EI_MAG3] = ELFMAG3; // Building the elf signature: 0x7f 'E' 'L' 'F'
    
    elf_header->e_ident[EI_CLASS]   = ELFCLASS64;    // 64-bit format
    elf_header->e_ident[EI_DATA]    = ELFDATA2LSB;   // Little Endian
    elf_header->e_ident[EI_VERSION] = EV_CURRENT;    // File version index (must be EV_CURRENT)
    elf_header->e_ident[EI_OSABI]   = ELFOSABI_NONE; // No ABI

    elf_header->e_type      = ET_EXEC;    // Executable file 
    elf_header->e_machine   = EM_X86_64;  // x86-64 machine 
    elf_header->e_version   = EV_CURRENT; // Object file version
    elf_header->e_entry     = X64_VA_START + ENTRY_POINT;   // Entry point
    elf_header->e_phoff     = 64;         // Points to the start of the program header table
    elf_header->e_shoff     = 0x0;        // Points to the start of the section header table
    elf_header->e_flags     = 0x0;        // Interpretation of this field depends on the target architecture
    elf_header->e_ehsize    = 64;         // Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format
    elf_header->e_phentsize = 56;         // Contains the size of a program header table entry
    elf_header->e_phnum     = 4;          // Contains the number of entries in the program header table 

    elf_header->e_shentsize = 64;         // Contains the size of a section header table entry
    elf_header->e_shnum     = 0;          // Contains the number of entries in the section header table
    elf_header->e_shstrndx  = 0;          // Contains index of the section header table entry that contains the section names

    compiler->ip += sizeof(Elf64_Ehdr);

    /* section .data */

    Elf64_Phdr *data_header = (Elf64_Phdr *) compiler->ip;

    data_header->p_type   = PT_LOAD;                    // loadable segment
    data_header->p_flags  = PF_R | PF_W;                // readable and writeable segment

    data_header->p_offset = DATA_START;                 // segment file offset
    data_header->p_vaddr  = X64_VA_START + DATA_START;  // segment VA
    data_header->p_paddr  = 0x0;                        // segment physical address (dont care)
    data_header->p_filesz = TEXT_START - DATA_START;   // segment size in file
    data_header->p_memsz  = TEXT_START - DATA_START;   // segment size in memory

    data_header->p_align  = 0x800;                     // segment alignment

    compiler->ip += sizeof(Elf64_Phdr);

    /* section .text */

    Elf64_Phdr *text_header = (Elf64_Phdr *) compiler->ip;

    text_header->p_type   = PT_LOAD;                    // loadable segment
    text_header->p_flags  = PF_X;                       // executable segment

    text_header->p_offset = TEXT_START;                            // segment file offset
    text_header->p_vaddr  = X64_VA_START + TEXT_START;     // segment VA
    text_header->p_paddr  = 0x0;                            // segment physical address
    text_header->p_filesz = ELF_SIZE - TEXT_START;          // segment size in file
    text_header->p_memsz  = ELF_SIZE - TEXT_START;          // segment size in memory

    text_header->p_align  = 0x800;                          // segment alignment

    compiler->ip += sizeof(Elf64_Phdr);
}

void generateMemory(struct Compiler *compiler) {
    compiler->memory = compiler->ip;

    compiler->ip = compiler->memory + MEMORY_SIZE;                                  
}

void generateLibs      (struct Compiler *compiler) {
    compiler->libs = compiler->ip;

    generateLabel("decimal", POISON, compiler);

    FILE *decimal_out = fopen("src/Libs/decimal", "r");

    fread(compiler->ip, sizeof(char), OUT_SIZE, decimal_out);

    fclose(decimal_out);

    compiler->ip += (OUT_SIZE / LIB_ALIGNMENT + 1) * LIB_ALIGNMENT;

    generateLabel("in", POISON, compiler);

    FILE *in = fopen("src/Libs/in", "r");

    fread(compiler->ip, sizeof(char), IN_SIZE, in);

    fclose(in);

    compiler->ip = compiler->libs + LIBS_SIZE;
}