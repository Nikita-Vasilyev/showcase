#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "terminal.h"
#include "rtc.h"
#include "file_system.h"
#include "system_calls.h"
#include "paging.h"

#define PASS 1
#define FAIL 0

#define TESTED_INT 10 /* select the desired test interrupt */

// #define SINGLE_FREQ_TEST /* select if wanted single freq test */

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}

/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(uint8_t test){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 20; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	// Trigger the specific exception based on the exception_index
    switch (test) {
        case 0:
            asm volatile("int $0"); // Division Error
            break;
        case 1:
            asm volatile("int $1"); // Debug
            break;
        case 2:
            asm volatile("int $2"); // Non Maskable Interrupt
            break;
        case 3:
            asm volatile("int $3"); // Breakpoint
            break;
        case 4:
            asm volatile("int $4"); // Overflow
            break;
        case 5:
            asm volatile("int $5"); // Bound Range Exceeded
            break;
        case 6:
            asm volatile("int $6"); // Invalid Opcode
            break;
        case 7:
            asm volatile("int $7"); // Device Not Available
            break;
        case 8:
            asm volatile("int $8"); // Double Fault
            break;
        case 9:
            asm volatile("int $9"); // Coprocessor Segment Overrun
            break;
        case 10:
            asm volatile("int $10"); // Invalid TSS
            break;
        case 11:
            asm volatile("int $11"); // Segment Not Present
            break;
        case 12:
            asm volatile("int $12"); // Stack Segment Fault
            break;
        case 13:
            asm volatile("int $13"); // General Protection Fault
            break;
        case 14:
            asm volatile("int $14"); // Page Fault
            break;
        case 15:
            // Reserved
            break;
        case 16:
            asm volatile("int $16"); // Floating Point Exception
            break;
        case 17:
            asm volatile("int $17"); // Alignment Check
            break;
        case 18:
            asm volatile("int $18"); // Machine Check
            break;
        case 19:
            asm volatile("int $19"); // SIMD Floating Point Exception
            break;
        case 20:
            asm volatile("int $20"); // Virtualization Exception
            break;
        // Add additional cases for other exceptions if necessary
        default:
            printf("Invalid exception index\n");
            result = FAIL;
    }

	return result;
}

// add more tests here
// Note: new tests are editted above in the original idt_test function.

int spin_test() {
    int i = 0;
    while(1) {
        printf("%d\n", i++);
    }

    return 0;
}

/* Checkpoint 2 tests */

//testing if terminals work
int terminal_test(){
    int a, b;
    char test_text1[128] = "This is the terminal test!\n";
    printf("Bytes Read: %d\n", a=terminal_read(0, test_text1, 128));
    printf("Bytes Written: %d\n", b=terminal_write(0, test_text1, 128));
    if(a == b)
        return PASS;
    return FAIL;
}

//rtc test
int rtc_test()
{
    clear();

    int32_t my_fd = 0x0;
    void* my_buf = 0x0;
    uint32_t freq = 0x2;
    int32_t nbytes = 0x2;
    
    const uint8_t* my_filename = 0x0;
    open_rtc(my_filename);

//tests for single frequency
#ifdef SINGLE_FREQ_TEST
    uint16_t single_test = SINGLE_FREQ_TEST;
    if(write_rtc(my_fd, (const void*)single_test, nbytes) == -1)
        printf("unable to write frequency %d\n", single_test);
    else
        printf("frequency updated to %d\n", single_test);
#endif

    while(1){

        //settign rtc frequency to 2Hz intially
        printf("start rtc read: %d Hz\n", freq);
        read_rtc(my_fd, my_buf, nbytes);
        printf("end rtc read: %d Hz\n", freq);
        
        //checks if writing new freq works
        if(write_rtc(my_fd, (const void*)freq, nbytes) == -1){
            printf("unable to write frequency %d\n", freq);
            return FAIL;
        }
        else
            printf("frequency updated to %d\n", freq);
        
        //increasing frequency by power of 2
        freq *= 2;
        
        if(freq > 1024)
            return PASS;
    }
    
    close_rtc(my_fd);
    return FAIL;
}


