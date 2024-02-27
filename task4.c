#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_STUDENTS 10

// Global variables
int total_grade = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to add grade to total_grade with mutual exclusion
void class_total(int grade) {
    pthread_mutex_lock(&mutex);
    total_grade += grade;
    pthread_mutex_unlock(&mutex);
}

// Function to be executed by each thread
void* student_thread(void* arg) {
    int grade = *(int*)arg;
    class_total(grade);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_STUDENTS];
    int student_grades[NUM_STUDENTS];

    // Prompt for student grades
    printf("Please provide grades for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        printf("Student %d: ", i + 1);
        scanf("%d", &student_grades[i]);
    }

    // Create threads
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_create(&threads[i], NULL, student_thread, (void*)&student_grades[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Print the total grade
    printf("The total grade of the class is: %d\n", total_grade);

    return 0;
}
