#ifndef TABLES_H
#define TABLES_H

#include "headers.h"

#define INITIAL_CAPACITY 1


typedef struct DataImageEntry_s
{
    int address;
    short int word;
}DataImageEntry;

extern unsigned short instructionsImage[];
extern DataImageEntry *dataImage;
extern int dataImageCap;

typedef enum {
    ARE_A = 0, 
    ARE_E = 1, 
    ARE_R = 2 
} AREFlag; /* A=00, E=01, R=10 */

Bool addDataToDataImage(char *line);
Bool addStrToDataImage(char *line);
Bool addMatToDataImage(int matSize, int argsCount, int *matNums);
Bool initiateDataImage(void);
void printDataImg(void);


#endif
