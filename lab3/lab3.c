#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>


uint32_t mylevel = 0;
uint32_t myindex = 0;

void print_usage(FILE* fp)
{
    fprintf(fp, "./lab3 <num-procs>\n");
}

void signal_handler(int sig)
{
  
    fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d\n", mylevel, getpid(), getppid());


    exit(0);
}


int main(int argc, char* argv[])
{
  
    signal(SIGUSR1, signal_handler);
 
    if (argc != 2)
    {
        print_usage(stderr);
        exit(-1);
    }
    
    
    ///turn arg 1 into num_procs
    int num_procs = atoi(argv[1]);
    
    if (num_procs < 1 || num_procs > 32)
    {
        ///"where <num-procs> will be a well-formed integer between 1 and 32, inclusive."
        fprintf(stderr, "num-procs must be between 1 and 32 inclusive\n");
        print_usage(stderr);
        exit(-1);
    }
    
    ///calculate how much shared memory we will need
    int memory_required = num_procs*sizeof(pid_t);
    
    int shmid = shmget(IPC_PRIVATE, memory_required, IPC_CREAT | 0666);
    
    
    ///check for errors
    if(shmid < 0)
    {
        fprintf(stderr, "error obtaining shared memory!\n");
        exit(-1);
    }
    
    ///get a pointer to the shared memory, via the shmid handle
    uint8_t* shm = shmat(shmid,NULL,0);

    ///check if there is an error retrieving the pointer to the shared memory
    if (shm == (uint8_t*)-1)
    {
        fprintf(stderr, "error obtaining shared memory!\n");
        exit(-1);
    }
    
    pid_t* pids = (pid_t*)shm;
    
    
    ///the root level is the height of the "linear tree" (i.e the list) of processes
    int root_level = num_procs;

    int root_index = num_procs - root_level;
    assert(root_index == 0);
    
    mylevel = root_level;
    ///same for index.
    myindex = root_index;
    
    pids[root_index] = getpid();
    
    fprintf(stdout, "ROOT!\n");

    fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d\n", root_level, getpid(), getppid());
    

    fflush(stdout);
    
    ///prepare the named pipe file
    const char* myfifo = "/tmp/lab3-piper-donepipe";
    mkfifo(myfifo, 0666);
    
    ///this loop does the actual forking
    while (mylevel > 1)
    {
        ///Create an unnamed pipe:
        ///this is how you create an unnamed pipe
        int unnamed[2]; pipe ( unnamed ); 
        
        fflush(stdout);

        ///now just do the fork
        pid_t childpid = fork();
        
        ///if there is an error, just stop everything now.
        if (childpid < 0)
        {
            fprintf(stderr, "Fork failed\n");
            exit(-1);
        }
        
        
        bool is_parent = childpid != 0;
        bool is_child = childpid == 0;
        
        
        if (is_parent) {
            
            uint32_t child_level = mylevel - 1;
            
            ///send the child's level over the unnamed pipe
            write(unnamed[1], &child_level, sizeof(child_level));
            close(unnamed[0]);
            close(unnamed[1]);
            
            break;
        }
        ///this only happens for the child
        assert(is_child);
        
        pid_t mypid = getpid();
        

            ///retrieve mylevel from the parent, via the unnamed pipe
            int bytes_read = read(unnamed[0], &mylevel, sizeof(mylevel));
            assert(bytes_read == sizeof(mylevel));
            close(unnamed[0]);
            close(unnamed[1]);
            
            
            myindex = num_procs - mylevel;

        fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d\n", mylevel, mypid, getppid());
        fflush(stdout);
        

            
            fprintf(stderr, "mypid: %u\n", mypid);
            assert(myindex < num_procs);
            ///"Each process should write its own process id into the shared memory object,
            /// which should be used as an array of integers"
            pids[myindex] = mypid;
        
        bool is_leaf = mylevel == 1;
        
        if (is_leaf)
        {
            break;
        }
        
        ///anything executing here is NOT a leaf
        assert(!is_leaf);
        
        ///continue the loop and create another process, because we are NOT the leaf
    }

    bool is_root = mylevel == root_level;
    bool is_leaf = mylevel == 1;
    
    if (is_root && is_leaf)
    {
            size_t i;
            for ( i = 0; i < num_procs; ++i)
            {
                fprintf(stdout, "%u\n", pids[i]);
            }

        ///now kill the root itself
        fprintf(stdout, "KILLING ROOT: %u, mylevel: %u\n", pids[0], mylevel);
        kill(pids[0], SIGUSR1);
    }
    
    if (is_leaf)
    {
        ///SCAFFOLDING:
        fprintf(stdout, "Leaf is opening named pipe\n");
        fflush(stdout);
        
        ///notify the root via the named pipe, that we are done, because this is the leaf.
        int donepipe = open ( myfifo, O_WRONLY );
        
        ///SCAFFOLDING:
        fprintf(stdout, "Leaf is writing to named pipe\n");
        fflush(stdout);
        ///write the letter "d" (for "done")
        write(donepipe, "d", 1);
        
        
    }
    
    if (is_root)
    {
        ///receive notification from leaf via the named pipe
        int donepipe = open ( myfifo, O_RDONLY );
        
        fprintf(stdout, "Root is reading from named pipe\n");
        
        fflush(stdout);
        
        char d[1];
        ///wait until the letter 'd' comes through the named pipe.
        int bytes_read = read( donepipe, d, 1 );
        
        if (bytes_read != 1 || d[0] != 'd')
        {
            fprintf(stderr, "unexpected error while reading from the donepipe; bytes_read: %i\n", bytes_read);
            exit(-1);
        }
            size_t i;
            for ( i = 0; i < num_procs; ++i)
            {
                uint32_t level = num_procs - i;
                fprintf(stdout, "pid for level %u is: %u\n", level, pids[i]);
            }
            for ( i = 1; i < num_procs; ++i)
            {
                uint32_t level = num_procs - i;
                fprintf(stdout, "KILLING: %u, level: %u\n", pids[i], level);
                kill(pids[i], SIGUSR1);
            }
          
        ///now kill the root itself
        fprintf(stdout, "KILLING ROOT: %u, mylevel: %u\n", pids[0], mylevel);
        kill(pids[0], SIGUSR1);
        
    }
    ///SCAFFOLDING:
    fprintf(stdout, "PAUSING: Level %d process with pid=%d, child of ppid=%d\n", mylevel, getpid(), getppid());
    fflush(stdout);
    ///pause forever; the process will be killed via the signal handler on the top of the page
    pause();
    
    return 0;
}