#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void* hello_word() {
    time_t t;
    srand((unsigned) time(&t));
    sleep(rand() % 5);
    printf("Hello World!\n");
    return NULL;
}

void* goodbye() {
    time_t t;
    srand((unsigned) time(&t));
    sleep(rand() % 5);
    printf("Goodbye!\n");
    return NULL;
}

int main() {
    pthread_t thread_hello, thread_goodbye;
    pthread_create(&thread_hello, NULL, hello_word, NULL);
    pthread_create(&thread_goodbye, NULL, goodbye, NULL);
    pthread_join(thread_hello, NULL);
    pthread_join(thread_goodbye, NULL);
    return 0;
}