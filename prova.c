#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int c = 0;

    int pid = fork();
    c++;

    if (pid == 0) {
        printf("F%d\n", c);
    } else {
        printf("P%d\n", c);
    }

    int pid2 = fork();
    c++;

    if (pid2 == 0) {
        printf("F%d\n", c);
    } else {
        printf("P%d\n", c);
    }

    int pid3 = fork();
    c++;

    if (pid3 == 0) {
        printf("F%d\n", c);
    } else {
        printf("P%d\n", c);
    }

    return 0;
}