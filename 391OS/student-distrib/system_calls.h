#ifndef SYS_H
#define SYS_H

#define NUM_ENTRIES 8
#define COMMAND_LEN 32
#define PROGRAM_START 0x08048000
#define MAX_PCB 6
#define NUM_TERM 3
#define MAGIC 28

#ifndef ASM

#include "lib.h"
#include "types.h"
#include "file_system.h"
#include "terminal.h"
#include "rtc.h"
#include "pit.h"

// struct for fd function pointers
typedef struct file_ops_t{
    int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write) (int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*open) (const uint8_t* filename);
    int32_t (*close) (int32_t fd);
} file_ops_t;

// fd array entry struct
typedef struct file_desc_entry_t{
    file_ops_t* entries;
    inode_t* inode;
    uint32_t file_pos;
    uint32_t flags;
} file_desc_entry_t;

// pcb struct
typedef struct pcb_t{
    uint32_t PID;
    uint32_t exc_ESP;
    uint32_t exc_EBP;
    uint32_t last_ESP;
    uint32_t last_EBP;
    int8_t status;
    uint32_t EBP;
    uint32_t EIP;
    uint32_t parent_PID;
    file_desc_entry_t file_descriptor_table[NUM_ENTRIES];
    uint8_t com[COMMAND_LEN];
    uint8_t args[COMMAND_LEN]; //added this for get args
    uint8_t vidmap_flag;
} pcb_t;

uint32_t pcb_index[NUM_TERM];
uint32_t process_count;
uint32_t active_terminal;
uint32_t shown_terminal;

// operations vars
file_ops_t stdin_ops;
file_ops_t stdout_ops;

// operations vars
file_ops_t RTC_ops;
file_ops_t file_ops;
file_ops_t directory_ops;

int32_t system_halt(int8_t status); 
int32_t system_execute(const int8_t* command); 
int32_t system_read(int32_t fd, void* buf, int32_t nbytes);
int32_t system_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t system_open(const int8_t* filename); 
int32_t system_close(int32_t fd); 
int32_t system_getargs(int8_t* buf, int32_t nbytes);
int32_t system_vidmap(int8_t** screen_start);

int32_t get_keyword(const int8_t* command, int8_t* buf, int32_t nbytes);
void store_args(const int8_t* command, pcb_t* pcb);

int32_t system_call_init();

pcb_t* get_current_pcb(uint32_t terminal, uint32_t pcb);

uint32_t get_current_pcb_ebp(uint32_t terminal, uint32_t pcb);

void switchContext(uint32_t new_eip);

void returnContext(uint32_t status, uint32_t old_esp, uint32_t old_ebp);

void system_switch_terminals(uint32_t terminal);

void system_schedule();

void init_scheduling();

#endif /* ASM */

#endif /* SYS_H */
