#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../include/linked_list.h"
#include "../include/pubsub.h"



void cat_init(struct cat *cat, char *name){
    cat->max_pubs = MAX_NUM_PUBS;
    cat->max_size = MAX_MSG_SIZE;
    cat->max_subs = MAX_NUM_SUBS;
    cat->read_fds = (int *) malloc(cat->max_subs * sizeof(int)); // NULL check
    memset(cat->read_fds, 0, cat->max_subs * sizeof(int)); // NULL check

    cat->write_fds = (int *) malloc(cat->max_pubs * sizeof(int)); // NULL check
    memset(cat->write_fds, 0, cat->max_pubs * sizeof(int)); // NULL check
    
    strncpy(cat->name, name, 100);
    cat->num_pubs = 0;
    cat->num_subs = 0;
}

void push(list_t * head, struct cat *cat) {
    list_t * cur = head;
    
    while (cur->next != NULL) {
        if(cur->cat == NULL){
            cur->cat = cat;
            return;
        }
        cur = cur->next;
    }

    cur->next = (list_t *) malloc(sizeof(list_t));
    cur->next->cat = cat;
    cur->next->next = NULL;
}

void free_all(list_t *head){
    list_t *cur = head;
    list_t *last = NULL;
    while (cur != NULL){
        if(cur->cat != NULL){
            if(cur->cat->read_fds != NULL){
                free(cur->cat->read_fds);
            }
            if(cur->cat->write_fds != NULL){
                free(cur->cat->write_fds);
            }
            free(cur->cat);
        }
        last = cur;
        cur = cur->next;
        free(last);
    }   
}

void realloc_reads(struct cat *cat){
    cat->read_fds= (int *)realloc(cat->read_fds, cat->max_subs*sizeof(int));//NULL
    memset(cat->read_fds + (cat->max_subs - cat->num_subs - 1), 0, (cat->max_subs - cat->num_subs) * sizeof(int)); // NULL check
}

void realloc_writes(struct cat *cat){
    cat->write_fds = (int *)realloc(cat->write_fds, cat->max_pubs*sizeof(int)); //NULL
    //memset(cat->write_fds + (cat->max_pubs - cat->num_pubs - 1), 0, (cat->max_pubs - cat->num_pubs) * sizeof(int)); // NULL
}

void *find_cat_by_name(list_t *head, char *name){
    list_t *cur = head;
    while (cur != NULL) {
        if(cur->cat == NULL){
            return NULL;
        }
        if (strcmp(cur->cat->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;  
}

void *find_cat_by_fd(list_t *head, int fd, int *witch){
    list_t *cur = head;
    int *cur_r = NULL;
    int *cur_w = NULL;

    while (cur != NULL) {
        if(cur->cat != NULL){
            if(cur->cat->read_fds != NULL && cur->cat->write_fds != NULL){
                cur_r = cur->cat->read_fds;
                cur_w = cur->cat->write_fds;
            }
            else{
                return NULL; 
            }     
        }
        else{
            return NULL;
        }

        for(int i = 0; i < cur->cat->max_subs; i++) {
            if(fd == *(cur_r + i)){
                *witch = 100;
                return cur;
            }
        }
        for(int i = 0; i < cur->cat->max_pubs; i++) {
            if(fd == *(cur_w + i)){
                *witch = 200;
                return cur;
            }
        }
        cur = cur->next;
    }
    return NULL;  
}

int remove_item(list_t * head, list_t *to_remove) {
    list_t *cur = head;
    list_t *last = NULL;
    if(head == to_remove){ // head problem TODO
        last = head;
        if(head->next != NULL){
            head = head->next;
        }
        else{
            head = NULL;
        }
        free(last->cat->read_fds);
        free(last->cat->write_fds);
        free(last->cat);
        free(last);
        return 0;
    }
    while(cur != to_remove) {
        last = cur;
        cur = cur->next;
    }

    last->next = cur->next;

    free(cur->cat->read_fds);
    free(cur->cat->write_fds);
    free(cur->cat);
    free(cur);
    return 0;
}
