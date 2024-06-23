#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#define MAX_INODES 63
#define MAX_DATA_BLOCKS 1023 // 1024 total - 1 boot block - 63 inodes
#define MAX_FILE_BLOCKS 1087 // 1 boot block + 63 inodes + 1023 data blocks
#define MAX_FILE_NAME_SIZE 32
#define FOUR_KB_SIZE 4096
#define FOUR_MB (4<<20)
#define EIGHT_MB (8<<20)
#define EIGHT_KB (8<<10)
#define FOUR_KB (4<<10)
#define FD_SIZE 8
#define NUM_TERM 3

#include "types.h"
#include "lib.h"

typedef struct dentry_t {
    char file_name[MAX_FILE_NAME_SIZE];               // 32*1 = 32 bytes
    uint32_t file_type;                               // 4 bytes
    uint32_t inode_num;                               // 4 bytes
    uint32_t e_reserved[6];                           // 6*4 = 24 bytes
} dentry_t;

typedef struct inode_t{
    uint32_t byte_length;                     // 4 bytes
    uint32_t data_blocks[MAX_DATA_BLOCKS];    // 1023(4)
} inode_t;

typedef struct file_block {
    union {
        // Inode Struct
        struct {
            uint32_t byte_length;                     // 4 bytes
            uint32_t data_blocks[MAX_DATA_BLOCKS];    // 1023(4)
        } __attribute__ ((packed));

        // Boot Block Struct
        struct {
            uint32_t num_entries;                     // 4 bytes
            uint32_t num_inodes;                      // 4 bytes
            uint32_t num_data_blocks;                 // 4 bytes
            uint32_t b_reserved[13];                  // 13*4 bytes = 52 bytes
            dentry_t entries[MAX_INODES];             // 63*64 bytes = 4032 bytes
        } __attribute__ ((packed));

        // Data Block Struct
        struct {
            uint8_t file_contents[(MAX_DATA_BLOCKS+1)*4];    // 1024(4)
        } __attribute__ ((packed));
    };
} file_block; 

// 1 boot block + 63 inodes + 1023 data blocks
file_block* file_system_memory; // __attribute__((aligned(FOUR_KB_SIZE)));

void init_file_system(unsigned int file_system);

void set_file_fd(uint32_t fd);

void set_file_terminal(uint32_t terminal);

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t file_open(const uint8_t* filename);
int32_t file_close(int32_t fd);
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);

int32_t directory_open(const uint8_t* filename);
int32_t directory_close(int32_t fd);
int32_t directory_read(int32_t fd, void* buf, int32_t nbytes);
int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes);



#endif /* _FILE_SYSTEM_H */
