#include "kernel/types.h"
#include "user.h"
#define NUM_OF_CHECKERS 3

// Function to check if a number is prime
int isPrime(int number) {
    if (number <= 1) {
        return 0; // 0 and 1 are not prime numbers
    }
    if (number <= 3) {
        return 1; // 2 and 3 are prime numbers
    }
    if (number % 2 == 0 || number % 3 == 0) {
        return 0; // divisible by 2 or 3 is not a prime number
    }

    for (int i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0) {
            return 0; // divisible by i or i+2 is not a prime number
        }
    }

    return 1;
}

void run_system(int num) {
    int pids[num + 1];

    // create channels
    int chan1 = channel_create();
    int chan2 = channel_create();
    if (chan1 < 0 || chan2 < 0) {
        printf("Failed to create channels\n");
        exit(1);
    }
    

    // create checkers
    for(int i=0; i<num; i++){
        pids[i] = fork();
        if(pids[i] == 0){
            
            // checker code
            int checker_num = i+1;
            printf("Checker %d created, pid is %d\n",checker_num, pids[i]);
            int data;
            while(!channel_take(chan1, &data)){
                // check if prime number
                if(isPrime(data) && channel_put(chan2, data) < 0){
                    break;
                }
            }
            channel_destroy(chan1);
            printf("Checker %d finished, pid is %d\n",checker_num, pids[i]);
            exit(0);
        }
    }

    // create printer
    pids[num] = fork();
    if(pids[num] == 0){
         printf("Printer created, pid is %d.\n",pids[num]);
        // printer code
        int prime_num;
        int counter = 0;
        while(counter < 100 && !channel_take(chan2, &prime_num)){
            printf("Prime number: %d\n", prime_num);
            counter++;
        }
        // destroy channels
        channel_destroy(chan2);
        printf("Printer finished, pid is %d, %d prime numbers printed!\n",pids[num], counter);
        exit(0);
    }

    // generator code
    int i = 2;
    while (!channel_put(chan1, i++)){}
    
    // Wait for all child processes to finish
    int pid;
    for (int i = 0; i < num + 1; i++) {
        wait(&pid);
    }
    
    printf("All processes finished!\n");
}

int main(int argc, char* argv[]) {
    int num = NUM_OF_CHECKERS;

    if (argc > 2) {
        printf("Usage: prime_numbers [number of checkers]\n");
        exit(1);
    }
    if (argc == 2) {
        num = atoi(argv[1]);
        printf("Using %d checkers.\n", num);
    } else {
        printf("Using default of %d checkers.\n", NUM_OF_CHECKERS);
    }

    while (1) {
        run_system(num);

        printf("Do you want to start the system again? press 1 to restart and any other key to exit: ");
        char buf[100];
        gets(buf, 100);
        if(buf[0] != '1'){
            break;
        }
    }

    exit(0);
}