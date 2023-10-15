#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#define MAX_FILENAME_LENGTH 20
#define MAX_FILE_ENTRIES 100

struct file {
    char fileName[MAX_FILENAME_LENGTH];
    int size;
    int startByte;
    int endByte;
};

struct directory {
    int totalSize;
    int sizeDirectory;
    int totalEntries;
    struct file fileList[MAX_FILE_ENTRIES];
};


struct Log {
    char message[100];
    struct Log* next;
};

struct directory directoryStruct;

void save(const char* fileName) {
    FILE* fileTar = fopen(fileName, "wb");
    if (fileTar == NULL) {
        printf("Error: No se pudo abrir el archivo \"%s\" para escritura.\n", fileName);
        return;
    }
    fwrite(&directoryStruct, sizeof(directoryStruct), 1, fileTar);


    for (int i = 0; i < directoryStruct.totalEntries ; i++) {

        char data[directoryStruct.fileList[i].size];

        FILE* file = fopen(directoryStruct.fileList[i].fileName, "rb"); 

        fread(data , sizeof(unsigned char), directoryStruct.fileList[i].size, file);

        fseek(fileTar, 0, directoryStruct.fileList[i].startByte);


        fwrite(data, sizeof(data), 1, fileTar);

    }
    fclose(fileTar);
    printf("Información guardada en el archivo \"%s\".\n", fileName);
}

void readFromFile(const char* fileName){
    FILE* file = fopen(fileName, "rb");
    if (file == NULL) {
      printf("Error: No se pudo abrir el archivo \"%s\" para lectura.\n", fileName);
      return;
    }
    fread(&directoryStruct, sizeof(directoryStruct), 1, file);
    fclose(file);
    printf("Información leída desde el archivo \"%s\".\n", fileName);

    printf("%i \n",directoryStruct.totalSize);
    printf("%i \n",directoryStruct.totalEntries);
}

int include(const char *option, char* flag){
  for (int i = 0; option[i] != '\0'; i++) { 
    if (strcmp(option, flag) == 0) {
      return 0;
    }
  }
  return 1;
}

char* file(char* argv[]){
    if(include(argv[2], "f")) {
        return argv[3];
    } else {
        return "prueba.tar";
    }
}

void extract(const char* tarName){
    FILE* tarFile = fopen(tarName, "rb");

    for(int i = 0; i < directoryStruct.totalEntries; i ++){
        struct file fileDirectory =  directoryStruct.fileList[i];

        char data[directoryStruct.fileList[i].size];

        const char* fileName = fileDirectory.fileName;
        
        FILE* file = fopen(fileName, "wb");
        if (file == NULL) {
            //insertLog(&headLog,"Error: No se pudo abrir el archivo \"%s\".\n", fileName);
            return;
        }
        
        fseek(tarFile, directoryStruct.fileList[i].startByte, SEEK_SET);
  

        fread(data , sizeof(unsigned char), directoryStruct.fileList[i].size, tarFile);
        


        fwrite(data, sizeof(data), 1, file);

        printf("Archivo extraido:%s" , fileName);

        printf("Archivo extraido:%s", fileName);

        fclose(file);
    }

    fclose(tarFile);

}

int findFile(const char* fileNameFound) {
  for(int i = 0; i < directoryStruct.totalEntries; i ++){
        struct file fileDirectory =  directoryStruct.fileList[i];
        const char* fileName = fileDirectory.fileName;
        int resultado = strcmp(fileNameFound, fileName);
        if (resultado == 0) {
            return i;
        }   
  } 
  return -1;
}

