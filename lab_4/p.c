#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    char comando[20];
    char ** arg = malloc(sizeof(char*) * 2);

    printf("Command: ");
    if(fgets(comando, sizeof(comando), stdin) != NULL){
        comando[strcspn(comando, "\n")] = '\0';
    } else {
        perror("Error: \n");
        exit(1);
    }

    arg[0] = strtok(comando, " "); 
    arg[1] = strtok(NULL, " ");
    if (arg[1] == NULL){
        printf("HELLO\n");
    }


    printf("%s\n", arg[0]);

    return 0;
}
