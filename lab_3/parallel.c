#include <stdio.h> 
#include <fcntl.h>      // O_CREAT, O_RDWR
#include <sys/mman.h>   // shm_open, mmap
#include <sys/stat.h>   // S_IRUSR, S_IWUSR
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
/*
COSA DEVE FARE IL MIO PROGRAMMA:
    Dati un file f, un numero n e una stringa s in ingresso:
     - 1) deve aprire il file contenente i path
     - 2) leggere (path) riga per riga -> 
        faccio un for e per ogni riga letta la passo al processo figlio attraverso la pipe
     - 3) ogni processo figlio attraverso una system() deve eseguire il comando passato attraverso una pipe

     -per il punto 2) potrei usare sempre una system() per una lettura più rapida del file
*/


char ** get_matrice(const char * file, int * righe){
    char **matrice_path = malloc(sizeof(char *));
    if(matrice_path == NULL){printf("Errore durante allocazione 1"); exit(1);}
    char path[1024];

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        printf("Errore durante l'apertura del file.\n");
        exit(1);
    }

    while (fgets(path, 1024, fp) != NULL) {
        path[strcspn(path, "\n")] = 0; 
        // Ignora le righe vuote
        if (strlen(path) == 0) {
            continue;
        }
        
        (*righe)++;
        int lunghezza = strlen(path); // Lunghezza path per l'allocazione dinamica su matrice_path
        // realloca la matrice per aggiungere una riga
        matrice_path = realloc(matrice_path, (*righe) * sizeof(char *)); 
        if (matrice_path == NULL) {
            printf("Errore durante la riallocazione della memoria.\n");
            exit(1);
        }
        // allocazione di una singola riga
        matrice_path[(*righe) - 1] = malloc(lunghezza);
        strcpy(matrice_path[(*righe) - 1], path);
        // NOTA: il numero di righe è una in più perchè viene letta anche l'ultima riga vuota del file, che non contiene dati
    }
    (*righe);
    fclose(fp);
    return matrice_path;
}

int main(int argc, const char * argv[4]){

    // PRIMA PARTE: 
    // Costruisco una matrice n_righe x 1 per salvarmi i path all'interno dei file
    // Sarebbe meglio costruirla però su una zona di memoria condivisa solo con i figli (anonima) 
    // (altrimetni ogni figlio avrebbe una copia della matrice nello stack
    //      1. ridondante
    //      2. consumo eccessivo di memoria; )
    // -> MA TROPPO COMPLICATO, SARA' per una prossima volta

    int righe = 0;
    char ** matrice_path = get_matrice(argv[1], &righe);  

    // printf("Righe: %d\n", righe);
    /* for (int i = 0; i < righe; i++){
        printf("Path: %s\n", matrice_path[i]);
    }*/
    // printf("Path: %s\n", matrice_path[3]);

/**************************************************************************************/



    // SECONDA PARTE:
    // Faccio i processi figli e gli assegno i comandi da eseguire

    int numero_processi = atoi(argv[2]); 
    int num_CperProcess = righe/numero_processi;
    int proc_eccesso = righe % numero_processi;
    int status;
    pid_t pid = getpid();
    
    // Creazione processi figli
    for(int i = 0; i < numero_processi; i++){
        if(pid != 0){ // Padre
            fork();
        }
        else { // Figli
            /* Legge num_CperProcess, li salva, converte la stringa in comando, la usa con system
            */
           // Calcola la lunghezza della nuova stringa


        // FINIRE DA QUI (FARE CICLO FOR PER LEGGERE COMANDI)

        char * string = 
        
        size_t new_length = strlen(placeholder) + strlen(path) - 1; // -1 per sostituire '%'
        
        // Alloca memoria per la nuova stringa
        char *command = malloc(new_length + 1); // +1 per il terminatore null
        if (command == NULL) {
            perror("malloc");
            return 1;
        }
        
        // Trova la posizione del segnaposto '%'
        const char *percent_pos = strchr(placeholder, '%');
        if (percent_pos == NULL) {
            fprintf(stderr, "Segnaposto '%' non trovato nella stringa.\n");
            free(command);
            return 1;
        }
        
        // Copia la parte prima del segnaposto
        size_t prefix_length = percent_pos - placeholder;
        strncpy(command, placeholder, prefix_length);
        command[prefix_length] = '\0';
        
        // Aggiungi il percorso
        strcat(command, path);
        
        // Aggiungi la parte dopo il segnaposto (se esiste)
        strcat(command, percent_pos + 1);

        // Stampa il comando risultante
        printf("Comando: %s\n", command);
            }
            
        }
    
    


























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