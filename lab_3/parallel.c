#include <stdio.h> 
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 10

/*
COSA DEVE FARE IL MIO PROGRAMMA:
    Dati un file f, un numero n e una stringa s in ingresso:
     - 1) deve aprire il file contenente i path
     - 2) leggere (path) riga per riga -> uso della PIPE con scrittura e lettura in buffer
     - 3) definisco una nuova stringa con il terzo parametro in ingresso 
     - eseguire i comandi



*/

char * lettura_riga(){

}

int main(int argc, const char * argv[4]){
    if(argc != 4){
        perror("Few arguments");
        return 0;
    }

    // 1) apertura di un file:
    FILE * f = fopen(argv[1], "r");

    lettura_riga(f);

    char * path;

    int lengh = 10;
        
    char * path;  

    fgets(path, MAX_SIZE, f);
    printf("%s", path);
    
    return 0;
}