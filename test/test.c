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

void exigence_v1_e1() {
    //pubsub_reset();   
    char * category = "test.txt";
    CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, S_IRUSR));
}

void exigence_v1_e2() {
    //pubsub_reset();    
    char * category = "test.txt";
    //for (int i = 0; i < 10; i++)
        CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, S_IRUSR));
    CU_ASSERT(-1 == pubsub_open(category, O_RDONLY, S_IRUSR));
    CU_ASSERT(errno == 150);

    //for (int i = 0; i < 10; i++)
        CU_ASSERT(-1 != pubsub_open(category, O_WRONLY, S_IWUSR));
    CU_ASSERT(-1 == pubsub_open(category, O_WRONLY, S_IWUSR));
    CU_ASSERT(errno == 151);
}

void exigence_v1_e3() {
    //pubsub_reset();      
    char * category = "test.txt";
    CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, S_IRUSR));
    CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, S_IRGRP));
    CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, S_IROTH));
    CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, -1));

    //pubsub_reset();      
    CU_ASSERT(-1 != pubsub_open(category, O_WRONLY, S_IWUSR));
    //pubsub_reset();      
    CU_ASSERT(-1 != pubsub_open(category, O_WRONLY, S_IWGRP));
    //pubsub_reset();      
    CU_ASSERT(-1 != pubsub_open(category, O_WRONLY, S_IWOTH));
    //pubsub_reset();      
    CU_ASSERT(-1 != pubsub_open(category, O_WRONLY, -1));
}

void exigence_v1_e4() {
    //pubsub_reset();      
    char * category = "inexistant.txt";
    CU_ASSERT(-1 == pubsub_open(category, O_RDONLY, S_IRUSR));
    CU_ASSERT(errno == ENOENT);
    CU_ASSERT(-1 == pubsub_open(category, O_WRONLY, S_IWUSR));
    CU_ASSERT(errno == ENOENT);
}

void exigence_v1_e5() {
    //pubsub_reset();      
    char * category = "test.txt";
    CU_ASSERT(-1 == pubsub_open(category, -1, S_IRUSR));
    CU_ASSERT(errno == 152);
}

void exigence_v1_e7() {
    //pubsub_reset();      
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IWUSR);
    CU_ASSERT(0 == pubsub_close(fd_write));
    int fd_read = pubsub_open(category, O_RDONLY, S_IWUSR);
    CU_ASSERT(0 == pubsub_close(fd_read));
}

void exigence_v1_e8() {
    //pubsub_reset();      
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_WRONLY, 777);
    CU_ASSERT(-1 == pubsub_close(fd_inexistant));
    CU_ASSERT(errno == EBADF);
}

void exigence_v1_e9() {
    //pubsub_reset();      
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IWUSR);
    CU_ASSERT(5 == pubsub_write(fd_write, "1234", 5));
    CU_ASSERT(-1 == pubsub_write(fd_write, "12345", 5));
    CU_ASSERT(errno == 154);
}

void exigence_v1_e10() {
    //pubsub_reset();      
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IWUSR);
    CU_ASSERT(1 == pubsub_write(fd_write, "", 1));
    CU_ASSERT(-1 == pubsub_write(fd_write, "", 0));
    CU_ASSERT(errno == 153);
}

void exigence_v1_e11() {
    //pubsub_reset();      
    char * category = "inexistant.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IWUSR);
    CU_ASSERT(-1 == pubsub_write(fd_write, "1234", 5));
    CU_ASSERT(errno == EBADF);
}

void exigence_v1_e12() {
    //pubsub_reset();      
    char * category = "test_read.txt";
    char str[10];
    int fd_read = pubsub_open(category, O_RDONLY, S_IRUSR);
    CU_ASSERT(4 == pubsub_read(fd_read, str, 4));
}

void exigence_v1_e13() {
    //pubsub_reset();      
    char * category = "test_read.txt";
    char str[10];
    int fd_read = pubsub_open(category, O_RDONLY, S_IRUSR);
    CU_ASSERT(-1 == pubsub_read(fd_read, str, 0));
    CU_ASSERT(errno == 153);
}

void exigence_v1_e14() {
    //pubsub_reset();      
    char * category = "inexistant.txt";
    char str[10];
    int fd_read = pubsub_open(category, O_RDONLY, S_IRUSR);
    CU_ASSERT(-1 == pubsub_read(fd_read, str, 10));
    CU_ASSERT(errno == EBADF);
}

void exigence_v1_e15() {
    //pubsub_reset();      
    char * category = "test_read.txt";
    char str[10];
    int fd_read = pubsub_open(category, O_RDONLY, S_IRUSR);
    CU_ASSERT(-1 == pubsub_read(fd_read, str, 10));
}

