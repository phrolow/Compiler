Хей! С вами эксперт Х-курсов по ELF-файлам василий фролов, и сегодня я научу вас писать ELF-файлы

# 0. Что такое ELF-файл?

ELF (Executable and Linkable Format) - формат исполняемого фалйа в Linux. *#достаточно*, во всяком случае для нас сегодня.

# 1. Заголовок файла

Заголовок состоит из туевой хучи заголовков, но сегодня мы рассмотрим заголовок непосредственно файла и заголовок сегмента - их вполне достаточно для написания рабочего ELF-файла.

## Инструменты

Мы будем плотно использовать стандартную библиотеку **<elf.h>**. В ней нам пондобятся константы и структуры, в процессе вы поймёте, что с ними делать.

## Заголовок файла

За заголовок файла отвечает структура **Elf64_Ehdr**. Рассмотрим описание структуры и используемых в ней констант из исходника **elf.h**:

```c
/* 64-bit ELF base types. */
typedef __u64	Elf64_Addr;
typedef __u16	Elf64_Half;
typedef __s16	Elf64_SHalf;
typedef __u64	Elf64_Off;
typedef __s32	Elf64_Sword;
typedef __u32	Elf64_Word;
typedef __u64	Elf64_Xword;
typedef __s64	Elf64_Sxword;

#define EI_NIDENT	16

typedef struct elf64_hdr {
  unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;		/* Entry point virtual address */
  Elf64_Off e_phoff;		/* Program header table file offset */
  Elf64_Off e_shoff;		/* Section header table file offset */
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;
```

<details>
<summary>Как я сделал</summary>
```c
Elf64_Ehdr *elf_header = (Elf64_Ehdr *) compiler->ip;

elf_header->e_ident[EI_MAG0] = ELFMAG0;
elf_header->e_ident[EI_MAG1] = ELFMAG1;
elf_header->e_ident[EI_MAG2] = ELFMAG2;  
elf_header->e_ident[EI_MAG3] = ELFMAG3; // Building the elf signature: 0x7f 'E' 'L' 'F'

elf_header->e_ident[EI_CLASS]   = ELFCLASS64;    // 64-bit format
elf_header->e_ident[EI_DATA]    = ELFDATA2LSB;   // Big Endian
elf_header->e_ident[EI_VERSION] = EV_CURRENT;    // File version index (must be EV_CURRENT)
elf_header->e_ident[EI_OSABI]   = ELFOSABI_NONE;

elf_header->e_type      = ET_EXEC;    // Executable file 
elf_header->e_machine   = EM_X86_64;  // x86-64 machine 
elf_header->e_version   = EV_CURRENT; // Object file version
elf_header->e_entry     = 0x401000;   // Entry point
elf_header->e_phoff     = 64;         // Points to the start of the program header table
elf_header->e_shoff     = 0x0;        // Points to the start of the section header table
elf_header->e_flags     = 0x0;        // Interpretation of this field depends on the target architecture
elf_header->e_ehsize    = 64;         // Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format
elf_header->e_phentsize = 56;         // Contains the size of a program header table entry
elf_header->e_phnum     = 3;          // Contains the number of entries in the program header table 

elf_header->e_shentsize = 64;         // Contains the size of a section header table entry
elf_header->e_shnum     = 0;          // Contains the number of entries in the section header table
elf_header->e_shstrndx  = 0;          // Contains index of the section header table entry that contains the section names```
</details>

Дедушке Линусу было лень пояснять всё, так что сделаем работу за него.

