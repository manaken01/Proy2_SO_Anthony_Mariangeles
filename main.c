//Proyecto 2 de sistema de archivos, memoria contigua
//Mariangeles Carranza Varela y Anthony Jimenez Barrantes
//Curso Sistemas Operativos
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#define MAX_FILENAME_LENGTH 20
#define MAX_FILE_ENTRIES 100

struct file {
    char fileName[MAX_FILENAME_LENGTH];         //Nombre del archivo
    int size;                                   //Tamano del archivo
    int startByte;                              //Byte de inicio del archivo en el directorio
    int endByte;                                //Byte de final del archivo en el directorio
};

struct directory {
    int totalSize;                              //Tamano total de todo
    int sizeDirectory;                          //Tamano del directorio
    int totalEntries;                           //Total de archivos en el directorio
    struct file fileList[MAX_FILE_ENTRIES];     //Lista de los archivos en el directorio
};

// Definición de la estructura de registro de logs
struct LogEntry {
    char message[200];
    struct LogEntry* next;
};

// Puntero a la cabeza de la lista de registros de logs 1, informacion general
struct LogEntry* logList1 = NULL;
struct LogEntry* logListEnd1 = NULL; // Puntero al final para printear en orden
// Puntero a la cabeza de la lista de registros de logs 2, informacion detallada
struct LogEntry* logList2 = NULL;
struct LogEntry* logListEnd2 = NULL; // Puntero al final de la lista detallada para printear en orden
struct directory directoryStruct;

//-------------------------------------------------------------
// Función para agregar un registro de log
void addLog(const char *message, int flag) {
    struct LogEntry* newLog = (struct LogEntry*)malloc(sizeof(struct LogEntry));
    if (newLog == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el registro de log.\n");
        exit(1);
    }
    snprintf(newLog->message, sizeof(newLog->message), "%s\n", message);
    newLog->next = NULL;

    if(flag == 1){
        if (logList1 == NULL) {
            logList1 = newLog;
            logListEnd1 = newLog;
        } else {
            logListEnd1->next = newLog;
            logListEnd1 = newLog;
        }

    }else{
        if (logList2 == NULL) {
        logList2 = newLog;
        logListEnd2 = newLog;
        } else {
            logListEnd2->next = newLog;
            logListEnd2 = newLog;
        }

    };
}
// Función para imprimir todos los registros de logs
void printLogs(int flag) {
    struct LogEntry* current1 = logList1;
    struct LogEntry* current2 = logList2;
    if(flag == 1){
        printf("Registros de logs:\n");
        while (current1 != NULL) {
            printf("%s\n", current1->message);
            printf("---------\n");
            current1 = current1->next;
        }
    }else{
        printf("Registros de logs detallados:\n");
        while (current1 != NULL && current2 != NULL) {
            printf("%s\n", current1->message);
            printf("%s\n", current2->message);
            printf("---------\n");
            current1 = current1->next;
            current2 = current2->next;
        }
    }
}

