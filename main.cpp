#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUFFER_SIZE 1024 // Dimensione buffer utilizzata nella funzione di xor

void xorFile(char *inputFile, char *keyFile, char *outputFile);
int removeNbyte(char *filename, long n);
long getFileSize(char *filename);
void printStats(long fileLen, long keyLen);
char* cutLastNchars(char* str, int n);
void generateRandomKey(char *filename, long n);
void printHelp(char *fname);

int main(int argc, char** argv) { //<fileName> <key>
	if(argc<3){
		printHelp(argv[0]);
		exit(-3);
	}
	
	if(strcmp("keygen",argv[1]) == 0){ // Funzione per la generazione di chiavi casuali
		if(argc<4){// keygen, outputFile, len
			printf("Arguments Error!\nUse: %s keygen <keyFile> <keyLen>\n",argv[0]);
			exit(-3);
		}else{
			printf("[RNDKEY] Random Key Generator\n");
			long len = strtol(argv[3], NULL, 10);
			generateRandomKey(argv[2], len);
		}
	}else if(strcmp("strip",argv[1]) == 0){
		if(argc<4){// strip, keyFile, len
			printf("Arguments Error!\nUse: %s strip <keyFile> <len>\n",argv[0]);
			exit(-3);
		}else{
			long len = strtol(argv[3], NULL, 10);
			printf("[KEYSTRIP] Removed %u bytes from %s\n",len,argv[2]);
			removeNbyte(argv[2], len);
		}
	}else{
		if(strstr(argv[1], ".xorenc") != NULL){ //DECRYPT
			char *outFile = cutLastNchars(argv[1], 7); // Ricava l'estensione originale del file
			
			long fileLen = getFileSize(argv[1]);
			long keyLen = getFileSize(argv[2]);
			
			if(keyLen >= fileLen){ // dimensione Chiave >= dimensione file
				xorFile(argv[1],argv[2],outFile); // Esegue l'XOR
				printStats(fileLen, keyLen); // Mostra informazioni riguardo la procedura
				removeNbyte(argv[2],fileLen); // Rimuove i bytes utilizzati dal file della chiave
			}else{
				printf("[ERROR - KEYLEN] ERROR! Key length MUST have same size of the plaintext file\n");
			}
		}else{ // ENCRYPT
			char outFile[strlen(argv[1]) + strlen(".xorenc")];
			strcpy(outFile,argv[1]);
			strcat(outFile,".xorenc"); // Crea l'estezione per il file di output
		
			long fileLen = getFileSize(argv[1]);
			long keyLen = getFileSize(argv[2]);
		
			if(keyLen >= fileLen){ // dimensione Chiave >= dimensione file
				xorFile(argv[1],argv[2],outFile); // Esegue l'XOR
				// Mostra informazioni sui dati utilizzati
				printStats(fileLen, keyLen); // Mostra informazioni riguardo la procedura
				removeNbyte(argv[2],fileLen); // Rimuove i bytes utilizzati dal file della chiave
			}else{
				printf("[ERROR - KEYLEN] ERROR! Key length MUST have same size of the plaintext file\n");
			}
		}
	}
	return 0;
}

void printHelp(char *fname){
	printf("HackerInside FileXOR\n");
	printf("Use: %s <fileName> <keyFile> to encrypt / decrypt a file\n",fname);
	printf("Use: %s keygen <fileName> <n> to generate a random key of n bytes\n",fname);
	printf("Use: %s strip <keyFile> <n> to remove n bytes from keyFile\n",fname);
	printf("\nWARNING! The randomly generated KEY is not cryptographically secure, always provide a truly random KEY when encrypting\n");
	printf("\nWARNING! The key must always have the same length as the file to be encrypted!\n");
}

void generateRandomKey(char *filename, long n) { // Attenzione! Funzione non crittograficamente sicura / affidabile!
    // Apre il file in modalità write byte
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    // Genera i dati random e li scrive nel file
    srand(time(NULL));
    for (long i = 0; i < n; i++) {
        char c = rand() % 256; // Genera
        fwrite(&c, 1, 1, fp);  // Scrive
    }
    // Chiude il file
    fclose(fp);
}


char* cutLastNchars(char* str, int n) {
    int len = strlen(str);
    if (n >= len) {
        return "";
    }
    char* new_str = (char*)malloc(len - n + 1); // alloca memoria per la nuova stringa
    strncpy(new_str, str, len - n); // copia i caratteri fino a len - n
    new_str[len - n] = '\0'; // aggiunge il terminatore di stringa
    return new_str;
}

void printStats(long fileLen, long keyLen){
	printf("Key bytes used: %u (%.2f %%)\n",fileLen,(((float)fileLen / (float)keyLen) * 100));
	printf("Key Size: %u\n",keyLen);
	printf("Remaining key bytes: %u (%.2f %%)\n",(keyLen - fileLen),(((float)(keyLen - fileLen) / (float)keyLen) * 100));
}

void xorFile(char *inputFile, char *keyFile, char *outputFile) {
    FILE *input = fopen(inputFile, "rb"); // File input
    FILE *key = fopen(keyFile, "rb"); // File chiave
    FILE *output = fopen(outputFile, "wb"); // File output

    if (input == NULL || key == NULL || output == NULL) {
        printf("[ERROR - XOR] Error opening one of the files!\n");
        return;
    }
    int inputByte, keyByte;
    while ((inputByte = fgetc(input)) != EOF && (keyByte = fgetc(key)) != EOF) { // XOR
        fputc(inputByte ^ keyByte, output);
    }

    fclose(input);
    fclose(key);
    fclose(output);
}

int removeNbyte(char *filename, long n) {
    // Apre il file in modalita read byte
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("[ERROR - REMBY] Error opening the file.\n");
        return 1;
    }

    // prende la dimensione del file
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);

    // Allocazione di memoria per il buffer
    char *buffer = (char*)malloc(file_size - n);
    if (buffer == NULL) {
        printf("[ERROR - REMBY] Error allocating memory.\n");
        return 1;
    }

    // legge il contenuto del file nel buffer
    fseek(fp, n, SEEK_SET);
    fread(buffer, 1, file_size - n, fp);
    
    fclose(fp); // chiude il file
    
    // Riapre il file in binary write mode 
    fp = freopen(filename, "wb", fp);
    if (fp == NULL) {
        printf("[ERROR - REMOVE_N_BYTE] Error reopening the file.\n");
        return 1;
    }
    
    fwrite(buffer, 1, file_size - n, fp); // Scrive il contenuto del buffer nel file

    // chiude il file e libera la memoria allocata per il buffer
    fclose(fp);
    free(buffer);

    return 0;
}

long getFileSize(char *filename) {
	// Apre il file in modalità read byte
    FILE *file = fopen(filename, "rb"); 

    if (file == NULL) {
        printf("[ERROR - FILESIZE] Error opening file!\n");
        return -1;
    }
	
	// Prende le dimensioni del file
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}