- **e_ident**
```c
    unsigned char	e_ident[EI_NIDENT];```

    <details>
    <summary>Константы</summary>
    ```c
    #define	EI_MAG0		0		/* e_ident[] indexes */
    #define	EI_MAG1		1
    #define	EI_MAG2		2
    #define	EI_MAG3		3
    #define	EI_CLASS	4
    #define	EI_DATA		5
    #define	EI_VERSION	6
    #define	EI_OSABI	7
    #define	EI_PAD		8

    #define	ELFMAG0		0x7f		/* EI_MAG */
    #define	ELFMAG1		'E'
    #define	ELFMAG2		'L'
    #define	ELFMAG3		'F'
    #define	ELFMAG		"\177ELF"
    #define	SELFMAG		4

    #define	ELFCLASSNONE	0		/* EI_CLASS */
    #define	ELFCLASS32	1
    #define	ELFCLASS64	2
    #define	ELFCLASSNUM	3

    #define ELFDATANONE	0		/* e_ident[EI_DATA] */
    #define ELFDATA2LSB	1
    #define ELFDATA2MSB	2

    #define EV_NONE		0		/* e_version, EI_VERSION */
    #define EV_CURRENT	1
    #define EV_NUM		2

    #define ELFOSABI_NONE	0
    #define ELFOSABI_LINUX	3
```
    </details>

    На первые 8 индексов массива определены константы, начинающиеся с **EI_**. Используются только первые 7 индексов массива.

    По индексам **EI_MAG0** - **EI_MAG3** кладем константы **ELFMAG0** - **ELFMAG3** соответственно. Они выступают в роли "подписи" - по их наличию в начале система понимает, что имеет дело с ELF-файлом.

    По индексу **EI_CLASS** лежит одна из констант **ELFCLASSNONE**, **ELFCLASS32**, **ELFCLASS64**, **ELFCLASSNUM**. Выбор прост - мы пишем 64-битное приложение, поэтому записываем **ELFCLASS64**.

    По индексу **EI_DATA** лежит одна из констант **ELFDATANONE**, **ELFDATA2LSB**, **ELFDATA2MSB**. Значение по этому индексу отвечает за порядок байтов, а мы работаем с Little Endian, за который отвечает **ELFDATA2LSB**.

    По индексу **EI_VERSION** лежит версия нашего файла. Тут всё просто, пишем **EV_CURRENT**

    значение с индексом **EI_OSABI** указывает на ABI (двоичный интерфейс приложений), в котором выполнен файл. В общих чертах, ABI - набор соглашений о том, как мы используем регистры, какие у нас системные вызовы, какое у нас соглашение о вызове и т. д. На выбор у нас **ELFOSABI_NONE** и **ELFOSABI_LINUX**. Поскольку я не готов прямо сейчас курить линуксовское ABI, я выбрал **ELFOSABI_NONE**.

    Индекс **EI_PAD** - индекс начала неиспользуемого места в массиве.

-   **e_type**

    <details>
    <summary>Константы</summary>
    ```c
    /* These constants define the different elf file types */
    #define ET_NONE   0
    #define ET_REL    1
    #define ET_EXEC   2
    #define ET_DYN    3
    #define ET_CORE   4
    #define ET_LOPROC 0xff00
    #define ET_HIPROC 0xffff```
    </details>

    Задаёт тип файла. **ET_NONE** нужен для отлова ошибок, **ET_REL** - объектник, **ET_EXEC** - исполняемый, **ET_DYN** - динамическая библиотека, **ET_CORE** - дамп памяти. Да-да, когда тебе кидает сигфолт и пишет Core dumped, все сбрасывается в формат файла ELF. Все типы между **ET_LOPROC** и **ET_HIPROC** зарезервированы под специальные типы конкретного процессора.

    Нетрудно понять, что нас интересует **ET_EXEC**

-   ```c
    Elf64_Half e_machine;```

    Определяет архитектуру процессора, под который мы пишем файл. Среди десятков возможных констант наш выбор - EM_X86_64

-   ```c
    Elf64_Word e_version;```

    Версия объектного файла. Опять же, ставим **EV_CURRENT**.

-   ```c
    Elf64_Addr e_entry;```

    Определяет, какой виртуальный адрес будет у первой исполняемой команды. Для x86 его ставят равным 0x401000.

-   ```c
    Elf64_Off e_phoff;```

    Адрес таблицы заголовков программы. Ставим 64, ибо она идёт сразу после ELF-заголовка, занимающего 64 байта для x64

-   ```c
    elf_header->e_shoff     = 0x0;```

    Адрес таблицы сегментов. Ставим 0, ибо её нет.

-   ```c
    Elf64_Word e_flags;```

    Некоторые флаги, интерпертация которых зависит от процессора. Для х64 всегда зануляем.

-   ```c
    Elf64_Half e_ehsize;```

    Размер ELF-заголовка. 64 для х64.

-   ```c
    Elf64_Half e_phentsize;```

    Размер одного программного заголовка (Elf64_Phdr). В случае х64 это 56.

-   ```c
    Elf64_Half e_phnum;```

    Число программных заголовков. В моём образце он всего один.

-   ```c
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;```

    Аналогично размер и число заголовков секций. Т. к. у меня их нет, я эти поля обнулил.

-   ```c
    Elf64_Half e_shstrndx;```

    Номер заголовка для секции, хранящей таблицу строк. Её у нас нет, пишем константу SHN_UNDEF.

