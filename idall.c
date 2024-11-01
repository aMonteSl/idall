#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>



void executeId(int argc, char** argv){
    int i;
    int pidchild;


    for (i = 0; i < argc; i++)  {
        switch (pidchild = fork())
        {
        case -1:
            err(EXIT_FAILURE, "fork failed");
            break;

        case 0:
            execl("/usr/bin/id", "id", argv[i], NULL);
            err(EXIT_FAILURE, "can`t exec id");
        }

    }


}

int waitChilds(int numberchilds){
    int sts;
    int pidwait;

    do {
        pidwait = wait(&sts);
        
        if (pidwait != -1){
            numberchilds--;
            
            if (WIFEXITED(sts) && WEXITSTATUS(sts) != 0){
                return -1;
            }
        }
        
    } while (numberchilds > 0 && pidwait != -1);

    return 0;
}

int main(int argc, char* argv[]){
    int err;

    argc--;
    argv++;

    if (argc < 1){
        fprintf(stderr, "usage: idall user [user]...\n");
        exit(EXIT_FAILURE);
    }

    executeId(argc, argv);
    err = waitChilds(argc);

    if (err){
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}