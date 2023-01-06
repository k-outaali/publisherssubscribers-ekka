#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../include/pubsub.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define O_RDONLY 00
#define O_WRONLY 01

/*
 * Test pubsub_open
 * Test exigence V1-E1 V1-E2 V1-E3 V1-E4 V1-E5 V1-E19
 */
void test_open() {
    //pubsub_reset();
    char * category = "test_open.txt";

    CU_ASSERT(-1 == pubsub_open(category, 2, 777));
    CU_ASSERT(errno == 152);
    CU_ASSERT(-1 == pubsub_open(category, -3, 777));
    CU_ASSERT(errno == 152);

    CU_ASSERT(-1 != pubsub_open(category, O_WRONLY, 777));
    for (int i = 0; i < 12; i++)
        pubsub_open(category, O_WRONLY, 777);
    CU_ASSERT(-1 == pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
    CU_ASSERT(errno == 151);

    CU_ASSERT(-1 == pubsub_open("category", O_RDONLY, -1));
    CU_ASSERT(errno == ENOENT);
    CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR));
    for (int i = 0; i < 9; i++)
        pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(-1 == pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
    CU_ASSERT(errno == 150);
    return;
}

/*
 * Test pubsub_write
 * Test exigence V1-E9 V1-E10 V1-E11 V1-E16 V1-E17 V1-E19
 */
void test_write() {
    //pubsub_reset();
    char * category = "test_write.txt";
    int fd_write = pubsub_open(category, O_WRONLY, 777);
    CU_ASSERT(-1 == pubsub_write(fd_write, "test", -1));
    CU_ASSERT(errno == 153);
    CU_ASSERT(-1 == pubsub_write(fd_write, "test", 0));
    CU_ASSERT(errno == 153);
    CU_ASSERT(-1 == pubsub_write(fd_write, "test", 4));
    CU_ASSERT(errno == 154);
    CU_ASSERT(-1 == pubsub_write(fd_write, "test", 1025));
    CU_ASSERT(errno == 153);
    CU_ASSERT(1 == pubsub_write(fd_write, "", 1));
    CU_ASSERT(2 == pubsub_write(fd_write, " ", 2));
    CU_ASSERT(5 == pubsub_write(fd_write, "test", 5));
    CU_ASSERT(1023 == pubsub_write(fd_write, "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 1023));

    //pubsub_reset();
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_WRONLY, 777);
    CU_ASSERT(-1 == pubsub_write(fd_inexistant, "test", 5));
    CU_ASSERT(errno == EBADF);

    //pubsub_reset();
    int fd = pubsub_open(category, O_RDONLY, 777);
    CU_ASSERT(-1 == pubsub_write(fd, "test", 5));
    CU_ASSERT(errno == EBADF);
}

/*
 * Test pubsub_read
 * Test exigence V1-E12 V1-E13 V1-E14 V1-E15 V1-E16 V1-E17 V1-E18 V1-E19
 */
void test_read() {
    //pubsub_reset();
    char * category = "test_read.txt";
    char str[4];
    int fd_read = pubsub_open(category, O_RDONLY, 777);

    CU_ASSERT(-1 == pubsub_read(fd_read, str, -1));
    CU_ASSERT(errno == 153);
    CU_ASSERT(-1 == pubsub_read(fd_read, str, 0));
    CU_ASSERT(errno == 153);
    CU_ASSERT(-1 == pubsub_read(fd_read, str, 1025));
    CU_ASSERT(errno == 153);

    CU_ASSERT(4 == pubsub_read(fd_read, str, 4));

    //pubsub_reset();
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_RDONLY, 777);
    CU_ASSERT(-1 == pubsub_read(fd_inexistant, str, 5));
    CU_ASSERT(errno == EBADF);

    //pubsub_reset();
    char * category_1023 = "test_1023.txt";   
    char str_1023[1023]; 
    int fd_read_1023 = pubsub_open(category_1023, O_RDONLY, 777);
    CU_ASSERT(1023 == pubsub_read(fd_read_1023, str_1023, 1023));

    //pubsub_reset();
    int fd = pubsub_open(category, O_WRONLY, 777);
    CU_ASSERT(-1 == pubsub_read(fd, str, 4));
    CU_ASSERT(errno == EBADF);
}

/*
 * Test pubsub_read
 * Test exigence V1-E6 V1-E19
 */
void test_ioctl() {
    //pubsub_reset();
    char * category = "test_ioctl.txt";
    int fd = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    CU_ASSERT(-1 == pubsub_ioctl(fd, 0, 0));
    CU_ASSERT(errno == 155);
}

/*
 * Test pubsub_write
 * Test exigence V1-E7 V1-E8 V1-E19
 */
void test_close() {
    //pubsub_reset();
    char * category = "test_write.txt";
    int fd = pubsub_open(category, O_WRONLY, 777);
    CU_ASSERT(0 == pubsub_close(fd));

    //pubsub_reset();
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_WRONLY, 777);
    CU_ASSERT(-1 == pubsub_close(fd_inexistant));
    CU_ASSERT(errno == EBADF);
    pubsub_reset();
}

int main() {
    CU_initialize_registry();
    
    CU_pSuite suite_open = CU_add_suite("open", 0, 0);
    CU_add_test(suite_open, "open", test_open);

    CU_pSuite suite_write = CU_add_suite("write", 0, 0);
    CU_add_test(suite_write, "write", test_write);

    CU_pSuite suite_read = CU_add_suite("read", 0, 0);
    CU_add_test(suite_read, "read", test_read);

    CU_pSuite suite_ioctl = CU_add_suite("ioctl", 0, 0);
    CU_add_test(suite_ioctl, "ioctl", test_ioctl);

    CU_pSuite suite_close = CU_add_suite("close", 0, 0);
    CU_add_test(suite_close, "close", test_close);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}