void freeLogs() {
    struct LogEntry* current1 = logList1;
    struct LogEntry* current2 = logList2;
    while (current1 != NULL && current2 != NULL) {
        struct LogEntry* temp1 = current1;
        struct LogEntry* temp2 = current2;
        current1 = current1->next;
        current2 = current2->next;
        free(temp1);
        free(temp2);
    }
    logList1 = NULL;
    logList2 = NULL;
    logListEnd1 = NULL;
    logListEnd1 = NULL;
}
//-----------------------------------------------
void listFiles(){
    // imprimir la lista de archivos y su información
    addLog("Comando list, imprimiendo la lista de archivos y su información\n",1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);

    printf("\nLista de archivos:\n");
    for(int i = 0; i < directoryStruct.totalEntries; i ++){
        printf("Nombre: %s\n", directoryStruct.fileList[i].fileName);
        printf("Primer Byte: %d\n", directoryStruct.fileList[i].startByte);
        printf("Último Byte: %d\n", directoryStruct.fileList[i].endByte);
        printf("Tamaño: %d\n", directoryStruct.fileList[i].size);
        printf("---------\n");
    }
   
}
void save(const char* fileName) {
    FILE* fileTar = fopen(fileName, "wb");
    if (fileTar == NULL) {
        char message1[100] = "Error: no se pudo abrir tar para escritura: \n";
        strcat(message1, fileName);
        addLog(message1,1);
            
        char message[200];
        snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
        addLog(message,2);
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

    char message1[100] = "Informacion guardada en el archivo: \n";
    strcat(message1, fileName);
    addLog(message1,1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);
}

void readFromFile(const char* fileName){
    FILE* file = fopen(fileName, "rb");
    if (file == NULL) {
        char message1[100] = "Error: no se pudo abrir archivo para escritura: \n";
        strcat(message1, fileName);
        addLog(message1,1);
            
        char message[200];
        snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
        addLog(message,2);
        return;
    }
    fread(&directoryStruct, sizeof(directoryStruct), 1, file);
    fclose(file);

    char message1[100] = "Informacion leida del archivo: \n";
    strcat(message1, fileName);
    addLog(message1,1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);
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

//--------------------------------------------------
void extract(const char* tarName){
    FILE* tarFile = fopen(tarName, "rb");
    if (tarFile == NULL) {
            char message1[100] = "Comando extract, error, no se pudo abrir tar para escritura: \n";
            strcat(message1, tarName);
            addLog(message1,1);
                
            char message[200];
            snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
            addLog(message,2);
            return;
        }
    for(int i = 0; i < directoryStruct.totalEntries; i ++){
        struct file fileDirectory =  directoryStruct.fileList[i];

        char data[directoryStruct.fileList[i].size];

        const char* fileName = fileDirectory.fileName;
        
        FILE* file = fopen(fileName, "wb");
        if (file == NULL) {
            char message1[100] = "Comando extract, error, no se pudo abrir archivo para escritura: \n";
            strcat(message1, fileName);
            addLog(message1,1);
                
            char message[200];
            snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
            addLog(message,2);
            return;
        }
        
        fseek(tarFile, directoryStruct.fileList[i].startByte, SEEK_SET);
  

        fread(data , sizeof(unsigned char), directoryStruct.fileList[i].size, tarFile);
        
        fwrite(data, sizeof(data), 1, file);

        fclose(file);
    }

    fclose(tarFile);

    addLog("Comando extract, informacion extraida del tar\n",1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);
}

void delete(const char* tarName,const char* fileNameDelete){
    // Verificar si eiste falta
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
        char message1[100] = "Comando delete, error, no se pudo abrir tar par escritura: \n";
        strcat(message1, tarName);
        addLog(message1,1);
            
        char message[200];
        snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
        addLog(message,2);
        return;
    }

    fwrite(&directoryStruct, sizeof(directoryStruct), 1, fileTar);
    fclose(fileTar); 

    char message1[100] = "Comando delete, archivo borrado del directorio: \n";
    strcat(message1, fileNameDelete);
    addLog(message1,1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);
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
        if (file == NULL) {
            char message1[100] = "Comando append, archivo inexistente: \n";
            strcat(message1, fileNameAppend);
            addLog(message1,1);
            
            char message[200];
            snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
            addLog(message,2);
        return;
        }
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
        char message1[100] = "Comando append, archivo agregado al directorio: \n";
        strcat(message1, fileNameAppend);
        addLog(message1,1);
        
        char message[200];
        snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
        addLog(message,2);
        
    } else {
        char message1[100] = "Comando append, archivo ya se encontraba dentro del directorio: \n";
        strcat(message1, fileNameAppend);
        addLog(message1,1);
        
        char message[200];
        snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
        addLog(message,2);
    }
}


