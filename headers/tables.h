#ifndef TABLES_H
#define TABLES_H

#include "headers.h"

#define INITIAL_CAPACITY 1
#define FOUR_BIT_MASK 15 /*1111*/
#define TWO_BIT_MASK 3 /*11*/
#define CLEAR_ARE_MASK 4092 /*1111 1111 1100*/


typedef struct DataImageEntry_s
{
    int address;
    short int word;
}DataImageEntry;

extern unsigned short *instructionsImage;
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

Bool allocInstructionImg(int srcAddMode, int destAddMode);
unsigned short buildFirstWord(int opCode, int srcMode, int dstMode, AREFlag are);
unsigned short buildRegWord(int srcReg, int destReg);
void addWordToInstractionImg(unsigned short val, AREFlag are, unsigned short *img, int *IC);


#endif
