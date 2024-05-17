#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

/*
1. Nel main:
	a) controllo che ci siano gli argomenti

2. Passo l'argomento alla funzione ricorsiva dove:
	a) controllo che sia una directory
	b) apro la strut stat
	c) per ogni elemento nella dir:
		- lo passo a una funzione che apra la struct stat e stampi le info
		- controllo se è una dir
		- ritorno true se lo è
	d) se ho trovato una dir:
		- invoco se stessa con il nuovo path	 
*/

int stampa(char * f_d){
	struct stat buf;
	int flag = 0;

	if((stat(f_d, &buf)) < 0){ // controllo se posso leggere le informazioni su file/directory 
		printf("    Impossibile leggere info\n");
		return -1;
	}
	else {

		printf("    Inode: %ld\n", (long)buf.st_ino);
		if(S_ISLNK(buf.st_mode)){ printf("    Type: symbolic link\n");} 
		else if(S_ISDIR(buf.st_mode)){ printf("    Type: directory\n"); flag = -2;}
		else if(S_ISREG(buf.st_mode)){ printf("    Type: file\n");}
		else if(S_ISFIFO(buf.st_mode)){ printf("    Type: FIFO"); }
		else { printf("    Type: other\n"); }				
		printf("    Size: %ld\n", (long)buf.st_size);
		printf("    Owner: %d %s\n", buf.st_uid, getpwuid(buf.st_uid)->pw_name);
    	printf("    Group: %d %s\n", buf.st_gid, getgrgid(buf.st_gid)->gr_name);
		return flag;
	}
}

void info(char * dir, int count){
	// dir = directory da cui parto

	struct stat buf; // creo una struct di tipo stat per le info
	struct dirent *dirp; // creo un puntatore a struct dirent (per listare il contenuto) 
	DIR *dp; // creo un puntatore di tipo DIR per aprire la directory

	// S.C. stat > apro la struct di tipo stat chiamata buf riferita alla directory dir (dir di input) per poter leggere le informazioni sul file/dir
	if(stat(dir, &buf) < 0) { printf("Impossibile leggere le informazioni sul file \n"); exit(1);}
	
	// verifico che che dir sia una directory
	if (!S_ISDIR(buf.st_mode)){ printf("%s deve essere una directory\n", dir); exit(1);}
	
	// apro la directory per poterla leggere
	if ((dp = opendir(dir)) == NULL) { printf("%s impossibile da aprire\n", dir); exit(1);}
	
	// finchè *dirp != NULL, allora la struct lista linkata contiene informazioni leggo i valori 
	while ( (dirp = readdir(dp)) != NULL){ 
		// dp è come un file > il cursore si sposta ogni volta che leggo con readdir.
		char * elemento = dirp->d_name;
		char * path_MAX;

		//Devo unire il nome dir con elemento:
		int lunghezza = strlen(dir) + 1 + strlen(elemento) + 1;
		path_MAX = malloc(lunghezza);
		snprintf(path_MAX, lunghezza, "%s/%s", dir, elemento);

		if(strcmp(elemento, "..") == 0){
			continue;
		}

		if((strcmp(elemento, ".") == 0)){
			if (count < 2){
				printf("Node: %s\n", elemento);
				stampa(elemento);
				count++;
			}
			else{
				continue;
			}
		}
		else{
			printf("Node: %s\n", path_MAX);
			if(stampa(path_MAX) == -2){info(path_MAX, count);}
		}
		free(path_MAX);			
	}
	closedir(dp);
}

int main(int argc, char * argv[2]){
	if (argc != 2){
		printf("Richiesto un argomento");
	}

	info(argv[1], 1);

	return 0;
}