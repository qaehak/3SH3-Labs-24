// SFWRENG 3SH3 - Group 24: Lab 2

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Define constants
#define DEPOSIT_THREADS 7
#define WITHDRAW_THREADS 3
#define TRANSACTION_AMOUNT 100
#define MAX_AMOUNT 400

int amount = 0; // Shared bank account balance
pthread_mutex_t mutex; // Mutex lock
sem_t can_deposit, can_withdraw; // Semaphores to control deposit and withdrawal conditions

// Deposit function executed by deposit threads
void *deposit(void *param) {
    printf("Executing deposit function\n");
    sem_wait(&can_deposit); // Wait until deposits are allowed
    pthread_mutex_lock(&mutex); // Lock before modifying shared variable
    if (amount < MAX_AMOUNT) {
        amount += TRANSACTION_AMOUNT; // Add deposit amount to the shared balance
        printf("Amount after deposit = %d\n", amount);
    }
    pthread_mutex_unlock(&mutex); // Unlock after modification
    sem_post(&can_withdraw); // Signal that withdrawal may be possible
    pthread_exit(0);
}

// Withdraw function executed by withdraw threads
void *withdraw(void *param) {
    printf("Executing withdraw function\n");
    sem_wait(&can_withdraw); // Wait until withdrawals are allowed
    pthread_mutex_lock(&mutex); // Lock before modifying shared variable
    if (amount > 0) {
        amount -= TRANSACTION_AMOUNT; // Subtract withdrawal amount from the shared balance
        printf("Amount after withdrawal = %d\n", amount);
    }
    pthread_mutex_unlock(&mutex); // Unlock after modification
    sem_post(&can_deposit); // Signal that deposit may be possible
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2 || atoi(argv[1]) != TRANSACTION_AMOUNT) {
        fprintf(stderr, "Usage: %s 100\n", argv[0]);
        return 1;
    }
    
    pthread_t deposit_threads[DEPOSIT_THREADS], withdraw_threads[WITHDRAW_THREADS];
    pthread_mutex_init(&mutex, NULL); // Initialize mutex
    sem_init(&can_deposit, 0, MAX_AMOUNT / TRANSACTION_AMOUNT); // Allow deposits initially
    sem_init(&can_withdraw, 0, 0); // Block withdrawals initially
    
    // Create deposit threads
    for (int i = 0; i < DEPOSIT_THREADS; i++) {
        if (pthread_create(&deposit_threads[i], NULL, deposit, NULL) != 0) {
            fprintf(stderr, "Error creating deposit thread\n");
            return 1;
        }
    }
    
    // Create withdraw threads
    for (int i = 0; i < WITHDRAW_THREADS; i++) {
        if (pthread_create(&withdraw_threads[i], NULL, withdraw, NULL) != 0) {
            fprintf(stderr, "Error creating withdraw thread\n");
            return 1;
        }
    }
    
    // Join deposit threads to ensure they complete execution
    for (int i = 0; i < DEPOSIT_THREADS; i++) {
        pthread_join(deposit_threads[i], NULL);
    }
    
    // Join withdraw threads to ensure they complete execution
    for (int i = 0; i < WITHDRAW_THREADS; i++) {
        pthread_join(withdraw_threads[i], NULL);
    }
    
    // Print the final account balance after all operations
    printf("Final amount = %d\n", amount);
    
    pthread_mutex_destroy(&mutex); // Destroy mutex to free resources
    sem_destroy(&can_deposit); // Destroy semaphore
    sem_destroy(&can_withdraw); // Destroy semaphore
    return 0;
}