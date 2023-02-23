#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void wipe_buffer(char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = 0;
    }
}

void wipe_file(char *filename) {
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("[ERROR - ZEROIZE]Error: Failed to open file %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char buffer[1024];
    size_t count;

    // Sovrascrive il file con sequenze di 0
    wipe_buffer(buffer, sizeof(buffer));
    while ((count = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        wipe_buffer(buffer, count);
        fseek(file, -count, SEEK_CUR);
        fwrite(buffer, 1, count, file);
        fflush(file);
    }

    // Sovrascrive il file con sequenze di 1
    wipe_buffer(buffer, sizeof(buffer));
    memset(buffer, 0xff, sizeof(buffer));
    for (int i = 0; i < 7; i++) {
        fwrite(buffer, 1, sizeof(buffer), file);
        fflush(file);
        rewind(file);
        wipe_buffer(buffer, sizeof(buffer));
    }

    // Sovrascrive il file con byte random
	wipe_buffer(buffer, sizeof(buffer));
	for (int i = 0; i < 3; i++) {
    	for (size_t j = 0; j < size / sizeof(buffer); j++) {
        	for (size_t k = 0; k < sizeof(buffer); k++) {
            	buffer[k] = (char) rand();
        	}
        	fwrite(buffer, 1, sizeof(buffer), file);
        	fflush(file);
    	}
	}

    // Tronca il file
    ftruncate(fileno(file), 0);
    fflush(file);
    fclose(file);

    // Elimina il file dal filesystem
    remove(filename);
}



