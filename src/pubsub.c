#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/pubsub.h"


int num_subs = 0;
int num_pubs = 0;
int read_fds[MAX_NUM_SUBS];
int write_fds[MAX_NUM_PUBS];

int err[] = {150, 151, 152, 153, 154, 155};
char *serr[] = {
 "ERR MAX NB SUBS",
 "ERR MAX NB PUBS", 
 "ERR NOT READ OR WRITE ONLY (wrong p_option)", 
 "ERR dans READ p_size < 1 ou >= 1024",
 "ERR dans WRITE p_size != strlen(p_message) + 1",
 "ERR IOCTL (always return -1)"
};

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
            errno = 150;
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
                errno = 151;
                return -1;
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

    if(p_size < 1 || p_size > MAX_MSG_SIZE){
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

    if(p_size < 1 || p_size > MAX_MSG_SIZE){
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

    errno = 155;
    return -1;
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


char *pubsub_get_error(){
    char *ret = strerror(errno);
    if( strstr(ret, "Unknown error") != NULL){
        for(int i = 0; i < sizeof(err)/sizeof(int); i++){
            if(err[i] == errno){
                return serr[i];
            }
        }
    }
    else{
        return ret;
    }
}
