#include "paging.h"

void init_paging(){
    //initalize
    /* First 4kB pages */
    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    unsigned int i;
    
    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(i = 0; i < S_TABLE; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes
        paging_4K_table[i].S_bitsAddr = i; // attributes: supervisor level, read/write, present.
        paging_4K_table[i].AVL = 0; 
        paging_4K_table[i].G = 0; 
        paging_4K_table[i].PAT = 0; 
        paging_4K_table[i].D = 0; 
        paging_4K_table[i].A = 0; 
        paging_4K_table[i].PCD = 0; 
        paging_4K_table[i].PWT = 0; 
        paging_4K_table[i].US = 0; 
        paging_4K_table[i].RW = 1; 
        paging_4K_table[i].P = 0; 

        if(i == 0xB8 || i == 0xB9 || i == 0xBA || i == 0xBB){
            paging_4K_table[i].P = 1;
            paging_4K_table[i].RW = 1; 
            paging_4K_table[i].S_bitsAddr = i;
        }

    }
    
    for(i = 0; i < L_DIRECT; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        paging_directory[i].S_bitsAddr = 0;
        paging_directory[i].reserved0 = 0;
        paging_directory[i].M_directory = 0;
        paging_directory[i].AVL = 0;
        paging_directory[i].G = 0;
        paging_directory[i].PS = 1;
        paging_directory[i].D = 0;
        paging_directory[i].A = 0;
        paging_directory[i].PCD = 0;
        paging_directory[i].PWT = 0;
        paging_directory[i].US = 1;
        paging_directory[i].RW = 1;
        paging_directory[i].P = 0;

        if(i == 32){
            paging_directory[i].P = 1;
            paging_directory[i].PS = 1;
        }

    }

    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(i = 0; i < S_TABLE; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes
        video_memory_table[i].S_bitsAddr = 0; // attributes: supervisor level, read/write, present.
        video_memory_table[i].AVL = 0; 
        video_memory_table[i].G = 0; 
        video_memory_table[i].PAT = 0; 
        video_memory_table[i].D = 0; 
        video_memory_table[i].A = 0; 
        video_memory_table[i].PCD = 0; 
        video_memory_table[i].PWT = 0; 
        video_memory_table[i].US = 1; 
        video_memory_table[i].RW = 1; 
        video_memory_table[i].P = 0; 

    }

    paging_directory[0].L_bitsAddr1 = ((uint32_t)paging_4K_table) >> 12;
    paging_directory[0].RW1 = 1;
    paging_directory[0].P1 = 1;
    paging_directory[0].PS1 = 0;
    paging_directory[0].US = 0; 


    paging_directory[1].S_bitsAddr = 1;
    paging_directory[1].RW = 1;
    paging_directory[1].P = 1;
    paging_directory[1].PS = 1;
    paging_directory[1].US = 0; 

    paging_directory[33].L_bitsAddr1 = ((uint32_t)video_memory_table) >> 12;
    paging_directory[33].RW1 = 1;
    paging_directory[33].P1 = 1;
    paging_directory[33].PS1 = 0;

    video_memory_table[0].S_bitsAddr = 0xB8;
    video_memory_table[0].P = 1;

    //load
    loadPageDirectory((unsigned int*)paging_directory);

    //enable
    enablePaging();

}

uint32_t set_program_page(uint32_t proc_id){

    paging_directory[32].S_bitsAddr = proc_id+2;

    flushTLB();

    return 0;
}

/* 
index (virtual mapped to physical) : 
    0 - mapped to 0xB8
    1 - mapped to 0xB9
    2 - mapped to 0xBA
    3 - mapped to 0xBB
*/
uint32_t set_video_page(uint32_t index){
    
    // choose correct page
    switch (index) {
    case 0:
        video_memory_table[0].S_bitsAddr = 0xB8;
        break;
    case 1:
        video_memory_table[0].S_bitsAddr = 0xB9;
        break;
    case 2:
        video_memory_table[0].S_bitsAddr = 0xBA;
        break;
    case 3:
        video_memory_table[0].S_bitsAddr = 0xBB;
        break;
    default:
        return -1;
    }

    video_memory_table[0].P = 1;

    flushTLB();

    return FOUR_MB*33;
}

uint32_t restore_video_page(){

    video_memory_table[0].P = 1;

    flushTLB();

    return 0;
}

/* 
index (virtual mapped to physical) : 
    0 - 0xB8 mapped to 0xB8
    1 - 0xB8 mapped to 0xB9
    2 - 0xB8 mapped to 0xBA
    3 - 0xB8 mapped to 0xBB
*/
uint32_t set_terminal_page(uint32_t index){

    // choose correct page
    switch (index) {
    case 0:
        paging_4K_table[0xB8].S_bitsAddr = 0xB8;
        break;
    case 1:
        paging_4K_table[0xB8].S_bitsAddr = 0xB9;
        break;
    case 2:
        paging_4K_table[0xB8].S_bitsAddr = 0xBA;
        break;
    case 3:
        paging_4K_table[0xB8].S_bitsAddr = 0xBB;
        break;
    default:
        return -1;
    }

    flushTLB();

    return 0;

}
