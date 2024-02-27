#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_STUDENTS 5
#define MAX_NAME_LENGTH 50

struct Student {
    char name[MAX_NAME_LENGTH];
    int student_id;
    int grade;
    pthread_t thread;
};

void* bellcurve(void* arg) {
    struct Student* student = (struct Student*)arg;
    int bellcurved_grade = student->grade * 1.50;

    printf("Info: Name: %s, ID: %d, Original Grade: %d, Bellcurved Grade: %d\n",
           student->name, student->student_id, student->grade, bellcurved_grade);

    pthread_exit(NULL);
}

int main() {
    struct Student students[NUM_STUDENTS];

    // Prompt for student information
    printf("Enter information for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        printf("Student %d:\n", i + 1);
        printf("Name: ");
        scanf("%s", students[i].name);
        printf("Student ID: ");
        scanf("%d", &students[i].student_id);
        printf("Grade: ");
        scanf("%d", &students[i].grade);
    }

    // Create threads after receiving all student information
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_create(&students[i].thread, NULL, bellcurve, (void*)&students[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_join(students[i].thread, NULL);
    }

    return 0;
}