void exigence_v1_e16() {
    //pubsub_reset();    
    char * category = "test.txt";
    char str[1025];
    int fd_read = pubsub_open(category, O_RDONLY, S_IRUSR);
    CU_ASSERT(-1 == pubsub_read(fd_read, str, 1024));
    CU_ASSERT(errno == 153);

    int fd_write = pubsub_open(category, O_WRONLY, S_IWUSR);
    CU_ASSERT(-1 == pubsub_read(fd_read, "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 1025));
    CU_ASSERT(errno == 153);
}

void exigence_v1_e17() {
    //pubsub_reset();    
    char * category = "test.txt";
    char str[4];
    int fd_read = pubsub_open(category, O_WRONLY, S_IRUSR);
    int fd_write = pubsub_open(category, O_RDONLY, S_IWUSR);
    CU_ASSERT(-1 == pubsub_read(fd_read, str, 4));
    CU_ASSERT(errno == EBADF);
    CU_ASSERT(-1 == pubsub_write(fd_write, "1234", 5));
    CU_ASSERT(errno == EBADF);
}

void exigence_v1_e18() {
    //pubsub_reset();    
    char * category = "test_read.txt";
    char str[10];
    int fd_read = pubsub_open(category, O_RDONLY, S_IRUSR);
    CU_ASSERT(1 == pubsub_read(fd_read, str, 1));
    CU_ASSERT(3 == pubsub_read(fd_read, str, 3));

    char * category_1023 = "test_1023.txt";   
    char str_1023[1023]; 
    int fd_read_1023 = pubsub_open(category_1023, O_RDONLY, S_IRUSR);
    CU_ASSERT(1023 == pubsub_read(fd_read_1023, str_1023, 1023));
}

void exigence_v2_e1() {
    //pubsub_reset();    
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, 777);
    pubsub_ioctl(fd_write, PS_MSG, 5);
    CU_ASSERT(4 == pubsub_write(fd_write, "123", 4));
    CU_ASSERT(-1 == pubsub_write(fd_write, "123456789", 10));
    CU_ASSERT(errno == 153);

    char * read = "test_read.txt";
    char str_5[5];
    char str_10[5];
    int fd_read = pubsub_open(read, O_RDONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(4 == pubsub_read(fd_read, str_5, 4));
    CU_ASSERT(-1 == pubsub_read(fd_read, str_10, 10));
    CU_ASSERT(errno == 153);
}

void exigence_v2_e2() {
    //pubsub_reset();    
    int nb_pub = 5;
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(0 == pubsub_ioctl(fd_write, PS_PUBS, nb_pub));
    for (int i = 0; i < nb_pub ; i++)
        CU_ASSERT(-1 != pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR));
    CU_ASSERT(-1 == pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR));
}

void exigence_v2_e3() {
    //pubsub_reset();    
    int nb_subs = 5;
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(0 == pubsub_ioctl(fd_write, PS_SUBS, nb_subs));
    for (int i = 0; i < nb_subs; i++)
        CU_ASSERT(-1 != pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR));
    CU_ASSERT(-1 == pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR));
}

void exigence_v2_e4() {
    //pubsub_reset();    
    CU_ASSERT(-1 == pubsub_ioctl(-1, PS_MSG, 512));
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    int fd_read = pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(0 == pubsub_ioctl(fd_write, PS_MSG, 512));
    CU_ASSERT(-1 == pubsub_ioctl(fd_read, PS_MSG, 512));
}

void exigence_v2_e5() {
    //pubsub_reset();    
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(-1 == pubsub_ioctl(fd_write, PS_MSG, -1));
    CU_ASSERT(-1 == pubsub_ioctl(fd_write, PS_MSG, 0));
}

void exigence_v2_e6() {
    //pubsub_reset();    
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(-1 == pubsub_ioctl(fd_write, -1, 512));
    CU_ASSERT(-1 == pubsub_ioctl(fd_write, 0, 512));
}

void exigence_v2_e7() {
    //pubsub_reset();    
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(0 == pubsub_ioctl(fd_write, PS_MSG, 512));
}

void exigence_v2_e8() {
    //pubsub_reset();    
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(0 == pubsub_ioctl(fd_write, PS_PUBS, 5));
}

void exigence_v2_e9() {
    //pubsub_reset();    
    char * category = "test.txt";
    int fd_write = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR);
    CU_ASSERT(0 == pubsub_ioctl(fd_write, PS_SUBS, 5));
}

/*
 * Test  
 */
void test() {
    // Exigence V1
    //exigence_v1_e1();
    //exigence_v1_e2();
    /*
    exigence_v1_e3();
    exigence_v1_e4();
    exigence_v1_e5();
    exigence_v1_e7();
    exigence_v1_e8();
    exigence_v1_e9();
    exigence_v1_e10();
    exigence_v1_e11();
    exigence_v1_e12();
    exigence_v1_e13();
    exigence_v1_e14();
    
    exigence_v1_e15();
    exigence_v1_e16();
    exigence_v1_e17();
    exigence_v1_e18();
    // Exigence V2
    exigence_v2_e1();
*/
    exigence_v2_e2();
    
    exigence_v2_e3();

    exigence_v2_e4();
    exigence_v2_e5();
    exigence_v2_e6();
    exigence_v2_e7();
    exigence_v2_e8();
    exigence_v2_e9();
    pubsub_reset();
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite_read = CU_add_suite("test", 0, 0);
    CU_add_test(suite_read, "test", test);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}