// Test case for read_dentry_by_name and read_dentry_by_index
int file_test1(){
    clear();
    
    dentry_t test, test2;
    char name[MAX_FILE_NAME_SIZE] = "frame0.txt";
    int32_t index = 10; // this is the index of frame0.txt

    // Test to see if we can grab the file from the file system based on the name
    int32_t result = read_dentry_by_name((const uint8_t*)name, &test);
    if(result == -1)
        return FAIL;

    // Test to see if we can grab the file from the fily system based on the index
    result = read_dentry_by_index(index, &test2);
    if(result == -1)
        return FAIL;
    
    // Print the results and see if the i_node numbers and the file length are the same
    printf("test1 inode:%d name:%s\ntest2 inode:%d name:%s\n", test.inode_num, test.file_name, test2.inode_num, test2.file_name);
    printf("byte length: %d\n", file_system_memory[test.inode_num+1].byte_length);
    if(test.inode_num != test2.inode_num){
        printf("different elements found\n");
        return FAIL;
    }
    if(file_system_memory[test.inode_num+1].byte_length != file_system_memory[test2.inode_num+1].byte_length){
        printf("different elements found\n");
        return FAIL;
    }

    return PASS;
}

// Test case for read_data
int file_test2(){




    uint8_t buf[200];        // 200 is so that we have enough size in our buffer
    int32_t index = 47;      // 47 is the i_node index of frame1.txt
    int32_t length = 174;    // 174 is the actual data length

    // Test to see if we can read the file from the file system
    int32_t result = read_data(index, 0, buf, length);
    if(result != 0)
        return FAIL;

    // Print the contents of the file
    printf("file contents:\n");
    for(index=0; index < length; index++){
        printf("%c", (char)buf[index]);
    }

    return PASS;
}

// Test cases for file_open, file_read, and file_close for small txt files
int file_test_open_read_close_small_txt(){
    int index;

    //insert the filename that you like to serach for
    uint8_t name[MAX_FILE_NAME_SIZE] = "frame1.txt";
    dentry_t dentry;
    uint32_t result = read_dentry_by_name(name, &dentry);
    if(result == -1) {
        printf("could not file the file in the system\n");
        return FAIL;
    }

    // test out file_open with the given file name in name
    int32_t length = file_system_memory[dentry.inode_num + 1].byte_length;
    uint8_t buf[length];
    result = file_open(name);
    if(result == -1) {
        printf("could not open file\n");
        return FAIL;
    }

    // test out file_read for the given file name in name
    int bytes_read = file_read(0, buf, length);
    for(index=0; index < length; index++){
        putc(buf[index]);
    }

    if (bytes_read != 0) {
        printf("frame1: could not read every byte\n");
        return FAIL;
    }

    // test out file_close for the given file name in name
    file_close(0);


    //insert the filename that you like to serach for
    uint8_t name2[MAX_FILE_NAME_SIZE] = "frame0.txt";
    dentry_t dentry2;
    result = read_dentry_by_name(name2, &dentry2);
    if(result == -1) {
        printf("could not file the file in the system\n");
        return FAIL;
    }


    // test out file_open with the given file name in name2
    length = file_system_memory[dentry2.inode_num + 1].byte_length;
    uint8_t buf2[length];
    result = file_open(name2);
    if(result == -1) {
        printf("could not open file\n");
        return FAIL;
    }

    // test out file_read for the given file name in name2
    bytes_read = file_read(0, buf2, length);
    for(index=0; index < length; index++){
        if (buf2[index] != '\0') {
            putc(buf2[index]);
        }
    }
    if (bytes_read != 0) {
        printf("could not read every byte\n");
        return FAIL;
    }

    // test out file_close for the given file name in name2
    file_close(0);

    return PASS;
}

// Test cases for file_open, file_read, and file_close for large files
int file_test_open_read_close_large_txt(){
    int index;

    //insert the filename that you like to serach for
    uint8_t name[MAX_FILE_NAME_SIZE + 1] = "verylargetextwithverylongname.tx";
    dentry_t dentry;
    uint32_t result = read_dentry_by_name(name, &dentry);
    if(result == -1) {
        printf("case 1: could not find the file in the system\n");
        return FAIL;
    }

    // test out file_open with the given file name in name
    int32_t length = file_system_memory[dentry.inode_num + 1].byte_length;
    uint8_t buf[length];
    result = file_open(name);
    if(result == -1) {
        printf("could not open file\n");
        return FAIL;
    }

    // test out file_read for the given file name in name
    int bytes_read = file_read(0, buf, length);
    for(index=0; index < length; index++){
        putc(buf[index]);
    }

    if (bytes_read != 0) {
        printf("could not read every byte\n");
        return FAIL;
    }

    // test out file_close for the given file name in name
    file_close(0);

    //insert the filename that you like to serach for
    uint8_t name2[MAX_FILE_NAME_SIZE + 2] = "verylargetextwithverylongname.txt";
    dentry_t dentry2;
    result = read_dentry_by_name(name2, &dentry2);
    if(result != -1) {
        printf("found a file that shouldn't exist in the file system\n");
        return FAIL;
    }

    return PASS;
}

