#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ALPHABET_SIZE 256

#define FIFO_NAME "/TestArea/Report/AtoR" 	//PlaceHolder

#define REPORT_FILE "/TestArea/Report/AtoR.txt" 	//PlaceHolder

#define SETN 0		//Code of command to set n

#define SETM 1		//Code of command to set m

#define HELP 2		//Code of command for help

#define SETREC 3	//Code for setting recursive search in directories

#define COMMAND 0	//Code of argument as command

#define NOTCOMMAND 1

#define FILES 0      	//Code for a found file in input

#define DIRECTORY 1	//Code for a found directory in input

#define MANPATH "/LabSO-AA_2019_2020--201856_187968_202002_188009/src/ManFiles/"	//Path of the help files PLACEHOLDER

#define PNAME "/TestArea/Executable/P.out"		//PLACEHOLDER

#define QNAME "/TestArea/Executable/Q.out"		//PLACEHOLDER

#define INTMAXCHAR 13					//TO VERIFY

#define READ 0

#define WRITE 1

#define END "\\end"					//Last message before closing pipe

#define ASCII_SIZE 256

#define ASCII_START 32

#define COLS 10						//FILIPPO DEFINISCI QUESTO

#define ARGS_P_START_FILE_OFFSET 4			//INDICE DI INIZIO FILE IN P

#define ARGS_Q_START_FILE_OFFSET 5			//INDICE DI INIZIO FILE IN Q
