#include "compiler.h"

void CreateKeyword(token_t **token, KEYW keyword) {
    assert(token && *token);

    if(!token) {
        PRINT_("Null token pointer");

        return;
    }

    if(!(*token)) {
        PRINT_("Null token");

        return;
    }

    (*token)->type = KEYWORD_TYPE;
    (*token)->value.keyword = keyword;
}

int IsNum(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return (node->val->type == NUM_TYPE);
}

int IsVar(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return (node->val->type == VAR_TYPE && node->parent && KEYW(node->parent) != KEYW_DEFINE);
}

int IsMathOper(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_ADD) && (KEYW(node) <= KEYW_POW));
}

int IsLogOper(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_EQUAL) && (KEYW(node) <= KEYW_OR));
}

void IncreaseRBX(const size_t number, struct Compiler *compiler) {
    // BYTE4(0x48, 0x83, 0xc3, number * 8); // add rbx, number * 8

    addCmd(compiler->cmds, ADD_RBX, number * 8);
}

void DecreaseRBX(const size_t number, struct Compiler *compiler) {
    // BYTE4(0x48, 0x83, 0xeb, number * 8); // sub rbx, number * 8

    addCmd(compiler->cmds, SUB_RBX, number * 8);
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
    elf_header->e_ident[EI_OSABI]   = ELFOSABI_NONE;

    elf_header->e_type      = ET_EXEC;    // Executable file 
    elf_header->e_machine   = EM_X86_64;  // x86-64 machine 
    elf_header->e_version   = EV_CURRENT; // Object file version
    elf_header->e_entry     = X64_VA_START + ENTRY_POINT;   // Entry point
    elf_header->e_phoff     = 64;         // Points to the start of the program header table
    elf_header->e_shoff     = 0x0;        // Points to the start of the section header table
    elf_header->e_flags     = 0x0;        // Interpretation of this field depends on the target architecture
    elf_header->e_ehsize    = 64;         // Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format
    elf_header->e_phentsize = 56;         // Contains the size of a program header table entry
    elf_header->e_phnum     = 3;          // Contains the number of entries in the program header table 

    elf_header->e_shentsize = 64;         // Contains the size of a section header table entry
    elf_header->e_shnum     = 0;          // Contains the number of entries in the section header table
    elf_header->e_shstrndx  = 0;          // Contains index of the section header table entry that contains the section names

    compiler->ip += sizeof(Elf64_Ehdr);

    Elf64_Phdr *code_header = (Elf64_Phdr *) compiler->ip;

    code_header->p_type   = PT_LOAD;                    // loadable segment
    code_header->p_flags  = PF_R | PF_W | PF_X;         // readable, writeable and executable segment

    code_header->p_offset = 0x0;             // segment file offset
    code_header->p_vaddr  = X64_VA_START;    // segment VA
    code_header->p_paddr  = 0x0;             // segment physical address
    code_header->p_filesz = BUFSIZE;         // segment size in file
    code_header->p_memsz  = BUFSIZE;         // segment size in memory

    code_header->p_align  = 0x1000;          // segment alignment

    compiler->ip += sizeof(Elf64_Phdr);
}

