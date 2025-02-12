// SFWRENG 3SH3 - Group 24: Lab 2

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 20  // size of the list
int numbers[SIZE]; // global array of numbers
int total_sum = 0; // this variable will hold the final sum

// Structure to pass parameters to threads
typedef struct {
    int from_index;
    int to_index;
} parameters;

// Thread function to sum a portion of the array
void *runner(void *param) {
    parameters *data = (parameters *) param;
    int sum = 0;
    
    // Sum the numbers in the given range
    for (int i = data->from_index; i <= data->to_index; i++) {
        sum += numbers[i];
    }
    
    // Add the sum to the global sum (atomic operation may be needed in more advanced versions)
    total_sum += sum;

    // Exit the thread
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    // Check if exactly one argument is passed
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return -1;
    }
    
    int num_threads = atoi(argv[1]);
    
    // Check if the number of threads is valid
    if (num_threads <= 0 || num_threads > SIZE) {
        printf("Invalid number of threads. It should be between 1 and %d\n", SIZE);
        return -1;
    }

    // Initialize the numbers array
    for (int i = 0; i < SIZE; i++) {
        numbers[i] = i + 1; // Fill array with numbers 1 to 20
    }

    // Declare thread identifiers and attributes
    pthread_t tid[num_threads];
    pthread_attr_t attr;

    // Initialize thread attributes
    pthread_attr_init(&attr);

    // Split the list into equal parts for each thread
    int chunk_size = SIZE / num_threads;

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->from_index = i * chunk_size;
        data->to_index = (i == num_threads - 1) ? SIZE - 1 : (i + 1) * chunk_size - 1;
        
        pthread_create(&tid[i], &attr, runner, data);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }

    // Output the total sum
    printf("Sum of numbers in the list is: %d\n", total_sum);

    return 0;
}