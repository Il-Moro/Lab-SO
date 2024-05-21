#include <stdio.h> 
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXPROCESSES 256

/*
COSA DEVE FARE IL MIO PROGRAMMA:
    Dati un file f, un numero n e una stringa s in ingresso:
     - 1) deve aprire il file contenente i path
     - 2) leggere (path) riga per riga -> 
        faccio un for e per ogni riga letta la passo al processo figlio attraverso la pipe
     - 3) ogni processo figlio attraverso una system() deve eseguire il comando passato attraverso una pipe

     -per il punto 2) potrei usare sempre una system() per una lettura più rapida del file
*/

int main(int argc, const char * argv[4]){

    // PRIMA PARTE: 
    // costruisco una matrice n_righe x 1 per i path all'interno dei file

    char **matrice_path = malloc(sizeof(char *));
    int righe = 0;
    char path[1024];
    printf("File: %s\n", argv[1]);
    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Errore durante l'apertura del file.\n");
        return 1;
    }
    while (fgets(path, 1024, fp) != NULL) {
        righe++;
        matrice_path = realloc(matrice_path, sizeof(char *) * righe);
        int lunghezza = strlen(path);

        if (matrice_path == NULL) {
            printf("Errore durante la riallocazione della memoria.\n");
            return 1;
        }

        matrice_path[righe - 1] = malloc(lunghezza);
        strcpy(matrice_path[righe - 1], path);
        printf("Path: %s", matrice_path[righe - 1]);
        
    }
    righe--;
    fclose(fp);

    // SECONDA PARTE:
    // faccio i processi figli e gli assegno i comandi da eseguire





    for (int i = 0; i < righe; i++) {
        free(matrice_path[i]);
    }
    free(matrice_path);
   
    return 0;
}

/*
// UNA PRIMA IDEA CHE NON FUNZIONA: CON system() utilizzando il comando tail volevo 
    leggere le righe di un file ma non avevo modo di salvarmi l'output

    //lettura file riga per riga
    char * lettura_bash = malloc(15);
    int i = 1;

    char * path = malloc(200);

    while(1){
        sprintf(lettura_bash, "tail -n %d args.txt", i);
        path = system(lettura_bash);
        printf("%s", path);
        i++;
        // devo uscire dal ciclo;
        if (i == 4){break;}
    }

    free(lettura_bash);

*/ 
// -> CON SYSTEM() NON POSSO SALVARMI I DATI RELATIVI A UN COMANDO TIPO LS O TAIL
// -> potrei farlo con popen() che apre una pipe tra processo padre e un processo figlio; 
    // nel figlio attraverso una execve() viene modificato il processo figlio
    // e indirizza il risultato sulla pipe, in modo che il padre possa leggerlo