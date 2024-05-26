#include <stdio.h> //standard input/output
#include <stdlib.h> // standard library (per malloc, calloc, realloc, free)
#include <errno.h> // per perror, exit...
#include <semaphore.h> // per semafori
#include <string.h> // gestione stringhe
#include <sys/types.h> // system types (O_CREATE, O_RW ecc...)
#include <pthread.h> // per creazione di thread 
#include <fcntl.h>

#define file_catalogo "catalog.txt"
/* COSA DEVE FARE IL PROGRAMMA:
Gestione di un parco macchine definito in catalog.txt
Programma che opera su ciclo infinito:
    sono possibili 4 comandi
    - visualizzazione: view 
    - noleggio: lock
    - termine noleggio: release
    - quit

più operatori possono eseguire il programma simultaneamente:
Tutti operano sulle stesse macchine
-> utilizzo di named semaphores, uno per vettura
il programma, dopo avere letto il catalogo, crea se necessario i semafori

// PRIMA PARTE:
    lettura del file catalog.txt -> conto il numero di righe
    creazione di n = righe semafori
    -> i semafori devono essere creati se non esistono 
    alrimenti devono essere persistenti e riferirsi sempre 
    alla stessa vettura

// SECONDA PARTE:
    nel ciclo infinito:
    - eseguo i 4 comandi
*/

char **get_matrice(int *righe, char **DB_vetture) {
    FILE *f = fopen(file_catalogo, "r");
    if (f == NULL) {
        perror("Errore in apertura file");
        exit(1);
    }
    char vettura[1025];
    while (fgets(vettura, sizeof(vettura), f) != NULL) {
        int lunghezza = strlen(vettura);
        if (lunghezza == 0) {
            continue;
        }
        vettura[strcspn(vettura, "\n")] = '\0';
        (*righe)++;
        DB_vetture = realloc(DB_vetture, (*righe) * sizeof(char *));
        if (DB_vetture == NULL) {
            perror("Error: realloc\n");
            exit(1);
        }
        DB_vetture[(*righe) - 1] = malloc(lunghezza + 1); // +1 per il terminatore null
        if (DB_vetture[(*righe) - 1] == NULL) {
            perror("Error: malloc");
            exit(1);
        }
        strcpy(DB_vetture[(*righe) - 1], vettura);
    }
    fclose(f);
    return DB_vetture;
}

sem_t ** get_sem(int righe, char ** DB_vetture){
    sem_t ** semaphores = malloc(sizeof(sem_t) * righe);
    for (int i = 0; i < righe; i++){
        char name[256];
        snprintf(name, (5 + strlen(DB_vetture[i]) + 1), "/sem_%s", DB_vetture[i]);
        semaphores[i] = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 0);
    }
    return semaphores;
}

int main(){
    int righe = 0;
    char ** DB_vetture = malloc(sizeof(char *)); 
    DB_vetture = get_matrice(&righe, DB_vetture);
    sem_t ** semaphores = get_sem(righe, DB_vetture);
    // 4 COMANDI:
    while(1){
        char comando[50];
        char * arg_a;
        char * arg_b;
        printf("Command: ");
        if (fgets(comando, sizeof(comando), stdin) != NULL){
            comando[strcspn(comando, "\n")] = '\0';
        } else {
            perror("Error\n");
            exit(1);
        }
        arg_a = strtok(comando," ");
        arg_b = strtok(NULL, " ");
        
        if(strcmp(arg_a, "view") == 0){
            for(int i = 0; i < righe; i ++){
                int value;
                sem_getvalue(semaphores[i], &value);
                char * status = value == 0 ? "busy" : "free"; 
                printf("Car: %s, status: %s\n", DB_vetture[i], status);
            }
        } else if (strcmp (arg_a, "lock") == 0){
            if(arg_b == NULL){
                printf("Error: required two argument\n");
                continue;
            } else {
                int flag = 0;
                for(int i = 0; i < righe; i++){
                    if(strcmp(arg_b, DB_vetture[i]) == 0){
                        flag = 1;
                        if(sem_trywait(semaphores[i]) == 0){
                            printf("Car: %s is now locked\n");
                            break;
                        } else {
                            printf("Error. Car %s already locked\n", DB_vetture[i]);
                            break;
                        }
                    }
                }
                if (flag == 0){ printf("Cannot find car %s\n", arg_b);}
            }
        } else if(strcmp(arg_a, "release") == 0){
            if(arg_b == NULL){
                printf("Error: required two argument\n");
                continue;
            } else {
                int flag = 0;
                for(int i = 0; i < righe; i++){
                    if(strcmp(arg_b, DB_vetture[i]) == 0){
                        flag = 1;
                        int value;
                        sem_getvalue(semaphores[i], &value);
                        if(value == 0){
                            sem_post(semaphores[i]);
                            printf("Car: %s is now free\n", DB_vetture[i]);
                            continue;
                        } else if (value == 1){
                            printf("Error. Car %s already free\n", DB_vetture[i]);
                            continue;
                        }
                    }
                }
                if (flag == 0){ printf("Cannot find car %s\n", arg_b);}
            }
        } else if(strcmp(arg_a, "quit") == 0){
            return 0;
        } else{
            printf("Unknown Command\n");
        }
    }
    for(int i = 0; i < righe; i++){
        free(DB_vetture[i]);
        free(semaphores[i]);
    }
    free(DB_vetture);
    free(semaphores);
    return 0;
}