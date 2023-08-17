/* tour.c
* To learn the basic signal handling in c
* Can send the interrupt signal via Ctrl-c in a terminal.
*
* Complete TODO items for this question
*/

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_FRIEND 5 // Number of friends visiting the Dinosaurs Park

int friend_count = 0; // Current count of friends completed the tour

// TODO: Add a signal handling function which handle SIGINT and SIGCHLD signals
void  SIGINT_handler(int sig)
{
    // printf("Interrupt called, friend_count %d\n", friend_count);
    printf("\n\nYou have interrupted the tour.\n");

    if(friend_count==5){
        printf("All 5 friends have completed the tours.\n");
        printf("Thank you for visiting the Dinosaurs park\n");
        exit(0);
    }
    else{
        printf("Oh Sorry! Only %d out of the 5 friends have completed the tour.\n", friend_count);
    }
}

void  SIGCHILD_handler(int sig)
{
    while(1){
        if(waitpid(-1,NULL,0)<0){
            break;
        }
        friend_count++;
    }
    
}
// END TODO

int main(int argc, char *argv[])
{

    printf("Welcome to the Dinosaurs Park.\n");

    /* 
    * TODO: add struct/calls to sigaction() to handle SIGINT and SIGCHLD. 
    * Use sigaction() and associated structs.
    * Ensure that the flag SA_RESTART is set as well to ensure system calls are automatically restarted.
    */
    struct sigaction sigint;
    sigint.sa_handler = SIGINT_handler;
    sigemptyset(&sigint.sa_mask);
    sigint.sa_flags=SA_RESTART;
    if(sigaction(SIGINT, &sigint, NULL)==-1){
        printf("Error handling SIGINT\n");
    };

    struct sigaction sigchild;
    sigchild.sa_handler = SIGCHILD_handler;
    sigemptyset(&sigchild.sa_mask);
    sigchild.sa_flags=SA_RESTART;
    if(sigaction(SIGCHLD, &sigchild, NULL)==-1){
        printf("Error handling SIGCHLD\n");
    };

    // END TODO
    printf("The Process ID of Dinosaurs Park: %d \n", getpid());
    for (size_t friends = 1; friends <= NUM_FRIEND; friends++)
    {
        if (fork() == 0)
        {
            //TODO Note that, you must carefully place the various children in different process groups 
            // that is different from the default process group of its parent
            //say, using the setpgid system call.

            if(setpgid(getpid(),0)!=0){
                printf("Error\n");
            }
            else{
                // printf("%d pid has grp %d\n",getpid(),getpgrp());
            }

            sleep(5 * friends);             // sleep emulates "touring" time
            printf("Friend #%zu with process ID - %d has completed the tour.\n", friends, getpid());
            exit(0);
            //END TODO
        }
    }

    for(;;)pause(); // wait for all friends to complete tour

    return 0;
}
