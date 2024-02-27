#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void hello_world() {
  
    printf("Hello, world!\n");
}

void goodbye() {
  
    printf("Goodbye!\n");
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, (void *)hello_world, NULL);
    pthread_create(&thread2, NULL, (void *)goodbye, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
