


#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct cat{
	char name[100];
    int num_subs;
    int num_pubs;
	int max_subs;
	int max_pubs;
	int max_size;
	int *read_fds;
	int *write_fds;
};

typedef struct cat cat_t;

struct list{
	struct cat *cat;
    struct list *next;
};

typedef struct list list_t;

void push(list_t * head, struct cat *cat);

void realloc_reads(struct cat *cat);

void realloc_writes(struct cat *cat);

void *find_cat_by_name(list_t *head, char *name);

void *find_cat_by_fd(list_t *head, int fd, int *witch);

int remove_item(list_t * head, list_t *to_remove);

void cat_init(struct cat *cat, char *name);

void free_all(list_t *head);

#endif // LINKED_LIST_H