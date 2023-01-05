


#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct cat{
	char name[100];
    int num_subs;
    int num_pubs;
	int max_subs;
	int max_pubs;
	int max_size;
	int *read_fds;
	int *write_fds;
}cat_t;

typedef struct list{
	struct cat *cat;
    struct list *next;
}list_t;


void push(list_t * head, struct cat *cat);

void *realloc_reads(struct cat *cat);

void *realloc_writes(struct cat *cat);

void *find_cat_by_name(list_t *head, char *name);

void *find_cat_by_fd(list_t *head, int fd, int *witch);

int pop(list_t ** head);

int remove_by_index(list_t ** head, int n);

void cat_init(struct cat *cat, char *name);

#endif // LINKED_LIST_H