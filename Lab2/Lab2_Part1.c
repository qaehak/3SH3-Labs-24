// SFWRENG 3SH3 - Group 24: Lab 2

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Define the number of threads for deposit and withdraw operations
#define NUMBER_OF_THREADS 6
#define DEPOSIT_THREADS 3
#define WITHDRAW_THREADS 3

int amount = 0; // Shared bank account balance
pthread_mutex_t mutex; // Mutex lock to ensure mutual exclusion

// Deposit function executed by deposit threads
void *deposit(void *param) {
    int deposit_amount = *((int *)param);
    pthread_mutex_lock(&mutex); // Lock before modifying shared variable
    amount += deposit_amount; // Add deposit amount to the shared balance
    printf("Deposit amount = %d\n", amount);
    pthread_mutex_unlock(&mutex); // Unlock after modification
    pthread_exit(0);
}

// Withdraw function executed by withdraw threads
void *withdraw(void *param) {
    int withdraw_amount = *((int *)param);
    pthread_mutex_lock(&mutex); // Lock before modifying shared variable
    amount -= withdraw_amount; // Subtract withdrawal amount from the shared balance
    printf("Withdrawal amount = %d\n", amount);
    pthread_mutex_unlock(&mutex); // Unlock after modification
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <deposit_amount> <withdraw_amount>\n", argv[0]);
        return 1;
    }
    
    // Convert arguments to integers
    int deposit_value = atoi(argv[1]);
    int withdraw_value = atoi(argv[2]);
    
    // Validate input values
    if (deposit_value <= 0 || withdraw_value <= 0) {
        fprintf(stderr, "Error: Both deposit and withdraw amounts must be positive integers.\n");
        return 1;
    }
    
    pthread_t deposit_threads[DEPOSIT_THREADS], withdraw_threads[WITHDRAW_THREADS];
    pthread_mutex_init(&mutex, NULL); // Initialize mutex
    
    // Create deposit threads
    for (int i = 0; i < DEPOSIT_THREADS; i++) {
        if (pthread_create(&deposit_threads[i], NULL, deposit, &deposit_value) != 0) {
            fprintf(stderr, "Error creating deposit thread\n");
            return 1;
        }
    }
    
    // Create withdraw threads
    for (int i = 0; i < WITHDRAW_THREADS; i++) {
        if (pthread_create(&withdraw_threads[i], NULL, withdraw, &withdraw_value) != 0) {
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
    return 0;
}