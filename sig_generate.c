#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
    // printf("%d",argc);
    if(argc<2){
        printf("Provide pid");
    }
    else{
        int pid=atoi(argv[1]);
        sleep(1);
        
        if(kill(pid,SIGINT)>=0){
            printf("SIGINT signal sent to PID: %d\n",pid);
        }
        else{
            printf("SIGINT error\n");
        }
        sleep(1);
        
        if(kill(pid,SIGTERM)>=0){
            printf("SIGTERM signal sent to PID: %d\n",pid);
        }
        else{
            printf("SIGTERM error\n");
        }
        sleep(1);
        
        if(kill(pid,9)>=0){
            printf("SIGKILL signal sent to PID: %d\n",pid);
        }
        else{
            printf("SIGKILL error\n");
        }
    }
}