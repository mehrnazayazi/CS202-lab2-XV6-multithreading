#include "types.h"
#include "stat.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int pid;
int global = 1;
lock_t lk;
int totalpass = 20;
int passcount = 0;
int repeats = 5;

void
Frisbee(void * arg_ptr){
    int id = *(int*)arg_ptr;
    while(passcount < totalpass){
        lock_acquire(&lk);
        if (passcount < totalpass && passcount%repeats==id){
            printf(1, "Pass numer no: %d, Thread %d is passing the token to thread %d\n", ++passcount, id, id+1);
        }
        lock_release(&lk);
    }
    exit() ;

}


int
main(int argc, char* argv[])
{
    pid = getpid();
    lock_init(&lk);
    int i=0;
    int id[repeats];
    int localrepeats = repeats;
    for(i=0; i<localrepeats; i++){
        id[i] = i;
        int thread_pid = thread_create(Frisbee, &id[i], PGSIZE*2);
        printf(1, "pid of thread created = %d\n", thread_pid);

    }
    for(i=0; i<localrepeats; i++){
        int join_pid = thread_release();
        printf(1, "pid of thread released = %d\n", join_pid);
    }
    printf(1, "Simulation of Frisbee game has finished, %d rounds were played in total!\n", totalpass/repeats);
    exit();

}

