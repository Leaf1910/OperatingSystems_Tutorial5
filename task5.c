#include <stdio.h>
#include <stdlib.h>
#include <dispatch/dispatch.h>
#include <pthread.h>

#define NUM_STUDENTS 10
#define BELL_CURVE_FACTOR 1.50

int grades[NUM_STUDENTS];
int total_grade = 0;
int total_bellcurve = 0;

dispatch_semaphore_t semaphore;

void* read_grades(void* arg) {
    FILE* file = fopen("grades.txt", "r");
    
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_STUDENTS; ++i) {
        fscanf(file, "%d", &grades[i]);
    }

    fclose(file);

    dispatch_semaphore_signal(semaphore);
    pthread_exit(NULL);
}

void* save_bellcurve(void* arg) {
    int grade = *(int*)arg;
    int bellcurved_grade = grade * BELL_CURVE_FACTOR;

    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);

    total_grade += grade;
    total_bellcurve += bellcurved_grade;

    dispatch_semaphore_signal(semaphore);

    FILE* file = fopen("bellcurve.txt", "a");
    
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d\n", bellcurved_grade);

    fclose(file);

    dispatch_semaphore_signal(semaphore);
    return NULL;
}

int main() {
    pthread_t read_thread, save_threads[NUM_STUDENTS];

    // Initialize semaphore
    semaphore = dispatch_semaphore_create(1);

    // Create read thread
    pthread_create(&read_thread, NULL, read_grades, NULL);

    // Wait for read thread to finish
    pthread_join(read_thread, NULL);

    // Create save threads
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_create(&save_threads[i], NULL, save_bellcurve, (void*)&grades[i]);
    }

    // Wait for all save threads to finish
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_join(save_threads[i], NULL);
    }

    // Print results
    printf("Total Grade: %d\n", total_grade);
    printf("Class Average (Before Bellcurve): %.2f\n", (float)total_grade / NUM_STUDENTS);
    printf("Total Bellcurved Grade: %d\n", total_bellcurve);
    printf("Class Average (After Bellcurve): %.2f\n", (float)total_bellcurve / NUM_STUDENTS);

    // Destroy semaphore
    dispatch_release(semaphore);

    return 0;
}
