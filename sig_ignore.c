#include<stdio.h>
#include<unistd.h>
#include<signal.h>
int main(){
    printf("Process Id is: %d\n", getpid());
    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    while(1){
        printf("Waiting...\n");
        sleep(3);
    }
}