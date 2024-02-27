#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_STUDENTS 5

struct Student {
    int grade;
    pthread_t thread;
};

void* bellcurve(void* arg) {
    struct Student* student = (struct Student*)arg;
    int bellcurved_grade = student->grade * 1.50;

    printf("Original Grade: %d, Bellcurved Grade: %d\n", student->grade, bellcurved_grade);

    pthread_exit(NULL);
}

int main() {
    struct Student students[NUM_STUDENTS];

    // Prompt for grades
    printf("Enter grades for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        printf("Student %d: ", i + 1);
        scanf("%d", &students[i].grade);
    }

    // Create threads after receiving all grades
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_create(&students[i].thread, NULL, bellcurve, (void*)&students[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_join(students[i].thread, NULL);
    }

    return 0;
}
