#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DEBUG 0
#define debug_print(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

typedef struct m {
    int totalFramesCount;
    int freeFramesCount;
    unsigned long* frame; // contains the id of the process and the page using that frame.
    // I make it as an array and each entry has two things ( pid and page number)
    bool* frameIsEmpty; // contains whether the frame is empty
    unsigned int maxFrameCount;
    unsigned int activeProcessCount;
    unsigned int maxActiveProcessCount;
    int* activeProcesses;
    int* processFrameCount;
} memory;


enum operation {
    START,
    REFERENCE,
    TERMINATE
};


int find_victim(memory* m, int* LRUCounter, int command_pid, bool local_replacement){
    ///start with invalid victimNum
    int victimNum = -1;
    int victimPri = INT_MAX;


    if (local_replacement) {
        // find process' id (id is value in activeProcesses)
        int currentProcessIdx = -1;
        for (int i = 0; i < m->activeProcessCount; i++) {
            if (m->activeProcesses[i] == command_pid) {
                currentProcessIdx = i;
                break;
            }
        }

        unsigned int victimPID = 0;
        // process has less than max frames or no frames
        if (m->processFrameCount[currentProcessIdx] < m->maxFrameCount
                || m->processFrameCount[currentProcessIdx] == 0) {
            // find process with most frames in memory
            int highestFrameCount = 0;
            int highestFrameIdx = -1;
            for (int i = 0; i < m->activeProcessCount; i++) {
                if (m->processFrameCount[i] > highestFrameCount) {
                    highestFrameCount = m->processFrameCount[i];
                    highestFrameIdx = i;
                }
            }

            victimPID = m->activeProcesses[highestFrameIdx];
        } else {
            // select least recently used frame from process pool
            victimPID = m->activeProcesses[currentProcessIdx];
        }

        for (int i = 0; i < m->totalFramesCount; i++) {
            if ((m->frame[i] >> 16) == victimPID
                    && LRUCounter[i] < victimPri) {
                victimNum = i;
                victimPri = LRUCounter[i];
            }
        }
    } else {
        // find the piece of memory that is the oldest.
        for (int i = 0; i < m->totalFramesCount; i++) {
            // if the last opNum of this memory is older,
            if (LRUCounter[i] < victimPri) {
                // then this is going to be our chosen memory,
                victimNum = i;
                victimPri = LRUCounter[i];
            }
        }
    }
    return victimNum;
}

void remove_idx(int* array, int idx, int size) {
    for (int i = idx; i < size - 1; i++) {
        array[i] = array[i+1];
    }
}

// checks if page is in memory
bool is_in_memory(memory *m, int pid, int pageNumber) {
    for(int i = 0; i < m->totalFramesCount; i++) {
        if(!m->frameIsEmpty[i] && pid == (m->frame[i] >> 16) && pageNumber == (m->frame[i] & 0xffff))
            return true;
    }
    return false;
}

// checks if there is any free memory available
bool has_free_memory(memory m) {
    return m.freeFramesCount > 0;
}

void remove_frames_for_process(memory* m, int pid) {
    for(int i = 0; i < m->totalFramesCount; i++) {
        if(pid == (m->frame[i] >> 16)) {
            m->frameIsEmpty[i] = true;
            m->freeFramesCount++;
        }
    }
    
    for (int i = 0; i < m->activeProcessCount; i++) {
        if (m->activeProcesses[i] == pid) {
            remove_idx(m->activeProcesses, i, m->activeProcessCount);
            remove_idx(m->processFrameCount, i, m->activeProcessCount);
            m->activeProcesses[m->activeProcessCount] = -1;
            m->processFrameCount[m->activeProcessCount] = 0;
            m->activeProcessCount--;
            break;
        }
    }
}

