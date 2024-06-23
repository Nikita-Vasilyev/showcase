#include "file_system.h"
#include "system_calls.h"

/* void init_file_system(unsigned int file_system);
 * Inputs: the address of the file system
 * Return Value: nothing
 * Function: loads the file system array filled with file blocks */
void init_file_system(unsigned int file_system) {
    file_system_memory = (file_block*)file_system;
}

int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {

    int i, j;
    
    // check if the length of fname exceeds the max file name length
    int length_of_string = strlen((int8_t*)fname);

    if (length_of_string > MAX_FILE_NAME_SIZE + 1) {
        return -1;
    }

    if (fname[0] == NULL)
        return -1;

    // Get the array of dentry_t from the boot_block
    dentry_t* list_of_dentries = file_system_memory[0].entries;
    dentry_t c_dentry = list_of_dentries[0];

    //loop through the list of dentries
    for (i = 0; i < file_system_memory[0].num_inodes; i++) {

        // grab the current dentry object
        c_dentry = list_of_dentries[i];

        // get the max size of the two
        j = strlen(list_of_dentries[i].file_name);
        if (j < length_of_string)
            j = length_of_string;
        else if(j > MAX_FILE_NAME_SIZE)
            j = MAX_FILE_NAME_SIZE;

        // check if the name matches
        if (strncmp((int8_t*)(list_of_dentries[i].file_name), (int8_t*)fname, j) == 0){
            // copy into the dentry
            memcpy(dentry, &c_dentry, 64);
            return 0;
        }
    }

    // We didn't find a dentry that matched the file name
    return -1;
}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry) {
    
    // check for an invalid index
    if (index < 0 || index >= file_system_memory[0].num_inodes) {
        return -1;
    }

    // grab the dentry at 'index' in the boot block
    dentry_t c_dentry = file_system_memory[0].entries[index];

    // copy into the dentry
    strcpy(dentry->file_name, c_dentry.file_name);
    dentry->file_type = c_dentry.file_type;
    dentry->inode_num = c_dentry.inode_num;
    return 0;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    // grab the inode from boot block
    file_block c_inode = file_system_memory[inode + 1];

    // grab the list of data blocks from the inode
    uint32_t* c_data_blocks = c_inode.data_blocks;

    // get starting offset and block index
    uint32_t data_block_index = offset / FOUR_KB;
    uint32_t data_block_offset = offset % FOUR_KB; 

    uint32_t copied;

    // loop through whole buffer and copy elements if they exist
    for(copied=0; copied<length; copied++, data_block_offset++) {
        // check if we need to navigate to the next block
        if(data_block_offset == FOUR_KB){
            data_block_index++;
            data_block_offset = 0;
        }

        // check if the current index is out of bounds
        if((data_block_index*FOUR_KB)+data_block_offset >= c_inode.byte_length){
            buf[copied] = NULL;
            return copied;
        }

        // copy over value from data block to buf
        buf[copied] = file_system_memory[c_data_blocks[data_block_index] + file_system_memory[0].num_inodes + 1].file_contents[data_block_offset];
        /* void* memcpy(void* dest, const void* src, uint32_t n);*/
        // memcpy(&(buf[copied]), &(file_system_memory[c_data_blocks[data_block_index] + file_system_memory[0].num_inodes + 1].file_contents[data_block_offset]), 1);
    }

    return length;
}

dentry_t dentry[NUM_TERM][FD_SIZE];
uint32_t d_offset[NUM_TERM][FD_SIZE];
uint32_t d_index[NUM_TERM];
uint32_t active_file_terminal = 0;

/* void set_file_fd(int32_t fd)
 * Inputs: int32_t fd
 * Return Value: none
 * Function: set the next fd to be written */
void set_file_fd(uint32_t fd){
    d_index[active_file_terminal] = fd;
}

/* void set_file_terminal(int32_t terminal)
 * Inputs: int32_t terminal
 * Return Value: none
 * Function: sets the current terminal */
void set_file_terminal(uint32_t terminal){
    active_file_terminal = terminal;
}

/* int8_t file_open(const uint8_t* filename);
 * Inputs: name of the filne
 * Return Value: returns 0 if the file was open successfully, -1 if not
 * Function: tries to find the file through in the file system */
 int32_t file_open(const uint8_t* filename) {
    int32_t result = read_dentry_by_name(filename, &(dentry[active_file_terminal][d_index[active_file_terminal]]));
    d_offset[active_file_terminal][d_index[active_file_terminal]] = 0;
    return result;
}

/* int8_t file_close(int32_t fd)
 * Inputs: int32_t fd
 * Return Value: returns 0
 * Function: resets the dentry */
int32_t file_close(int32_t fd) {
    memset(dentry[active_file_terminal][fd].file_name, 0, MAX_FILE_NAME_SIZE);
    dentry[active_file_terminal][fd].file_type = -1;
    dentry[active_file_terminal][fd].inode_num = -1;
    return 0;
}

/* file_read(int32_t fd, void* buf, int32_t nbytes);
 * Inputs: file descriptor, buffer, and number of bytes to copy
 * Return Value: returns the number of bytes successfully copied, and 0 if everything was copied
 * Function: reads nbytes of data from the inode and puts it into the buffer */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes) {

    int32_t result = read_data(dentry[active_file_terminal][fd].inode_num, d_offset[active_file_terminal][fd], (uint8_t*)buf, nbytes);

    d_offset[active_file_terminal][fd] += result;

    return result;
}

/* int8_t file_write();
 * Inputs: None
 * Return Value: returns -1
 * Function: does nothing (readonly file_system) */
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/* int8_t directory_open(const uint8_t* dir_name);
 * Inputs: dir_name
 * Return Value: 0 if not -1
 * Function: does nothing (readonly directory entry) */
int32_t directory_open(const uint8_t* filename) {
    int32_t result = read_dentry_by_name(filename, &(dentry[active_file_terminal][d_index[active_file_terminal]]));
    return result;
}

/* int8_t directory_close()
 * Inputs: None
 * Return Value: returns 0
 * Function: clears directory*/
int32_t directory_close(int32_t fd) {
    memset(dentry[active_file_terminal][fd].file_name, 0, MAX_FILE_NAME_SIZE);
    dentry[active_file_terminal][fd].file_type = -1;
    dentry[active_file_terminal][fd].inode_num = -1;
    return 0;
}

/* int8_t directory_read();
 * Inputs: None
 * Return Value: returns 0
 * Function: lists all files and sizes in directory */
int32_t directory_read(int32_t fd, void* buf, int32_t nbytes) {
    
    //read_dentry_by_index
    //make PCB pointer
    //user PID pointer to grab file position
    //find name using index
    //flie position isindex
    //this data located in dentry buffer
    //the file name in the dentry buffer you copy into buf
    //increment file posotion

    static int i = 0;
    dentry_t dentry;

    if(i == file_system_memory[0].num_entries){
        i = 0;
        return 0;
    }   
    if(read_dentry_by_index(i, &dentry))
        return -1;

    strncpy((int8_t*)buf, (int8_t*)dentry.file_name, MAX_FILE_NAME_SIZE);
    ((uint8_t*)buf)[MAX_FILE_NAME_SIZE] = '\0';

    i++;

    return strlen((int8_t*)buf);
}

/* int8_t directory_write();
 * Inputs: None
 * Return Value: returns -1
 * Function: does nothing (readonly file_system) */
int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}
