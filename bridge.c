#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

pthread_mutex_t mut; // mutex variable
pthread_cond_t cond; // condition variable

int currentDirec; // the current direction on the one lane bridge
int currentNumber; // the currentNumber of cars on the bridge

struct arg_struct { // so I can pass car number through in pthread_create
    int carNum;
};

/*Function prototype declarations*/
void *OneVehicle(void *arguments);
void CrossBridge(int direc, int carNum);
void ArriveBridge(int direc, int carNum);
void ExitBridge(int direc, int carNum);
bool isSafe(int direc);

// Purpose calling function from pthread_create, allows each thread to emulate car, enter, cross and exit bridge
void *OneVehicle(void *arguments) { 
    pthread_mutex_lock(&mut);
    struct arg_struct *args = (struct arg_struct *)arguments;
    int carNum = args->carNum;
    int direc = rand() % 2;
    pthread_mutex_unlock(&mut);
    ArriveBridge(direc, carNum);
    CrossBridge(direc, carNum);
    ExitBridge(direc, carNum);
    sched_yield();
}

// Purpose is to handle bridge arrival per pseudocode provided
void ArriveBridge(int direc, int carNum) {
    while(!isSafe(direc)) {
        pthread_mutex_lock(&mut);
        pthread_cond_wait(&cond, &mut);
        pthread_mutex_unlock(&mut);
    } 

    printf("Car %d entering bridge\n", carNum);
    sched_yield();
}

// Purpose is to handle bridge exit per pseudo code provided
void ExitBridge(int direc, int carNum) {
    pthread_mutex_lock(&mut);
    printf("Car %d exiting bridge\n", carNum);
    currentNumber = currentNumber - 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mut);
    sched_yield();
}

// Purpose is to sense for bridge entrance safety using monitors direction and number of cars variable
bool isSafe(int direc) {
    if (currentNumber == 0) return true;
    else if((currentNumber < 3) && (currentDirec == direc)) return true;
    else return false;
}

// Purpose is mainly for debugging to see when a thread enters this portion of the code
void CrossBridge(int direc, int carNum) {
    pthread_mutex_lock(&mut);
    if(currentDirec != 0 || currentDirec != 1) currentDirec = direc;
    currentNumber += 1;
    pthread_mutex_unlock(&mut);
        if(direc == 0) {
        printf("Car %d crossing bridge heading North. Current number of cars on bridge: %d\n", carNum, currentNumber);
    }

    else {
        printf("Car %d crossing bridge heading South. Current number of cars on bridge: %d\n", carNum, currentNumber);
    }
    sched_yield();
}

// Purpose is to drive the function. Initializes our args, monitor and 50 children threads
int main() {
    int numChildren = 50;
    struct arg_struct args;
    pthread_t id[numChildren];
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);
    currentNumber = 0;
    
    for(int i = 0; i < numChildren; i++) {
        args.carNum = i+1;
        int ret = pthread_create(&id[i], NULL, &OneVehicle, (void *)&args);
        if(ret != 0) {
            perror("Failed to create thread");
            return 1;
        }// create 50 threads one for each car

    }
    for(int i = 0; i < numChildren; i++) {
        if(pthread_join(id[i], NULL) != 0) {
            return 2;
        }
    }
    pthread_exit(0);
    return 0;
}