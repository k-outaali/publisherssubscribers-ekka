#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/pubsub.h"
#include "../include/linked_list.h"


int err[] = {150, 151, 152, 153, 154, 155, 156, 157};
char *serr[] = {
 "ERR MAX NB SUBS",
 "ERR MAX NB PUBS", 
 "ERR NOT READ OR WRITE ONLY (wrong p_option)", 
 "ERR dans READ p_size < 1 ou >= 1024",
 "ERR dans WRITE p_size != strlen(p_message) + 1",
 "ERR called ioctl with p_option value less than 1"
 "ERR called ioctl after opening a file only by subscribers"
 "ERR called ioctl with p_request not in {max_subs, max_pubs, max_size}"
};
list_t *head;

int pubsub_open(char* p_category, int p_options, int p_mode){
    int ret;
    if(p_options == O_RDONLY){
        //if the file is already open
        if(head == NULL){
            head = (list_t *) malloc(sizeof(list_t));
            if(head == NULL){
                return 999;
            }
        }
        list_t *list = find_cat_by_name(head, p_category);
        if(list != NULL){
            if(list->cat->num_subs < list->cat->max_subs ){
                ret = open(p_category, p_options, p_mode);
                if(ret != -1){
                    list->cat->read_fds[list->cat->num_subs] = ret;
                    list->cat->num_subs++;
                }
                else {
                    return -1;
                }
            }
            else{
                errno = 150;
                return -1;
            }
        }
        else{
            struct cat *cat;
            cat = (struct cat *) malloc(sizeof(struct cat)); // NULL not checked
            cat_init(cat, p_category);
            if(cat->num_subs < cat->max_subs ){
                ret = open(p_category, p_options, p_mode);
                if(ret != -1){
                    cat->read_fds[cat->num_subs] = ret;
                    cat->num_subs++;
                }
                else {
                    return -1;
                }
            }
            else{
                errno = 150;
                return -1;
            }
            push(head, cat);
        }
    }
    else if(p_options == O_WRONLY){
        if(head == NULL){
            head = (list_t *) malloc(sizeof(list_t)); // NULL not checked
            struct cat *cat;
            cat = (struct cat *) malloc(sizeof(struct cat)); // NULL not checked
            cat_init(cat, p_category);
            if(cat->num_pubs < cat->max_pubs ){
                ret = open(p_category, p_options, p_mode);
                if(ret != -1){
                    cat->read_fds[cat->num_pubs] = ret;
                    cat->num_pubs++;
                }
                else {
                    return -1;
                }
            }
            else{
                errno = 151;
                return -1;
            }
            push(head, cat);
        }
        list_t *list = find_cat_by_name(head, p_category);
        if(list == NULL){
            return 999;
        }
        if(list != NULL){
            if(list->cat->num_pubs < list->cat->max_pubs ){
                ret = open(p_category, p_options, p_mode);
                if(ret != -1){
                    list->cat->write_fds[list->cat->num_pubs] = ret;
                    list->cat->num_pubs++;
                }
                else {
                    return -1;
                }
            }
            else{
                errno = 151;
                return -1;
            }
        }
        else{
            struct cat *cat;
            cat = (struct cat *) malloc(sizeof(struct cat)); // NULL not checked
            cat_init(cat, p_category);
            if(cat->num_pubs < cat->max_pubs ){
                ret = open(p_category, p_options, p_mode);
                if(ret != -1){
                    cat->read_fds[cat->num_pubs] = ret;
                    cat->num_pubs++;
                }
                else {
                    return -1;
                }
            }
            else{
                errno = 151;
                return -1;
            }
            if(head == NULL){
                head = (list_t *) malloc(sizeof(list_t)); // NULL not checked
            }
            push(head, cat);
        }  
    }
    else 
    {
        errno = 152;
        return -1;
    }
    
    return ret;

}

int pubsub_read(int p_fd, char* p_message, int p_size){

    int witch;
    list_t *ret = find_cat_by_fd(head, p_fd, &witch); 
    if(ret == NULL || ret->cat == NULL){
        return read(p_fd, p_message, p_size);
    }
    if(p_size < 1 || p_size > ret->cat->max_size){
        errno = 153;
        return -1;
    }
    int size = read(p_fd, p_message, p_size);

    if(size != p_size){
        return -1;
    }
    return size;
}

int pubsub_write(int p_fd, char* p_message, int p_size){

    int witch;
    list_t *ret = find_cat_by_fd(head, p_fd, &witch);
    if(ret == NULL || ret->cat == NULL){
        return write(p_fd, p_message, p_size);
    }
    if(p_size < 1 || p_size > ret->cat->max_size){
        errno = 153;
        return -1;
    }
    if((int)(strlen(p_message) + 1) != p_size){
        errno = 154;
        return -1;
    }
    return write(p_fd, p_message, p_size);
}

int pubsub_close(int p_fd){

    int witch = 0;
    list_t *ret = find_cat_by_fd(head, p_fd, &witch);
    if(ret == NULL || ret->cat == NULL){
        return close(p_fd);
    }
    int *cur = ret->cat->read_fds;
    if(witch == 100){
        for(int i = 0; i < ret->cat->max_subs; i++){
            if(p_fd == *(cur + i)){
                *(cur + i) = 0;
            }
        }
        ret->cat->num_subs--;
        /*
        for(int i = 0; i < ret->cat->max_subs; i++){
            if(0 != *(cur + i)){
                return 0;
            }
        }
        cur = ret->cat->write_fds;
        for(int i = 0; i < ret->cat->max_pubs; i++){
            if(0 != *(cur + i)){
                return 0;
            }
        }
        remove_item(head, ret);
        */
    }
    else if (witch == 200){
        cur = ret->cat->write_fds;
        for(int i = 0; i < ret->cat->max_pubs; i++){
            if(p_fd == *(cur + i)){
                *(cur + i) = 0;
            }
        }
        ret->cat->num_pubs--;
        /*
        for(int i = 0; i < ret->cat->max_pubs; i++){
            if(0 != *(cur + i)){
                return 0;
            }
        }
        cur = ret->cat->read_fds;
        for(int i = 0; i < ret->cat->max_subs; i++){
            if(0 != *(cur + i)){
                return 0;
            }
        }
        remove_item(head, ret); 
        */

    }
    return close(p_fd);

}

int pubsub_ioctl(int p_fd, int p_request, int p_options){

    int witch = 0;
    if(p_options <= 0){
        errno = 155;
        return -1;
    }
    list_t *ret = find_cat_by_fd(head, p_fd, &witch);
    if(ret == NULL){
        return close(p_fd);
    }
    if(witch == 100){
        errno = 156;
        return -1;
    }
    if(p_request == PS_MSG && witch == 200){
        ret->cat->max_size = p_options;
        return 0;
    }
    else if(p_request == PS_PUBS && witch == 200){
        ret->cat->max_pubs = p_options;
        realloc_writes(ret->cat);
        return 0;
    }
    else if(p_request ==PS_SUBS && witch == 200){
        ret->cat->max_subs = p_options;
        realloc_reads(ret->cat);
        return 0;
    }
    else{
        errno = 157;
        return -1;
    }   
}

void pubsub_reset(){
    
    free_all(head);
}

char *pubsub_get_error(){
    char *ret = strerror(errno);
    if( strstr(ret, "Unknown error") != NULL){
        for(int i = 0; i < (int)(sizeof(err)/sizeof(int)); i++){
            if(err[i] == errno){
                return serr[i];
            }
        }
    }   
    return ret;
}
