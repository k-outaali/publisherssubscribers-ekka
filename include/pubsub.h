

#ifndef PUBSUB_H
#define PUBSUB_H

#define MAX_NUM_SUBS 4
#define MAX_NUM_PUBS 1
#define MAX_MSG_SIZE 1024

int pubsub_open(char* p_category, int p_options, int p_mode);

int pubsub_read(int p_fd, char* p_message, int p_size);

int pubsub_write(int p_fd, char* p_message, int p_size);

int pubsub_close(int p_fd);

int pubsub_ioctl(int p_fd, int p_request, int p_options);



#endif // PUBSUB_H