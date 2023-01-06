#include "../include/pubsub.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#define O_RDONLY 00
#define O_WRONLY 01

/*
 * Test pubsub_open
 * Test exigence V1-E1 V1-E2 V1-E3 V1-E4 V1-E5 V1-E19
 */
void test_open() {
    pubsub_reset();
    char * category = "test_open.txt";

    //printf("%d\n",  -1 == pubsub_open(category, 2, 777));
    //printf("%d\n",  errno == 152);
    printf("%d\n",  -1 == pubsub_open(category, -3, 777));
    printf("%d\n",  errno == 152);

    printf("%d\n",  -1 != pubsub_open(category, O_WRONLY, 777));
    for (int i = 0; i < 12; i++)
        pubsub_open(category, O_WRONLY, 777);
    //printf("%d\n",  -1 == pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
    //printf("%d\n",  errno == 151);

    printf("%d\n",  -1 == pubsub_open("category", O_RDONLY, -1));
    printf("%d\n",  errno == ENOENT);
    printf("%d\n",  -1 != pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR));
    for (int i = 0; i < 9; i++)
        pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR);
    printf("%d\n",  -1 == pubsub_open(category, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
    printf("%d\n",  errno == 150);
    return;
}

/*
 * Test pubsub_write
 * Test exigence V1-E9 V1-E10 V1-E11 V1-E16 V1-E17 V1-E19
 */
void test_write() {
    pubsub_reset();
    char * category = "test_write.txt";
    int fd_write = pubsub_open(category, O_WRONLY, 777);
    printf("%d\n",  -1 == pubsub_write(fd_write, "test", -1));
    printf("%d\n",  errno == 153);
    printf("%d\n",  -1 == pubsub_write(fd_write, "test", 0));
    printf("%d\n",  errno == 153);
    printf("%d\n",  -1 == pubsub_write(fd_write, "test", 4));
    printf("%d\n",  errno == 154);
    printf("%d\n",  -1 == pubsub_write(fd_write, "test", 1025));
    printf("%d\n",  errno == 153);
    printf("%d\n",  1 == pubsub_write(fd_write, "", 1));
    printf("%d\n",  2 == pubsub_write(fd_write, " ", 2));
    printf("%d\n",  5 == pubsub_write(fd_write, "test", 5));
    printf("%d\n",  1023 == pubsub_write(fd_write, "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 1023));

    pubsub_reset();
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_WRONLY, 777);
    printf("%d\n",  -1 == pubsub_write(fd_inexistant, "test", 5));
    printf("%d\n",  errno == EBADF);

    pubsub_reset();
    int fd = pubsub_open(category, O_RDONLY, 777);
    printf("%d\n",  -1 == pubsub_write(fd, "test", 5));
    printf("%d\n",  errno == EBADF);
}

/*
 * Test pubsub_read
 * Test exigence V1-E12 V1-E13 V1-E14 V1-E15 V1-E16 V1-E17 V1-E18 V1-E19
 */
void test_read() {
    pubsub_reset();
    char * category = "test_read.txt";
    char str[4];
    int fd_read = pubsub_open(category, O_RDONLY, 777);

    printf("%d\n",  -1 == pubsub_read(fd_read, str, -1));
    printf("%d\n",  errno == 153);
    printf("%d\n",  -1 == pubsub_read(fd_read, str, 0));
    printf("%d\n",  errno == 153);
    printf("%d\n",  -1 == pubsub_read(fd_read, str, 1025));
    printf("%d\n",  errno == 153);

    printf("%d\n",  4 == pubsub_read(fd_read, str, 4));

    //pubsub_reset();
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_RDONLY, 777);
    printf("%d\n",  -1 == pubsub_read(fd_inexistant, str, 5));
    printf("%d\n",  errno == EBADF);

    //pubsub_reset();
    char * category_1023 = "test_1023.txt";   
    char str_1023[1023]; 
    int fd_read_1023 = pubsub_open(category_1023, O_RDONLY, 777);
    printf("%d\n",  1023 == pubsub_read(fd_read_1023, str_1023, 1023));

    pubsub_reset();
    int fd = pubsub_open(category, O_WRONLY, 777);
    printf("%d\n",  -1 == pubsub_read(fd, str, 4));
    printf("%d\n",  errno == EBADF);
}

/*
 * Test pubsub_read
 * Test exigence V1-E6 V1-E19
 */
void test_ioctl() {
    pubsub_reset();
    char * category = "test_ioctl.txt";
    int fd = pubsub_open(category, O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    printf("%d\n",  -1 == pubsub_ioctl(fd, 0, 0));
    printf("%d\n",  errno == 155);
}

/*
 * Test pubsub_write
 * Test exigence V1-E7 V1-E8 V1-E19
 */
void test_close() {
    pubsub_reset();
    char * category = "test_write.txt";
    int fd = pubsub_open(category, O_WRONLY, 777);
    printf("%d\n",  0 == pubsub_close(fd));

    //pubsub_reset();
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_WRONLY, 777);
    printf("%d\n",  -1 == pubsub_close(fd_inexistant));
    printf("%d\n",  errno == EBADF);
}

int main() {
    char * inexistant = "inexistant.txt";
    int fd_inexistant = pubsub_open(inexistant, O_WRONLY, 777);
    printf("%d\n", -1 == pubsub_close(fd_inexistant));
    test_open();
    test_close();
    test_write();
    test_read();
    test_ioctl();

    return 0;
}