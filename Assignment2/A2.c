/* Group 24 Assignment 2
Compile and run the code using the given code:
gcc -pthread A2.c -o A2
./A2
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_STUDENTS 5
#define NUM_CHAIRS 3

// Semaphores and mutex
sem_t ta_sem;          // Semaphore for TA availability
sem_t student_sem;     // Semaphore for students waiting
pthread_mutex_t mutex; // Mutex for protecting shared data

// Shared variables
int waiting_students = 0; // Number of students waiting in the hallway
int ta_sleeping = 1;      // Flag to indicate if the TA is sleeping

// Function prototypes
void* ta_thread(void* arg);
void* student_thread(void* arg);
void ta_help_student(int student_id);
void student_programming(int student_id);

// TA thread function
void* ta_thread(void* arg) {
    while (1) {
        // Wait for a student to arrive
        sem_wait(&ta_sem);

        // To check if students are waiting
        pthread_mutex_lock(&mutex);
        if (waiting_students > 0) {
            // Help the next student
            waiting_students--;
            sem_post(&student_sem); // Signal a waiting student
            ta_sleeping = 0;        // TA is no longer sleeping
            pthread_mutex_unlock(&mutex);

            // Simulate helping the student
            ta_help_student(-1); // -1 will indicate the TA is helping a student
        } else {
            // No students waiting, TA goes back to sleep
            ta_sleeping = 1;
            pthread_mutex_unlock(&mutex);
            printf("TA is sleeping.\n");
        }
    }
}

// Student thread function
void* student_thread(void* arg) {
    int student_id = *(int*)arg;
    while (1) {
        // Simulate programming
        student_programming(student_id);

        // Try to get help from the TA
        pthread_mutex_lock(&mutex);
        if (waiting_students < NUM_CHAIRS) {
            // There is space in the hallway
            waiting_students++;
            pthread_mutex_unlock(&mutex);

            // Notify the TA
            sem_post(&ta_sem);

            // Wait for the TA to help
            sem_wait(&student_sem);

            // Get help from the TA
            ta_help_student(student_id);
        } else {
            // No chairs available, come back later
            pthread_mutex_unlock(&mutex);
            printf("Student %d: No chairs available, will try again later.\n", student_id);
        }
    }
}

// Simulate the TA helping a student
void ta_help_student(int student_id) {
    printf("TA is helping student %d.\n", student_id);
    sleep(rand() % 3 + 1); // Simulate helping time
    printf("TA finished helping student %d.\n", student_id);
}

// Simulate a student programming
void student_programming(int student_id) {
    printf("Student %d is programming now.\n", student_id);
    sleep(rand() % 5 + 1); // Simulate programming time
}

int main() {
    pthread_t ta;
    pthread_t students[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];

    // Initialize semaphores and mutex
    sem_init(&ta_sem, 0, 0);
    sem_init(&student_sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Seed the random number generator
    srand(time(NULL));

    // Create the TA thread
    pthread_create(&ta, NULL, ta_thread, NULL);

    // Create student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    // Join threads (this program runs indefinitely)
    pthread_join(ta, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }

    // Clean up
    sem_destroy(&ta_sem);
    sem_destroy(&student_sem);
    pthread_mutex_destroy(&mutex);

    return 0;
}