void generateMemory(struct Compiler *compiler) {
    compiler->memory = compiler->ip;

    BYTE7(0x48, 0x8d, 0x1d, 0x00, 0x00, 0x00, 0x00);                                                // lea rbx, [rip]
    BYTE1(0xe9); *((u_int32_t *) (compiler->ip)) = MEMORY_SIZE + LIBS_SIZE - 12;                    // skip data and libs space (with jmp) 
                                                                                                    // 12 is num of already printed bytes
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

void generateBinary(tree *tree, Compiler *compiler) {
    generateElfHead(compiler);

    compiler->ip = compiler->out + ENTRY_POINT;

    compiler->GlobalNT = newList();

    GenerateGS(tree->root, compiler);

    ListDtor(compiler->GlobalNT);

    reinitCompiler(compiler, compiler->out);

    compiler->GlobalNT = newList();

    GenerateGS(tree->root, compiler);
}

void GenerateGS(struct Node *node, struct Compiler *compiler) {
    if (!NODE_KEYW(node, KEYW_STMT)) {
        PRINT_("It's not global statement");

        return;
    }

    compiler->cmds = (cmds_t*) calloc(1, sizeof(cmds_t));
    cmdArrayCtor(compiler->cmds, NUM_CMDS, compiler->ip);

    generateMemory(compiler);

    generateLibs(compiler);

    compiler->instructions = compiler->ip;

    BYTE1(0xe8); putAddress("main", POISON, compiler);  // call main
    BYTE7(0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00);    // mov rax, 0x3c
    BYTE3(0x48, 0x31, 0xff);                            // xor rdi, rdi
    BYTE2(0x0f, 0x05);                                  // syscall

    compiler->cmds->ip = compiler->ip;

    while (node->children[RIGHT])
        node = node->children[RIGHT];

    struct Node *bottom = node;

    while (node) {
        if (KEYW(node->children[LEFT]) == KEYW_ASSIGN)
        {
            GenerateAssign(node, NULL, compiler);
        }

        node = node->parent;
    }

    struct List *NT = newList();

    node = bottom;

    while (node) {
        if (NODE_KEYW(node->children[LEFT], KEYW_DEFINE)) {
            GenerateFuncDef(node->children[LEFT], NT, compiler);
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

    GenerateMain(compiler->node_main, NT, compiler);
    
    printArray(compiler->cmds, compiler->ip);

    cmdArrayDtor(compiler->cmds);
    ListDtor(NT);
}

void InitGlobVar(struct Node *node, struct Compiler *compiler) {
    if (SearchInNametable(node->children[LEFT], compiler->GlobalNT))
        PRINT_("Repeating definition");

    PushInNametable(node->children[LEFT], compiler->GlobalNT);

    size_t index = IndexNametable(node->children[LEFT], compiler->GlobalNT);

    assert(index >= 1);

    GenerateGlobExpr(node->children[RIGHT], compiler);

    // BYTE2(0x41, 0x5c);                          // pop r12
    // BYTE4(0x4c, 0x89, 0x63, (index - 1) * 8);   // mov [rbx + (index - 1) * 8], r12 
    //                                             // risk if there're 17 or more global vars

    addCmd(compiler->cmds, POP_R12, POISON);
    addCmd(compiler->cmds, MOV_MEM_R12, (index - 1) * 8);
}

void InitVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if(SearchInNametable(node->children[LEFT], compiler->GlobalNT)) 
        PRINT_("Repeating variable");

    if (SearchInNametable(node->children[LEFT], NT))
    {
        GenerateExpr(node->children[RIGHT], NT, compiler);

        size_t index = IndexNametable(node->children[LEFT], NT);

        assert(index >= 1);

        // BYTE2(0x41, 0x5c);                          // pop r12
        // BYTE4(0x4c, 0x89, 0x63, (index - 1) * 8);   // mov [rbx + (index - 1) * 8], r12 
        //                                             // risk if there're 17 or more global vars

        addCmd(compiler->cmds, POP_R12, POISON);
        addCmd(compiler->cmds, MOV_MEM_R12, (index - 1) * 8);

        return;
    }

    PushInNametable(node->children[LEFT], NT);

    size_t index = IndexNametable(node->children[LEFT], NT);

    assert(index >= 1);

    GenerateExpr(node->children[RIGHT], NT, compiler);

    // BYTE2(0x41, 0x5c);                          // pop r12
    // BYTE4(0x4c, 0x89, 0x63, (index - 1) * 8);   // mov [rbx + (index - 1) * 8], r12 
    //                                             // risk if there're 17 or more global vars

    addCmd(compiler->cmds, POP_R12, POISON);
    addCmd(compiler->cmds, MOV_MEM_R12, (index - 1) * 8);
}