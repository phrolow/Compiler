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

    compiler->ip += LIBS_BUFS_OFFSET;

    #ifdef DOUBLES

    *((double*) (compiler->out + EPSILON_LOCATION)) = EPSILON;
    generateLabel("epsilon", EPSILON_LOCATION, compiler);
    compiler->ip += sizeof(double);

    *((double*) (compiler->out + MINUS_EPSILON_LOCATION)) = -EPSILON;
    generateLabel("neg_epsilon", MINUS_EPSILON_LOCATION, compiler);
    compiler->ip += sizeof(double);

    *((uint64_t*) (compiler->out + ABS_MASK_LOCATION)) = ABS_MASK;
    *((uint64_t*) (compiler->out + ABS_MASK_LOCATION + sizeof(uint64_t))) = ABS_MASK;
    generateLabel("abs_mask", ABS_MASK_LOCATION, compiler);
    compiler->ip += 2 * sizeof(uint64_t);

    #endif

    generateLabel("consts", CONSTS_LOCATION, compiler);

    compiler->ip = compiler->memory + MEMORY_SIZE;                                  
}

void generateLibs      (struct Compiler *compiler) {
    compiler->libs = compiler->ip;

    generateLabel("decimal", POISON, compiler);

    FILE *out_lib = fopen(OUT_LIB_PATH, "r");

    fread(compiler->ip, sizeof(char), OUT_SIZE, out_lib);

    fclose(out_lib);

    compiler->ip += (OUT_SIZE / LIB_ALIGNMENT + 1) * LIB_ALIGNMENT;

    generateLabel("in", POISON, compiler);

    FILE *in = fopen(IN_LIB_PATH, "r");

    fread(compiler->ip, sizeof(char), IN_SIZE, in);

    fclose(in);

    compiler->ip = compiler->libs + LIBS_SIZE;
}

void generateGS(struct Node *node, struct Compiler *compiler) {
    if (!NODE_KEYW(node, KEYW_STMT)) {
        PRINT_("It's not global statement");

        return;
    }

    compiler->cmds = (cmds_t*) calloc(1, sizeof(cmds_t));
    cmdArrayCtor(compiler->cmds, NUM_CMDS, compiler->ip);

    compiler->instructions = compiler->ip;

    BYTE3(0x48, 0xc7, 0xc3); INT(X64_VA_START + CONSTS_LOCATION + 8 * compiler->num_consts);   // mov rbx, CONSTS_LOCATION
    BYTE1(0xe8); putAddress("main", POISON, compiler);              // call main
    BYTE7(0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00);                // mov rax, 0x3c
    BYTE3(0x48, 0x31, 0xff);                                        // xor rdi, rdi
    BYTE2(0x0f, 0x05);                                              // syscall

    compiler->num_consts = 0;                                       // for 2nd initizalization
    compiler->cmds->ip = compiler->ip;

    while (node->children[RIGHT])
        node = node->children[RIGHT];

    struct Node *bottom = node;

    while (node) {
        if (KEYW(node->children[LEFT]) == KEYW_ASSIGN)
        {
            generateAssign(node, NULL, compiler);
        }

        node = node->parent;
    }

    struct List *NT = newList();

    node = bottom;

    while (node) {
        if (NODE_KEYW(node->children[LEFT], KEYW_DEFINE)) {
            generateFuncDef(node->children[LEFT], NT, compiler);
        } else if (NODE_KEYW(node->children[LEFT], KEYW_ASSIGN)) {
            node = node->parent;
            continue;
        } else {
            PRINT_("Invalid statement");

            ListDtor(NT);

            return;
        }
        node = node->parent;
    }

    generateMain(compiler->node_main, NT, compiler);
    
    printArray(compiler->cmds, compiler->ip);

    ListDtor(NT);
}