void remove_page_from_memory(memory *m, int pid, int pageNumber) {
    if(!is_in_memory(m, pid, pageNumber)) fprintf(stderr, "page to be removed not in memory\n");
    unsigned long val = 0;
    val = (val | pid) << 16;
    val = val | pageNumber;
    for(int i = 0; i < m->totalFramesCount; i++) {
        if(!m->frameIsEmpty[i] && m->frame[i] == val) {
            m->frameIsEmpty[i] = true;
            m->freeFramesCount++;
            if(is_in_memory(m, pid, pageNumber)) printf("Error removing page from memory\n");
            for (int j = 0; j < m->activeProcessCount; j++) {
                if (m->activeProcesses[j] == pid) {
                    m->processFrameCount[j]--;
                }
            }
            return;
        }
    }
}

//Returns the page's index in memory
int add_page_to_memory(memory* m, int pid, int page) {

    for (size_t frame = 0; frame < m->totalFramesCount; ++frame)
    {
        ///I put the pid AND the page number into the frames array
        ///So, each frame takes up two slots
        ///for example [pid, page, pid, page, pid, page]
        ///The reason that I cannot do it like key and value because the key is frame number
        // and the value will be (pid ,  page number ) -> this is not working at all
        /// this is the array of the frame | frame 1| |frame 2|  |frame 3 |

        //int frame_pid = m->frame[frame*2];
        //int frame_page = m->frame[(frame*2)+1];

        bool is_this_frame_free = m->frameIsEmpty[frame];

        if (is_this_frame_free)
        {
            ///fill this frame slot
            unsigned long val = 0;
            val = (val | pid) << 16;
            val = val | page;

            m->frame[frame] = val;

            ///mark the frame as full
            m->frameIsEmpty[frame] = false;

            m->freeFramesCount--;

            for (int i = 0; i < m->activeProcessCount; i++) {
                if (m->activeProcesses[i] == pid) {
                    m->processFrameCount[i]++;
                }
            }
            return frame;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int process;
    int memSize;
    int pageReplaceType = 0;
    int totalReferences = 0;
    int totalPageFaultCount = 0;
    int processReferences = 0;
    int processPageFaultCount = 0;

    char opt;
    // read parameters
    while ((opt = getopt(argc, argv, "p:s:r:")) != -1) {
        switch(opt) {
            case 'p':
                if (!sscanf(optarg, "%i", &process)) {
                    fprintf(stderr, "Invalid process id option.\n");
                    exit(1);
                }
                break;
            case 's':
                if (!sscanf(optarg, "%i", &memSize)) {
                    fprintf(stderr, "Invalid memory size option.\n");
                    exit(1);
                }
                break;
            case 'r':
                if (!sscanf(optarg, "%i", &pageReplaceType)) {
                    fprintf(stderr, "Invalid page replacement policy option.\n");
                    exit(1);
                }
                break;
            case '?':
                fprintf(stderr, "Unrecognized option -%c.\n", optopt);
                exit(1);
                break;
            case ':':
                fprintf(stderr, "The -%c option is missing an argument.\n",
                        optopt);
                exit(1);
                break;
            default:
                break;
        }
    }
    if (optind != 7) {
        fprintf(stderr, "Missing required arguments.\n");
        exit(1);
    }

    
    // initialize memory
    memory m;
    m.totalFramesCount = memSize;
    m.freeFramesCount = memSize;
    m.frame = malloc(sizeof(unsigned long) * memSize);
    m.frameIsEmpty = malloc(sizeof(bool) * memSize);
    m.activeProcessCount = 0;
    m.maxActiveProcessCount = memSize;
    m.activeProcesses = malloc(sizeof(int) * memSize);
    m.processFrameCount = malloc(sizeof(unsigned) * memSize);
    for (int i = 0; i < memSize; i++) {
        m.frameIsEmpty[i] = true;
        m.activeProcesses[i] = -1;
        m.processFrameCount[i] = 0;
    }

    // read input from stdin
    enum operation op;
    int pid;
    int pageNumber;
        
    int *LRUCounter;
    LRUCounter = malloc(sizeof(int)*memSize);
    memset(LRUCounter, 0, sizeof(*LRUCounter));
    
    int opNum = 0;

    ///START <PID> <ADDRESSSPACESIZE>
    ///TERMINATE <PID>

    while (true)
    {
        // fprintf(stderr,"m.freeFramesCount: %u, opNum: %u\n",m.freeFramesCount,opNum);

        char opstr[10]; // each word at most 10 characters 
        int arg1 = 0, arg2 = 0;
        int num_args = fscanf(stdin, "%9s %d %d\n", opstr, &arg1, &arg2);

        if (num_args == EOF)
            break;

        ///not parse because it's empty
        if (num_args == 0)
            ///skip
            continue;

        if (strcmp(opstr, "START") == 0)
        {
            op = START;
            pid = arg1;
            
            ///Here START command 
            /// just debugs
            // fprintf(stdout, "START, pid: %d, addressSpaceSize: %d\n", pid, addressSpaceSize);

        } else if (strcmp(opstr, "TERMINATE") == 0) {
            op = TERMINATE;
            pid = arg1;


            ///Here Terminals command 
            /// just debugs
            // fprintf(stdout, "TERMINATE, pid: %d\n", pid);
        } else if (strcmp(opstr, "REFERENCE") == 0) {
            op = REFERENCE;
            pid = arg1;
            pageNumber = arg2;

            ///Here Reerence command 
            /// just debugs
            // fprintf(stdout, "REFERENCE, pid: %d, page: %d\n", pid, pageNumber);


        } else {
            fprintf(stderr, "Invalid command\n%9s", opstr);
        }

        switch(op) {
            case START:
                if (m.maxActiveProcessCount == m.activeProcessCount) {
                    m.maxActiveProcessCount *= 2;
                    m.activeProcesses = realloc(m.activeProcesses, m.maxActiveProcessCount * sizeof(int));
                    m.processFrameCount = realloc(m.processFrameCount, m.maxActiveProcessCount * sizeof(unsigned));
                }
                m.activeProcesses[m.activeProcessCount] = pid;
                m.processFrameCount[m.activeProcessCount] = 0;
                m.activeProcessCount++;
                m.maxFrameCount = m.totalFramesCount / m.activeProcessCount;
                debug_print("start %i\n", pid);
                debug_print("max frame count: %i\n", m.maxFrameCount);
                break;

            case REFERENCE:
                if(!is_in_memory(&m, pid, pageNumber)) {
                    if(has_free_memory(m)) {
                        int index = add_page_to_memory(&m, pid, pageNumber);
                        LRUCounter[index] = opNum;
                        //else Update counter
                    } else { 
                        if(pid == process) processPageFaultCount++;
                        totalPageFaultCount++;
                        
                        int victimNum = find_victim(&m, LRUCounter, pid, /*local_replacement=*/pageReplaceType==1);

                        if (victimNum < 0)
                        {
                            fprintf(stderr, "Cannot do local replacement, process has no existing memory, aborting; pid: %u\n",pid);
                            exit(-1);
                        }
                        unsigned int victimPage = m.frame[victimNum] & 0xffff;
                        unsigned int victimPid = m.frame[victimNum] >> 16;
                        debug_print("removing pid: %d page: %d\n", victimPid, victimPage);
                        remove_page_from_memory(&m, victimPid, victimPage);
                        add_page_to_memory(&m, pid, pageNumber);
                        if(is_in_memory(&m, pid, pageNumber))
                            debug_print("adding pid: %d page: %d\n", pid, pageNumber);
                        for(int i = 0; i < memSize; i++)
                            debug_print("frame %d: %8lX %d\n", i, m.frame[i], LRUCounter[i]);
                        LRUCounter[victimNum] = opNum;
                    }
                }
                if(pid == process) processReferences++;
                totalReferences++;
                break;

            case TERMINATE:
                remove_frames_for_process(&m, pid);
                debug_print("removed %i\n", pid);
                break;
        }
        opNum++;
    }

    free(m.frame);
    free(m.frameIsEmpty);
    free(m.activeProcesses);
    free(m.processFrameCount);

    fprintf(stdout, "Total page faults: %i.\nProcess page faults: %i.\n",
            totalPageFaultCount, processPageFaultCount);
    fprintf(stdout, "Total page fault rate: %f.\nProcess page fault rate: %f.\n",
            (totalPageFaultCount * 1.0) / totalReferences, 
            (processPageFaultCount * 1.0) / processReferences);

    return 0;
}
