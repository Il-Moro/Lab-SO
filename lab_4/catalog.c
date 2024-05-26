#include <stdio.h> //standard input/output
#include <stdlib.h> // standard library (per malloc, calloc, realloc, free)
#include <errno.h> // per perror, exit...
#include <semaphore.h> // per semafori
#include <string.h> // gestione stringhe
#include <sys/types.h> // system types (O_CREATE, O_RW ecc...)
#include <pthread.h> // per creazione di thread 

sem_t s;
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
    lettura del file catalog.txt -> creazione di una matrice
    con rispettivo semaphore, per indicare se è occupato o no
    -> i semafori devono essere creati se non esistono 
    alrimenti devono essere persistenti e riferirsi sempre 
    alla stessa vettura

// SECONDA PARTE:
    nel ciclo infinito:
    - eseguo i 4 comandi
*/

// sem_t *sem_open(const char * name , int oflag , ... ,mode_t mode , unsigned int value */ );


char **get_matrice(int *righe, char **DB_vetture) {
    // lettura file, creazione matrice
    // printf("2) ************************************\n");
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
        // Rimuovi il carattere di nuova riga alla fine della stringa
        vettura[strcspn(vettura, "\n")] = '\0';
        // Incrementa il conteggio delle righe
        (*righe)++;
        // printf("Lunghezza: %d\n", lunghezza);
        // printf("Vettura: %s\n", vettura);
        // Rialloca memoria per la nuova riga
        DB_vetture = realloc(DB_vetture, (*righe) * sizeof(char *));
        if (DB_vetture == NULL) {
            perror("Error: realloc\n");
            exit(1);
        }
        // printf("3) ************************************\n");
        // Allocazione di memoria per la nuova stringa
        DB_vetture[(*righe) - 1] = malloc(lunghezza + 1); // +1 per il terminatore null
        if (DB_vetture[(*righe) - 1] == NULL) {
            perror("Error: malloc");
            exit(1);
        }
        // printf("4) ************************************\n");
        // Copia la stringa nella matrice
        strcpy(DB_vetture[(*righe) - 1], vettura);
    }
    fclose(f);
    // printf(" 5) ************************************\n");
    return DB_vetture;
}

sem_t * get_sem(righe){

}















int main(){
    int righe = 0;
    char ** DB_vetture = malloc(sizeof(char *)); 
    // printf("1) ************************************\n");
    DB_vetture = get_matrice(&righe, DB_vetture);

    sem_t * semaphores = malloc(sizeof(sem_t));
    semaphores = get_sem(righe);

    


    // 4 COMANDI:
    while(1){
        char * comando;
        printf("Command: ");
        scanf("%s", comando);

        if(strcmp(comando, "view")){
            continue;
        } else if (strcmp (comando, "lock")){
            continue;
        } else if(strcmp(comando, "release")){
            continue;
        } else if(strcmp(comando, "quit")){
            continue;
        } else{
            perror("Nessun comando con questo nome");
        }
    }


    for(int i = 0; i < righe; i++){
        free(DB_vetture[i]);
    }
    free(DB_vetture);
    return 0;
}