#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_FILENAME_LENGTH 20
#define MAX_FILE_ENTRIES 100

int INDEX;

typedef struct directory {
    int totalSize;
    int freeBlocks;
    int totalEntries;
    struct block* pointerToFirst;
} directory;

typedef struct block {
    char fileName[MAX_FILENAME_LENGTH];
    int index;
    int size;
    int occupiedBy; // número de entrada en la lista de fatEntries por quien está ocupado, -1 si no está ocupado
    struct block* previous;
    struct block* next;
    unsigned char data[]; // donde se almacenan los datos
} block;


// Structure for log
typedef struct Log {
    char message[100];
    struct Log* next;
} Log;