#include "processQfunc.h"
//Per ora legge tutto il file
//Effettua il conteggio dei caratteri del file e ritorna la stringa formato
char *computeCountingOnFile(int fileDescriptor, int idFile, int offset, int end){
  int *ASCII = (int *)calloc(ALPHABET_SIZE, sizeof(int));
  char carattere[1];
  char *datiParziali = (char *)calloc(PIPE_BUF + 1,  sizeof(char*));

  errorSysCall(lseek(fileDescriptor,offset,SEEK_SET));
  int counterChar = 0;
  while(errorSysCall(read(fileDescriptor,carattere,1)) && counterChar<(end-offset)){
    int ascii = (int)(carattere[0]);
    ASCII[ascii]++;
    counterChar++;
  }
  sprintf(datiParziali,"%d ",idFile);
  int i=0;
  while(i< ALPHABET_SIZE){
    sprintf(datiParziali,"%s %d ", datiParziali, ASCII[i]);
    i++;
  }
  sprintf(datiParziali, "%s\n", datiParziali);
  free(ASCII);
  return datiParziali;
}

//Ritorna la dimensione del file
int fileSize(int fileDescriptor){
  char *carattere = (char*)malloc(sizeof(char));
  int count=0;
  while(read(fileDescriptor,carattere,1)){
    count++;
  }
  return count;
}

//Stampa la stringa formato
void printFormatString(char **format){
  int i=0;
  while(i<258){
    printf("%s",format[i]);
    i++;
  }
}

//Controlla se le statistiche di quel file sono già state scritte nella pipe
//A idFile va passata i-ARGS_P_START_FILE_OFFSET
int isWrittenFile(int *writtenFiles,int idFile){
  if(writtenFiles[idFile]==1){
    return 1;
  }
  return 0;
}

//Calcola l'offset
int computeOffset(int parte, int denominatore, int size){
	int offset = ((double)(parte)/((double)denominatore)*size);
  return offset;
}

//Calcola la posizione dell'ultimo carattere che Q non deve leggere
int computeEnd(int parte, int denominatore, int size){
  int termine;
  if(parte<denominatore){
    termine = ((double)(parte + 1)/((double)denominatore)*size); //MODIFICATO
  }else{
    termine = size;
  }
  return termine;
}

//Calcola la dimensione del file
int computeSize(int fileDescriptor){
	 return errorSysCall(lseek(fileDescriptor,0,SEEK_END));
}

void readInput(int argc, char *argv[], int *parte, int *denominatore, int *pipeRead, int *pipeWrite, int *pipeReadOnTheFly, int *pipeWriteOnTheFly){
	(*parte) = atoi(argv[1]);
  (*denominatore) = atoi(argv[2]);
  (*pipeRead) = atoi(argv[3]);
  (*pipeWrite) = atoi(argv[4]);
  (*pipeReadOnTheFly) = atoi(argv[5]);
  (*pipeWriteOnTheFly) = atoi(argv[6]);
}

int openFile(char *name){
	int last = strlen(name) ;
	char temp[PATH_MAX + 1] = "";
	strncpy(temp, name, last - 1);
	return errorOpenInQ(open(temp, O_RDONLY), temp);
}

void addHandler(int pipeReadOnTheFly, int pipeWrite, int counterFilesOnTheFly, int parte, int denominatore, char *message){
  int byteRead = -1;
  do{
    byteRead = read(pipeReadOnTheFly,message,PIPE_BUF);
    if(byteRead>0 && strcmp(message, MOD_END)){
      int fd = openFile(message);
      int size = computeSize(fd);
      int offset = computeOffset(parte,denominatore,size);
      int end = computeEnd(parte,denominatore,size);
      char *format = computeCountingOnFile(fd,counterFilesOnTheFly,offset,end);
      errorSysCall(write(pipeWrite,format, PIPE_BUF));
    }
  }while(strcmp(message,MOD_END) && byteRead!=0);
}

void removeHandler(int pipeReadOnTheFly, int *removedFiles, int *writtenFiles, char *message, int argc, char *argv[]){
  int byteRead = -1;
  do{
    byteRead = read(pipeReadOnTheFly,message,PIPE_BUF);
    if(byteRead>0 && strcmp(message, MOD_END)){
      if(!isWrittenFile(writtenFiles,idFile(message,argc,argv)-ARGS_P_START_FILE_OFFSET)){
        removedFiles[idFile(message,argc,argv)-ARGS_P_START_FILE_OFFSET] = 1;
      }
    }
  }while(strcmp(message,MOD_END) && byteRead!=0);
}

int idFile(char *file, int argc, char *argv[]){
  int i=ARGS_Q_START_FILE_OFFSET;
  while(i<argc){
    if(!strcmp(argv[i],file)){
      return i-1;
    }
    i++;
  }
}
