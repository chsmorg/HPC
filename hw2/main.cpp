#include <iostream>
#include "hw2.h"
using namespace std::chrono;

int main(int argc, char **argv) {
    int N = atoi(argv[2]);
    int gens = atoi(argv[3]);
    int P = atoi(argv[4]);


    FILE *fp;
    fp = fopen(argv[1],"r");
    if(!fp){
        printf("File not found\n");
        exit(0);
    }

    std::vector<Displacement*> vec1 = readDisplacementData(fp,N);
    std::vector<std::vector<Displacement*>> vec2 = convertToTree(vec1, N);



    int shmId; 			        // ID of shared memory segment
    key_t shmKey = 123460; 		// key to pass to shmget(), key_t is an IPC key type defined in sys/types
    int shmFlag = IPC_CREAT | 0666;
    Displacement * shm;

    if ((shmId = shmget(shmKey, N*7*sizeof(Displacement)+ N + N*gens*sizeof(Displacement), shmFlag)) < 0)
    {
        std::cerr << "Init: Failed to initialize shared memory (" << shmId << ")" << std::endl;
        exit(1);
    }
    if ((shm = (Displacement *)shmat(shmId, NULL, 0)) == (Displacement *) -1)
    {
        std::cerr << "Init: Failed to attach shared memory (" << shmId << ")" << std::endl;
        exit(1);
    }


    Displacement ** tree = convertToList(vec1,N);
    std::vector<std::pair<int,int>> blocks = createBlocksList(tree,N,P);

    storeInShm(tree,N,shm);
//    auto start = high_resolution_clock::now();
//    pid_t pid;
//    pid = fork();
//    if(pid<0){
//        exit(1);
//    }
//
//    if(pid>0){
//        blockedGrow(shm,gens,blocks[0],N);
//    }
//    if(pid==0){
//        blockedGrow(shm,gens,blocks[1],N);
//    }
//
//    int status;    // catch the status of the child
//    do {// in reality, mulptiple signals or exit status could come from the child
//        pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
//        if (w == -1) {
//            // std::cerr << "Error waiting for child process ("<< pid <<")" << std::endl;
//            break;
//        }
//        if (WIFEXITED(status)) {
//            if (status > 0) {
//                std::cerr << "Child process (" << pid << ") exited with non-zero status of " << WEXITSTATUS(status)
//                          << std::endl;
//                continue;
//            } else {
//                std::cout << "Child process (" << pid << ") exited with status of " << WEXITSTATUS(status)
//                          << std::endl;
//                continue;
//            }
//        } else if (WIFSIGNALED(status)) {
//            std::cout << "Child process (" << pid << ") killed by signal (" << WTERMSIG(status) << ")" << std::endl;
//            continue;
//        } else if (WIFSTOPPED(status)) {
//            std::cout << "Child process (" << pid << ") stopped by signal (" << WSTOPSIG(status) << ")"
//                      << std::endl;
//            continue;
//        } else if (WIFCONTINUED(status)) {
//            std::cout << "Child process (" << pid << ") continued" << std::endl;
//            continue;
//        }
//    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//    for(int i =0; i<blocks.size(); i++){
//        blockedGrow(shm,gens,blocks[i],N);
//    }

//    auto stop = high_resolution_clock::now();
//    auto duration = duration_cast<milliseconds>(stop - start);
//    std::cout << "time taken to grow tree "<<gens<<" times with "<<blocks.size()<<" workers: "
//              << duration.count() << " milliseconds" << std::endl;
    std::cout<<"Starting growth..."<<std::endl;
    auto start = high_resolution_clock::now();

    if(blocks.size() == 1){
        blockedGrow(shm,gens,blocks[0],N);
    }
    else{
        int workers = blocks.size();
        pid_t pids[workers];

        for(int i = 0; i< workers; i++){
            if ((pids[i] = fork()) < 0) {
                perror("fork");
                printf("fork error");
                abort();
            } else if (pids[i] == 0) {
                blockedGrow(shm,gens,blocks[i],N);
                exit(0);
            }
        }
        int status;
        pid_t pid;
        while (workers > 0) {
            pid = waitpid(pid, &status, WUNTRACED | WCONTINUED);;
            //printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
            --workers;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "time taken to grow tree "<<gens<<" times with "<<blocks.size()<<" workers: "
              << duration.count() << " milliseconds" << std::endl;

    std::cout<<"Running non-parallelized growth...(safe to exit)"<<std::endl;




    //writeTreeToCsvShm(shm,N);

    shmdt((void *) shm);
    int t = shmctl(shmId, IPC_RMID, NULL);
    if(t == -1){
        perror("shmctl");
    }
    growTree(vec2,gens);
    writeTreeToCsv(vec2);
    fclose(fp);
}