int file_test_open_read_close_exe(){

    //insert the filename that you like to serach for ls, cat, grep
    uint8_t name2[MAX_FILE_NAME_SIZE] = "grep";
    dentry_t dentry;
    int32_t result = read_dentry_by_name(name2, &dentry);
    if(result == -1) {
        printf("could not file the file in the system\n");
        return FAIL;
    }

    int32_t length = file_system_memory[dentry.inode_num + 1].byte_length;
    uint8_t buf2[length];

    result = file_open(name2);
    if(result == -1) {
        printf("could not open file\n");
        return FAIL;
    }

    int32_t bytes_read = file_read(0, buf2, length);
    int index;
    for(index=0; index < length; index++){
        // uncomment this if you want to see the executable magic numbers
        if (index <= 3 || index >= length - 36) {
            putc(buf2[index]);
        }
    }

    if (bytes_read != 0) {
        printf("could not read every byte\n");
        return FAIL;
    }

    file_close(0);

    return PASS;
}

// Test cases for read directories
int dir_test_open_read_close(){

    // read the directory
    uint8_t name[MAX_FILE_NAME_SIZE + 1] = "verylargetextwithverylongname.tx";
    int32_t result = 0;
    int i;
    for(i=0; i<20; i++){
        result = directory_read(0, name, 128);
        if(result == -1)
            return FAIL;
    }
    return PASS;
}

/* Checkpoint 3 tests */

int getargs_test(){
    char command[20] = "cat frame1.txt";
    char arg[20];
    char com[20];
    terminal_write(0, command, 20);
    system_getargs((int8_t*)arg, 20);
    get_keyword(command, (int8_t*)com, 20);
    printf("correct: %s, command: %s, args: %s\n", command, com, arg);
    if(strncmp("frame1.txt", arg, 10))
        return FAIL;
    return PASS;
}

// int file_desc_test(){
//     char command[20] = "cat frame1.txt";
//     char result[20];
//     int index;
//     char* buf = PROGRAM_START;
//     dentry_t dentry;
//     terminal_write(0, command, 20);
//     read_dentry_by_name("cat", &dentry);
//     system_execute(command);
//     // file_descriptor_table[1].entries->write(1, command, 15);
//     // file_descriptor_table[0].entries->read(0, result, 20);
//     // printf("write: %s, read: %s\n", command, result);
//     // if(strncmp(command, result, 15))
//     //     return FAIL;

//     // for(index=0; index < file_system_memory[dentry.inode_num+1].byte_length; index++){
//     //     printf("%c", buf[index]);
//     // }putc('\n');

//     return PASS;
// }

int system_execute_test(){
    char command[20] = "shell";
    system_call_init();
    printf("%d\n", system_execute(command));
    return PASS;
}

/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){

	// choose tested interrupt using the value of test
	// uint8_t test = TESTED_INT;
	// TEST_OUTPUT("idt_test", idt_test(test));
    // TEST_OUTPUT("spinning", spin_test());


    // RTC TEST CASES
    // TEST_OUTPUT("Test RTC", rtc_test());

    // TERMINAL TEST CASES
    // TEST_OUTPUT("Terminal Print Test", terminal_test());


    // FILE SYSTEM TEST CASES
    // TEST_OUTPUT("file test read_dentry_by_name and read_dentry_by_index", file_test1());
    // TEST_OUTPUT("file test read_data", file_test2());
    // TEST_OUTPUT("file test open, read, and close for small files", file_test_open_read_close_small_txt()); 
    // TEST_OUTPUT("file test open, read, and close for large files", file_test_open_read_close_large_txt()); 
    //  TEST_OUTPUT("file test open, read, and close for executables", file_test_open_read_close_exe());
    // TEST_OUTPUT("file test directory open, read, close", dir_test_open_read_close());

    // SYS CALLS TEST
    // TEST_OUTPUT("getarg system call test", getargs_test());
    // TEST_OUTPUT("file descriptor table test", file_desc_test());
    TEST_OUTPUT("execute test", system_execute_test());

	// launch your tests here
}
