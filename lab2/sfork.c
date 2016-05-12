#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// I hardcoding the maximun children that i need 

#define MAXIMUM_CHILD_PROCESSES 3



// Here i just print usage statment 

void print_usage()
{
  fprintf(stderr, "%s\n", "Usage: Lab2 : [-u] [-N <num-levels>] [-M <num-children>] [-p] [-s <sleep-time>]");
  exit(0);
}



// here i am using print depth function to know the depth for each tree 

void print_depth(int depth)
{  
    int i;
    for (i = 0; i < depth; ++i)
        fputc(' ', stdout);
}


///declaration for create tree function 

void create_tree(int num_children, int num_levels, bool has_pause, int sleep_time, pid_t parent_pid, int sibling_index, int depth);



void create_tree(int num_children, int num_levels, bool has_pause, int sleep_time, pid_t parent_pid, int sibling_index, int depth)
{
    ///make sure the num_children is in the correct range, for sanity.
    assert(num_children <= MAXIMUM_CHILD_PROCESSES);
    
    pid_t current_pid = getpid();
    
    
    
    
    fprintf(stdout, "ALIVE: Level %d process with pid=%d, child of ppid=%d\n", num_levels - 1, current_pid, getppid());
    
    ///if we are asking for a tree of 0 levels, then the current process IS that tree, and we do not need any children, so end now.

    if (num_levels < 1)
    {
        fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d\n", num_levels - 1, current_pid, getppid());
        return;
    }
    
    ///we make an array to store the M child process's process-ids.

    pid_t child_pids[MAXIMUM_CHILD_PROCESSES];
    
    ///initialize the pids
    int i;
    for ( i = 0; i < MAXIMUM_CHILD_PROCESSES; ++i)
        child_pids[i] = 0;
    
    
    
    
    ///for each child that needs to be constructed,         
    for (i = 0; i < num_children; ++i)
    {
        ///we fork the program.
        pid_t pid = fork();
        
        
        ///forking returns -1 if there is an error

        if (pid < 0)
        {
            fprintf(stderr, "Fork failed.");
            exit(-1);
        }
        
        
        ///if the pid is not 0, then the current process fork is the parent fork.
        bool is_parent = pid != 0;
        
        ///if the pid IS 0, then we are currently in the child process fork of the fork.
        
        bool is_child = pid == 0;
        
        ///if there exists a child, then we calculate how many levels the child still needs to
        /// create under itself.

        int child_num_levels = num_levels - 1;
        
        ///next, if the number of levels that the child must create under itself is 0,
        /// then the child is a leaf.
        
        bool child_is_leaf = child_num_levels == 0;
        
        if (is_child)
        {
            ///here we diverge execution depending on if this process fork is a child
            /// or if it is the parent.
            
        
            if (!child_is_leaf)
                        
                create_tree(num_children, child_num_levels, has_pause, sleep_time, current_pid, i, depth+1);
            
            if (child_is_leaf)
            {
                ///then just execute the prescribed assignment, which is to pause or sleep
                
                
                ///pause or sleep

                if (has_pause)

                    pause();

                else
                    
                    sleep(sleep_time);
                    
            }
            
            return;
        }
        
        
        
        ///we store the child's pid, so that we can later wait for it to end.
        child_pids[i] = pid;
        
        
        ///the parent just continues and forks the next ones
    }
    
    if (!has_pause)
    {
    int i;
        for (i = 0; i < num_children; ++i)
        {
            pid_t pid = child_pids[i];
            
            int status;
            (void)waitpid(pid, &status, 0);
            
            ///if the child somehow exited with an error
            if (status != 0)
                exit(status);
        }
    } else {

    }
    
    fprintf(stdout, "EXITING: Level %d process with pid=%d, child of ppid=%d\n", num_levels - 1, current_pid, getppid());
}

bool arg_is_not_zero(const char* arg)
{
    const char* zero = "0";
    return memcmp(arg, zero, 2) != 0;
}


int main(int argc, char* argv[])
{
  
  
  
    bool print_usage_and_exit = false;
    bool has_pause = false;
    bool has_num_levels = false;
    int num_levels = 0;
    bool has_num_children = false;
    int num_children = 1;
    bool has_sleep_time = false;
    int sleep_time = 1;
    
    int i;
    for (i = 1; i < argc; ++i)
    {
        const char* argi = argv[i];
          
        char dash = argi[0];
        
        
        if (strlen(argi) != 2 || dash != '-')
        {
            print_usage();
            exit(-1);
        }
        
        
        char flag = argi[1];
        
        if (flag == 'u')
        {
            if (print_usage_and_exit) 
            {
            
                ///Example: ./Lab1 -u -u
                print_usage();
                exit(-1);
            }

            print_usage_and_exit = true;
            continue;
        }
        
        if (flag == 'p')
        {
            if (has_pause) {
            
                ///Example: ./Lab1 -p -p
                print_usage();
                exit(-1);
            }
            has_pause = true;
            continue;
        }
            
        
        if (!(i + 1 < argc))
        {
            ///Example: ./Lab1 -m
            print_usage();
            exit(-1);
        }
        
        
        if (flag == 'N')
        {
            if (has_num_levels) 
            {
            
                ///Example: ./Lab1 -N <num-levels> -N <num-levels> 
                print_usage();
                exit(-1);
            }
            
            has_num_levels = true;
            
            
            const char* next_arg_i = argv[i+1];
            
            num_levels = atoi(next_arg_i);
            
            if (num_levels == 0 && arg_is_not_zero(next_arg_i))
            {
                ///Example: ./Lab1 -N NOTANUMBER
                print_usage();
                exit(-1);
            }
            
            if (num_levels > 4)
            {
                fprintf(stderr, "<num-levels> should not be greater than 4\n");
                exit(-1);
            }
            
            i++;
            continue;
        }
        
        
        if (flag == 'M')
        {
            if (has_num_children) {
            
                ///Example: ./Lab1 -M <num-children> -M <num-children> 
                print_usage();
                exit(-1);
            }
            has_num_children = true;
            
            
            const char* next_arg_i = argv[i+1];
            num_children = atoi(next_arg_i);
            
            if (num_children == 0 && arg_is_not_zero(next_arg_i))
            {
                ///Example: ./Lab1 -M NOTANUMBER
                print_usage();
                exit(-1);
            }
            if (num_children > 3)
            {
                fprintf(stderr, "<num-children> should not be greater than 3\n");
                exit(-1);
            }
            
            i++;
            continue;
        }
        
        
        if (flag == 's')
        {
            if (has_sleep_time) {
            
                ///Example: ./Lab1 -s <sleep-time> -s <sleep-time> 
                print_usage();
                exit(-1);
            }
            has_sleep_time = true;
            
            
            const char* next_arg_i = argv[i+1];
            sleep_time = atoi(next_arg_i);
            
            if (sleep_time == 0 && arg_is_not_zero(next_arg_i))
            {
                ///Example: ./Lab1 -s NOTANUMBER
                print_usage();
                exit(-1);
            }
            
            i++;
            continue;
        }
        
    }
    
    if (print_usage_and_exit)
    {
        print_usage();
        exit(-1);
    }
    
    
    if (has_pause && has_sleep_time)
    {
        fprintf(stderr, "Cannot specify -p and -s\n");
        print_usage();
        exit(-1);
    }

    create_tree(num_children, num_levels, has_pause, sleep_time,0,0,0);
  
  
  
  
    return 0;
}
