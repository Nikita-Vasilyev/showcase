#ifndef _PAGE_H
#define _PAGE_H

#define S_DIRECT 1024
#define S_TABLE 1024
#define S_PAGE 4096

#define L_DIRECT 1024
#define L_PAGE 4194304

#define FOUR_MB (4<<20)

#define USER_PROG_START 128

#define VIDEO_MEMORY 0xB8000
#define TERM1_PAGE 0xB9000
#define TERM2_PAGE 0xBA000
#define TERM3_PAGE 0xBB000

#ifndef ASM

#include "lib.h"

#define PAGE_OFFSET 0x48000

void init_paging();

extern void loadPageDirectory(unsigned int*);

extern void enablePaging();

extern void flushTLB();

uint32_t set_program_page(uint32_t proc_id);

uint32_t set_video_page(uint32_t index);

uint32_t restore_video_page();

uint32_t set_terminal_page(uint32_t index);

typedef struct page_table_entry{
    union{ 
    uint32_t val;
    struct {
        uint32_t P               : 1;
        uint32_t RW              : 1;
        uint32_t US              : 1;
        uint32_t PWT             : 1;
        uint32_t PCD             : 1;
        uint32_t A               : 1;
        uint32_t D               : 1;
        uint32_t PAT             : 1;
        uint32_t G               : 1;
        uint32_t AVL             : 3;
        uint32_t S_bitsAddr      : 20;
    } __attribute__ ((packed));
    };
} page_table_entry;

typedef struct page_dir_entry{
    union { 
    uint32_t val;
    
    struct {
        uint32_t P1         : 1;
        uint32_t RW1        : 1;
        uint32_t US1        : 1;
        uint32_t PWT1       : 1;
        uint32_t PCD1       : 1;
        uint32_t A1         : 1;
        uint32_t AVL_21       : 1; //AVL again??
        uint32_t PS1        : 1;
        uint32_t AVL1       : 4;
        uint32_t L_bitsAddr1  : 20;
    } __attribute__ ((packed));
    struct {
        uint32_t P         : 1;
        uint32_t RW        : 1;
        uint32_t US        : 1;
        uint32_t PWT       : 1;
        uint32_t PCD       : 1;
        uint32_t A         : 1;
        uint32_t D         : 1;
        uint32_t PS        : 1;
        uint32_t G         : 1;
        uint32_t AVL       : 3;
        uint32_t PAT       : 1;
        uint32_t M_directory : 8;
        uint32_t reserved0    : 1;
        uint32_t S_bitsAddr   : 10;
    } __attribute__ ((packed));
    };
}page_dir_entry;

page_table_entry paging_4K_table[S_DIRECT] __attribute__((aligned(S_PAGE)));

page_dir_entry paging_directory[L_DIRECT] __attribute__((aligned(S_PAGE)));

page_table_entry video_memory_table[S_DIRECT] __attribute__((aligned(S_PAGE)));

#endif /* ASM */

#endif /* _PAGE_H */
