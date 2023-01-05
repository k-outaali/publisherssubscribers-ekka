#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../include/linked_list.h"
#include "../include/pubsub.h"



void cat_init(struct cat *cat, char *name){
    cat->max_pubs = MAX_NUM_PUBS;
    cat->max_size = MAX_MSG_SIZE;
    cat->max_subs = MAX_NUM_SUBS;
    cat->read_fds = (int *) malloc(cat->max_subs * sizeof(int)); // NULL check
    cat->write_fds = (int *) malloc(cat->max_pubs * sizeof(int)); // NULL check
    strncpy(cat->name, name, 100);
    cat->num_pubs = 0;
    cat->num_subs = 0;
}
void push(list_t * head, struct cat *cat) {
    list_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (list_t *) malloc(sizeof(list_t));
    cat->read_fds = (int *) malloc(cat->max_subs*sizeof(int));
    cat->write_fds = (int *) malloc(cat->max_pubs*sizeof(int));
    current->next->cat = cat;
    current->next->next = NULL;
}

void *realloc_reads(struct cat *cat){
    return realloc(cat->read_fds, cat->max_subs*sizeof(int));
}

void *realloc_writes(struct cat *cat){
    return realloc(cat->write_fds, cat->max_pubs*sizeof(int));
}

void *find_cat_by_name(list_t *head, char *name){
    list_t *cur = head;
    while (cur != NULL) {
        if (strcmp(cur->cat->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;  
}

void *find_cat_by_fd(list_t *head, int fd, int *witch){
    list_t *cur = head;
    int *cur_r = head->cat->read_fds;
    int *cur_w = head->cat->write_fds;
    while (cur != NULL) {
        while (cur_r != NULL) {
            if(fd == *cur_r){
                *witch = 100;
                return cur;
            }
        }
        while (cur_w != NULL) {
            if(fd == *cur_w){
                *witch = 200;
                return cur;
            }
        }
        cur = cur->next;
    }
    return NULL;  
}


int pop(list_t ** head) {
    list_t * next_list = NULL;

    if (*head == NULL) {
        return -1;
    }

    next_list = (*head)->next;
    // free the content
    free(*head);
    *head = next_list;

    return 0;
}

int remove_by_index(list_t ** head, int n) {
    int i = 0;
    list_t * current = *head;
    list_t * temp_list = NULL;

    if (n == 0) {
        return pop(head);
    }

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }

    if (current->next == NULL) {
        return -1;
    }

    temp_list = current->next;
    current->next = temp_list->next;
    free(temp_list);

    return 0;

}
