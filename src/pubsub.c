#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/pubsub.h"


int num_subs = 0;
int num_pubs = 0;
int read_fds[MAX_NUM_SUBS];
int write_fds[MAX_NUM_PUBS];
int max_subs = MAX_NUM_SUBS;
int max_pubs = MAX_NUM_SUBS;
int max_size = MAX_MSG_SIZE;

int pubsub_open(char* p_category, int p_options, int p_mode){
    int ret;
    if(p_options == O_RDONLY){
        if(num_subs < MAX_NUM_SUBS ){
            ret = open(p_category, p_options, p_mode);
            if(ret != -1){
                num_subs++;
                read_fds[num_subs] = ret;
            }
            else {
                return -1;
            }
            
        }
        else{
            return -1;
        }  
    }
    else if(p_options == O_WRONLY){
            if(num_pubs < MAX_NUM_PUBS ){
                ret = open(p_category, p_options, p_mode);
                if(ret != -1){
                    num_pubs++;
                    write_fds[num_pubs] = ret;
                }
                else {
                    return -1;
                }
            }
            else{
                return -1;
            }  
    }
    else 
    {
        return -1;
    }
    
    return ret;

}

int pubsub_read(int p_fd, char* p_message, int p_size){

    if(p_size < 1 || p_size > MAX_MSG_SIZE){
        return -1;
    }
    int size = read(p_fd, p_message, p_size);

    if(size != p_size){
        return -1;
    }
    return size;
}

int pubsub_write(int p_fd, char* p_message, int p_size){

    if(p_size < 1 || p_size > MAX_MSG_SIZE){
        return -1;
    }
    if((int)(strlen(p_message) + 1) != p_size){
        return -1;
    }
    return write(p_fd, p_message, p_size);
}

int pubsub_close(int p_fd){

    for(int i = 0; i < MAX_NUM_SUBS; i++){
        if(p_fd == read_fds[i]){
            num_subs--;
            return close(p_fd); 
        }
    }
    for(int i = 0; i < MAX_NUM_PUBS; i++){
        if(p_fd == write_fds[i]){
            num_pubs--;
            return close(p_fd); 
        }
    }
    return close(p_fd);

}

int pubsub_ioctl(int p_fd, int p_request, int p_options){

    if(p_options <= 0){
        return -1;
    }
    if(p_request != MAX_MSG_SIZE && p_request != MAX_NUM_PUBS && p_request != MAX_NUM_SUBS){
        return -1;
    }
    if(p_request == MAX_MSG_SIZE){
        max_size = p_options;
        return 0;
    }
    else if(p_request == MAX_NUM_PUBS){
        max_pubs = p_options;
        return 0;
    }
    else if(p_request == MAX_NUM_SUBS){
        max_subs = p_options;
        return 0;
    }
    else{
        return -1;
    }   
}

void pubsub_reset(){
    num_subs = 0;
    num_pubs = 0;
    for(int i = 0; i < MAX_NUM_SUBS; i++){
        read_fds[i] = 0;
    }
    for(int i = 0; i < MAX_NUM_PUBS; i++){
        write_fds[i] = 0;
    }
}