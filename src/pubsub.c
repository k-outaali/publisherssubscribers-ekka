#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/pubsub.h"


int num_subs = 0;
int num_pubs = 0;

int pubsub_open(char* p_category, int p_options, int p_mode){

    if(p_options == O_RDONLY){
        if(num_subs < MAX_NUM_SUBS ){
            num_subs++;
        }
        else{
            return -1;
        }  
    }
    else if(p_options == O_WRONLY){
            if(num_pubs < MAX_NUM_PUBS ){
                num_pubs++;
            }
            else{
                return -1;
            }  
    }
    else 
    {
        return -1;
    }
    
    return open(p_category, p_options, p_mode);

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

    num_subs--;
    return close(p_fd);

}

int pubsub_ioctl(int p_fd, int p_request, int p_options){

    return -1;
}