void update(const char* tarName,const char* fileNameUpdate){
    int fileNamePos = findFile(fileNameUpdate);
    if(fileNamePos == -1){
        char message1[100] = "Comando update, archivo inexistente en el directorio: \n";
        strcat(message1, fileNameUpdate);
        addLog(message1,1);
        
        char message[200];
        snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
        addLog(message,2);
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


    char message1[100] = "Comando update, archivo actualizado: \n";
    strcat(message1, fileNameUpdate);
    addLog(message1,1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);
}

void pack(const char* tarName){
    FILE* tarFile = fopen(tarName, "rb+");          //tar
    int sizeTar = sizeof(directoryStruct);
    int difference;
    int start = sizeof(directoryStruct);
    if (directoryStruct.totalEntries == 0) {
        exit(0);
    } else {
        //char allData[directoryStruct.size];
        //struct file fileDirectory[MAX_FILE_ENTRIES] =  directoryStruct.fileList;
        //fread(allData, sizeof(unsigned char), directoryStruct.size, tarFile);    //info vieja directorio
        for(int i = 0; i < directoryStruct.totalEntries; i++) {
            struct file file1 =  directoryStruct.fileList[i];

            sizeTar = sizeTar + file1.size;
            

            int difference = file1.startByte - start; // si es 0 va corrido

            //printf("start: %i \n", file1.startByte);
            //printf("end: %i \n", start);

            //printf("aver: %i \n", difference);

            if (difference != 0) {
                fseek(tarFile, file1.startByte, SEEK_SET);
                long fileSize = file1.size;
                char oldData[fileSize];

                fread(oldData, sizeof(unsigned char), fileSize, tarFile);    //info vieja directorio

                file1.startByte = start;
                file1.endByte = start + file1.size;

                fseek(tarFile, file1.startByte, SEEK_SET);

                fwrite(oldData, sizeof(oldData), 1, tarFile);

                directoryStruct.fileList[i] = file1;
            }

            start = file1.endByte;

            if (i == directoryStruct.totalEntries-1) {
                directoryStruct.totalSize = sizeTar;
                rewind(tarFile);
                fwrite(&directoryStruct, sizeof(directoryStruct), 1, tarFile);
                rewind(tarFile);
                char lastSave[sizeTar];
                fread(lastSave, sizeof(unsigned char), sizeTar, tarFile);    //info vieja directorio

                fclose(tarFile);

                //printf("Esta en el final del archivo: %i \n", directoryStruct.totalEntries);

                FILE* tarFile = fopen(tarName, "wb");  
                
                fwrite(lastSave, sizeof(lastSave), 1, tarFile);

                fclose(tarFile);
                break;
            }
            

            }
        }
        
    addLog("Comando pack, archivo tar desfragmentado \n",1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);
}
   


void filePack(const char** filesName, int numFiles){

    directoryStruct.totalEntries = numFiles;
    directoryStruct.totalSize = 0;

    for (int i = 0; i < numFiles; i++) {
        
        struct file fileStruct;
        const char* fileName = filesName[i];

        strcpy(fileStruct.fileName, fileName);

        FILE* file = fopen(fileName, "rb"); 
        if (file == NULL) {
            char message1[100] = "Comando file, error al abrir archivo: \n";
            strcat(message1, fileName);
            addLog(message1,1);
            
            char message[200];
            snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
            addLog(message,2);
            return;
         }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        fileStruct.size = fileSize;
 
        directoryStruct.totalSize = directoryStruct.totalSize + fileSize;

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

    addLog("Comando file,archivos comprimidos\n",1);
    
    char message[200];
    snprintf(message, sizeof(message), "Información adicional del directorio: \nTamaño total: %i\nTamaño del directorio: %i\nNumero de archivos del directorio: %i\n", directoryStruct.totalSize,directoryStruct.sizeDirectory,directoryStruct.totalEntries);
    addLog(message,2);
}

int main(int argc, char* argv[]) {

    if (argc < 4) {
        printf("Uso: star <opciones> <archivoSalida> <archivo1> <archivo2> ... <archivoN>\n");
        return 1;
    }

    const char *option = argv[2];
    char* archiveName = file(argv);

    if((strlen(option)>= 2) && option[0] == '-' && option[1] == '-') {
        if(strcmp(option, "--file") == 0){
            filePack((const char**)argv + 4, argc - 4);
            save(archiveName);
            listFiles();
        }else if(strcmp(option, "--create") == 0){
            save(archiveName);
        }else {
            readFromFile(archiveName);
        }
        if(strcmp(option, "--list") == 0){
            listFiles();
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

        if(strcmp(option, "--pack") == 0){
            pack(archiveName);
        }
        if(strcmp(option, "--verbose") == 0){
            printLogs(1);
        }
    freeLogs();
    return 0;
    }
    
    for (int i = 1; option[i] != '\0' ; i++) { 
        if(option[i] == 'f'){
            filePack((const char**)argv + 4, argc - 4);
            save(archiveName);
            // listFiles();
        }else if(option[i] == 'c'){
            save(archiveName);
        }else {
            readFromFile(archiveName);
        }

      if(option[i] == 'x'){
        extract(archiveName);
      }
      if(option[i] == 't'){
        listFiles();
      }
      
      if(option[i] == 'u'){
        update(archiveName,argv[4]);
      }

      if(option[i] == 'r'){
        append(archiveName,argv[4]);
        
      }
      if(option[i] == 'p'){
        pack(archiveName);
        
      }
      if(option[i] == 'v' && option[i + 1] != 'v'){
        printLogs(1);
        }else if (option[i] == 'v' && option[i + 1] == 'v'){
            printLogs(2);
            i++;
        }
    }
    

    freeLogs();
    return 0;
}