void delete(const char* tarName,const char* fileNameDelete){

    for(int i = 0; i < directoryStruct.totalEntries; i ++){
        struct file fileDirectory =  directoryStruct.fileList[i];
        const char* fileName = fileDirectory.fileName;
        int resultado = strcmp(fileNameDelete, fileName);
        if (resultado == 0) {
            for (int j = i; j < directoryStruct.totalEntries  - 1; j++) {
                directoryStruct.fileList[j] = directoryStruct.fileList[j + 1]; // Mover los elementos restantes
            }
            directoryStruct.totalEntries = directoryStruct.totalEntries - 1;
        }   
      }  
    FILE* fileTar = fopen(tarName, "r+");
    if (fileTar == NULL) {
        printf("Error: No se pudo abrir el archivo \"%s\" para escritura.\n", fileTar);
        return;
    }

    fwrite(&directoryStruct, sizeof(directoryStruct), 1, fileTar);


    fclose(fileTar); 
}

void append(const char* tarName,const char* fileNameAppend){
    int fileFound = findFile(fileNameAppend);
    int start = directoryStruct.sizeDirectory;
    int calc;
    int insert = 0;
    if (fileFound == -1) {
        struct file fileStruct;
        strcpy(fileStruct.fileName, fileNameAppend);
        FILE* file = fopen(fileNameAppend, "rb"); 
        // if (file == NULL) {
        //     insertLog(&headLog,"Error: No se pudo abrir el archivo \"%s\".\n", fileName);
        //     return;
        // }
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);
        fileStruct.size = fileSize;
        for(int i = 0; i < directoryStruct.totalEntries; i ++) {
            struct file fileDirectory =  directoryStruct.fileList[i];
            calc = fileDirectory.startByte - start;
            if (calc != 0 && fileSize <= calc) {
                fileStruct.startByte = start;
                fileStruct.endByte = start + fileSize;
                for (int j = directoryStruct.totalEntries; j > i; j--) {
                    directoryStruct.fileList[j] = directoryStruct.fileList[j - 1];

                }
                // Insertar el nuevo dato
                directoryStruct.fileList[i] = fileStruct;
                directoryStruct.totalEntries++;
                insert = 1;
                break;
            }
            start = fileDirectory.endByte;
        }
        FILE* tarFile = fopen(tarName, "r+");

        if (insert == 0) {

            fseek(tarFile, 0, SEEK_END);
            int tarSize = ftell(tarFile);
            rewind(tarFile);

            fileStruct.startByte = directoryStruct.fileList[directoryStruct.totalEntries-1].endByte;
            fileStruct.endByte = fileStruct.startByte + fileStruct.size;
            directoryStruct.totalSize = tarSize;
            directoryStruct.fileList[directoryStruct.totalEntries] = fileStruct;
            directoryStruct.totalEntries++;
        }

        

 

        char data[fileStruct.size];

        fread(data , sizeof(unsigned char), fileStruct.size, file);

        fwrite(&directoryStruct, sizeof(directoryStruct), 1, tarFile);

        fseek(tarFile, fileStruct.startByte, SEEK_SET);

        fwrite(data, sizeof(data), 1, tarFile);

        fclose(file);
        fclose(tarFile);

    } else {
        printf("El archivo ya que encontraba dentro del directorio");
    }
}


