
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "../include/linked_list.h"


int main(){

    list_t *head = (list_t *) malloc(sizeof(list_t));
    head->cat = (cat_t *) malloc(sizeof(cat_t));
    head->next = NULL;
    cat_init(head->cat, "head");
    
    cat_t *cat = (cat_t *) malloc(sizeof(cat_t));
    cat_init(cat, "node2");
    push(head, cat);
    
    cat = (cat_t *) malloc(sizeof(cat_t));
    cat_init(cat, "node3");
    push(head, cat);

    list_t *cur =  head;
    int j = 0;
    while(cur != NULL){
        int *r = cur->cat->read_fds;
        int *w = cur->cat->write_fds;
        for(int i = 0; i < 3; i++){
            if(i % 2 == 0){
                *(r + i) = i +j;
            }
            else{
            *(w + i) = i + j;
            }
        }
        cur->cat->max_pubs += 1;
        realloc_writes(cur->cat);
        cur->cat->max_subs -= 5;
        realloc_reads(cur->cat);

        j += 4;
        cur = cur->next;
        
    }

    list_t *ret = find_cat_by_name(head, "node3");
    remove_item(head, ret);

    int witch = 0;
    for(int i = 0; i < 17; i ++){
        witch = 0;
        find_cat_by_fd(head, i, &witch);
        printf("%d %d\n", i, witch);
    }
    
    cur = head;
   
    while(cur != NULL){
        printf("%s %d %d %d %d %d ", cur->cat->name, cur->cat->max_pubs, cur->cat->max_subs, cur->cat->max_size, cur->cat->num_pubs, cur->cat->num_subs);
        int *r = cur->cat->read_fds;
        int *w = cur->cat->write_fds;
        printf("[");
        for(int i = 0; i < cur->cat->max_pubs; i++){
            printf("%d ", *(w + i));
        }
        printf("][");
        for(int i = 0; i < cur->cat->max_subs; i++){
            printf("%d ", *(r + i));
        }
        printf("]\n");
        cur = cur->next;
    }
    
    
    free_all(head);
    return 0;
}

