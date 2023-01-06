
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define DEVICE_FILE "/dev/pubsub3"
#define NUM_SUBSCRIBERS 3

int main(int argc, char *argv[])
{
    int fd, i;

    /* open the device file for reading */
    fd = open(DEVICE_FILE, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device file");
        return 1;
    }

    /* read the message from the device file */
    char buf[128];
    if (read(fd, buf, sizeof(buf)) < 0) {
        perror("Failed to read from device file");
        close(fd);
        return 1;
    }

    /* print the message to the console */
    printf("Message: %s\n", buf);

    /* close the device file */
    close(fd);

    /* open the device file for reading NUM_SUBSCRIBERS times */
    for (i = 0; i < NUM_SUBSCRIBERS; i++) {
        fd = open(DEVICE_FILE, O_RDONLY);
        if (fd < 0) {
            perror("Failed to open device file");
            return 1;
        }

        /* read the message from the device file */
        if (read(fd, buf, sizeof(buf)) < 0) {
            perror("Failed to read from device file");
            close(fd);
            return 1;
        }

        /* print the message to the console */
        printf("Message: %s\n", buf);

        /* close the device file */
        close(fd);
    }

    return 0;
}