void update(const char* tarName,const char* fileNameUpdate){
    int fileNamePos = findFile(fileNameUpdate);
    if(fileNamePos == -1){
        //insertLog(&headLog,"Error: No se pudo actualizar el archivo \"%s\", no existe.\n", fileNameUpdate);
        return;
    }

    FILE* file = fopen(fileNameUpdate, "rb");        //newfile
   
    
    fseek(file, 0, SEEK_END);
    long NewfileSize = ftell(file);
    rewind(file);

    int difference = directoryStruct.fileList[fileNamePos + 1].startByte - directoryStruct.fileList[fileNamePos].endByte; // si es 0 va corrido

    int differenceSize = NewfileSize - directoryStruct.fileList[fileNamePos].size;


    if(NewfileSize <= directoryStruct.fileList[fileNamePos].size || difference >= differenceSize){
        delete(tarName, fileNameUpdate);
        append(tarName, fileNameUpdate);
        //log
    } else {
        

        FILE* tarFile = fopen(tarName, "rb+");          //tar

        struct file fileStruct = directoryStruct.fileList[fileNamePos];

        int sizeSave = directoryStruct.totalSize - directoryStruct.fileList[fileNamePos].endByte;

        char saveOld[sizeSave];
        char newInfo[NewfileSize];

        fread(newInfo, sizeof(unsigned char), NewfileSize, file);   //info nuevo archivo

        fseek(tarFile, fileStruct.endByte, SEEK_SET);

        fread(saveOld, sizeof(unsigned char), sizeSave, tarFile);    //info vieja directorio

        fseek(tarFile, fileStruct.startByte, SEEK_SET);
        fwrite(newInfo, sizeof(newInfo), 1, tarFile);

        fileStruct.endByte = fileStruct.startByte + NewfileSize;
        fileStruct.size = NewfileSize;

        fseek(tarFile, fileStruct.endByte, SEEK_SET);

        fwrite(saveOld, sizeof(saveOld), 1, tarFile);
        directoryStruct.fileList[fileNamePos] = fileStruct;

        for(int i = fileNamePos + 1; i < directoryStruct.totalEntries; i ++){
            struct file fileAfter = directoryStruct.fileList[i];
            fileAfter.startByte = fileAfter.startByte + differenceSize;
            fileAfter.endByte = fileAfter.endByte + differenceSize;
            directoryStruct.fileList[i] = fileAfter;
        }
        
        directoryStruct.totalSize = directoryStruct.totalSize + differenceSize;
        rewind(tarFile);

        fwrite(&directoryStruct, sizeof(directoryStruct), 1, tarFile);

        fclose(tarFile);
    }
    fclose(file);
    

}

void filePack(const char** filesName, int numFiles){

    directoryStruct.totalEntries = numFiles;
    directoryStruct.totalSize = 0;

    for (int i = 0; i < numFiles; i++) {
        
        struct file fileStruct;
        const char* fileName = filesName[i];

        strcpy(fileStruct.fileName, fileName);

        FILE* file = fopen(fileName, "rb"); 
        // if (file == NULL) {
        //     insertLog(&headLog,"Error: No se pudo abrir el archivo \"%s\".\n", fileName);
        //     return;
        // }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        fileStruct.size = fileSize;
 
        directoryStruct.totalSize = directoryStruct.totalSize + fileSize;

        // if (file.data  == NULL) {
        //         insertLog(&headLog,"Error: No se pudo asignar memoria para los datos del bloque.\n");
        //         fclose(file);
        //         return;
        //     }
        fclose(file);
        directoryStruct.fileList[i] =  fileStruct;
    }
    directoryStruct.sizeDirectory = sizeof(directoryStruct);
    directoryStruct.totalSize = directoryStruct.totalSize +directoryStruct.sizeDirectory;
    int start = directoryStruct.sizeDirectory;
    for (int i = 0; i < numFiles; i++) {
        directoryStruct.fileList[i].startByte = start;
        directoryStruct.fileList[i].endByte = directoryStruct.fileList[i].startByte + directoryStruct.fileList[i].size;
        start = directoryStruct.fileList[i].endByte;
    }
}

int main(int argc, char* argv[]) {

    if (argc < 4) {
        printf("Uso: star <opciones> <archivoSalida> <archivo1> <archivo2> ... <archivoN>\n");
        return 1;
    }

    const char *option = argv[2];
    char* archiveName = file(argv);

    if((strlen(option)>= 2) && option[0] == '-' && option[1] == '-') {
        //insertLog(&headLog,"Ejecución del comando: %c\n", option);
        
        if(strcmp(option, "--file") == 0){
            filePack((const char**)argv + 4, argc - 4);
            save(archiveName);
        }else if(strcmp(option, "--create") == 0){
            save(archiveName);
        }else {
            readFromFile(archiveName);
        }
        

        if(strcmp(option, "--extract") == 0){
            extract(archiveName);
        }

        if(strcmp(option, "--delete") == 0){
            delete(archiveName,argv[4]);
        }

        if(strcmp(option, "--append") == 0){
            append(archiveName,argv[4]);
        }

        if(strcmp(option, "--update") == 0){
            update(archiveName,argv[4]);
        }


    }
    return 0;
}