

#ifndef PUBSUB_H
#define PUBSUB_H

#define MAX_NUM_SUBS 10
#define MAX_NUM_PUBS 10
#define MAX_MSG_SIZE 1024
#define PS_SUBS 100
#define PS_PUBS 200
#define PS_MSG 300

typedef struct cat{
	char name[100];
	int max_subs;
	int max_pubs;
	int max_size;
	int *read_fds;
	int *write_fds;
}cat_t;

int pubsub_open(char* p_category, int p_options, int p_mode);

int pubsub_read(int p_fd, char* p_message, int p_size);

int pubsub_write(int p_fd, char* p_message, int p_size);

int pubsub_close(int p_fd);

int pubsub_ioctl(int p_fd, int p_request, int p_options);

void pubsub_reset();

char *pubsub_get_error();

#endif // PUBSUB_H