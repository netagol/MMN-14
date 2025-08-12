#include "../headers/tables.h"

DataImageEntry *dataImage = NULL;
int dataImageCap;
unsigned short *instructionsImage;

Bool initiateDataImage(void){
    dataImage = (DataImageEntry *)calloc(INITIAL_CAPACITY,sizeof(DataImageEntry));

    if (dataImage == NULL)
    {
        yieldError("dataImgInitFailed");
        return FALSE;
    }

    dataImageCap = INITIAL_CAPACITY;

    return TRUE;
}

Bool addDataToDataImage(char *line){
    char *num_s, *lineCopy;
    short int num_i;
    int argCount;

    lineCopy = num_s = NULL;

    if((lineCopy = malloc((strlen(line)+1) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    printf("inside addDataToDataImage\n");

    strcpy(lineCopy,line);
    printf("1\n");

    if (getDC() == 0)
    {
        if(!initiateDataImage()) {
            free(lineCopy);
            return FALSE;
        }
    }
    printf("2\n");

    argCount = countArgs(line);
    
    if ((dataImage = realloc(dataImage, (getDC() + argCount) * sizeof(DataImageEntry))) == NULL)
    {
        yieldError("dataImgReallocFailed");
        free(lineCopy);
        return FALSE;
    }
    printf("3\n");
    
    num_s = strtok(lineCopy,",");
    printf("4\n");

    while (num_s != NULL)
    {
        num_i = (short int)atoi(num_s);
        dataImage[DC].word = num_i;
        dataImage[DC].address = getDC();
        printf("num = %d, argCount: %d\n", num_i, argCount);
        increaseDC(1);

        num_s = strtok(NULL, ",");
    }
    printf("5\n");

    free(lineCopy);
    
    return TRUE;
}

Bool addStrToDataImage(char *line){
    int i;
    char *lineCopy = NULL;

    printf("inside addStrToDataImage\n");

    if ((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL)
    {
        yieldError("memoryAllocationFailed");
        free(lineCopy);
        return FALSE;
    }
    

    strcpy(lineCopy,line);

    if (getDC() == 0)
    {
        if(!initiateDataImage()) {
            free(lineCopy);
            return FALSE;
        }
    }

    if ((dataImage = realloc(dataImage, (getDC() + strlen(lineCopy)) * sizeof(DataImageEntry))) == NULL)
    {
        yieldError("dataImgReallocFailed");
        free(lineCopy);
        return FALSE;
    }

    for(i = 1; i < strlen(lineCopy) -2; i++){
        printf("word: %c, (short int)word: %d\n", lineCopy[i], (short int)lineCopy[i]);
        dataImage[DC].word = (short int)(lineCopy[i]);
        dataImage[DC].address = getDC();
        increaseDC(1);
    }

    

    printf("retrun true\n");
    free(lineCopy);

    return TRUE;

}

Bool addMatToDataImage(int matSize, int argsCount, int *matNums){

    int i;

    printf("inside addMatToDataImage, matSize: %d, argsCount: %d\n", matSize,argsCount);
    printf("1\n");

    if (getDC() == 0)
    {
        printf("1.1\n");
        if(!initiateDataImage()) return FALSE;
    }else{
        printf("1.1.1\n"); 
        if ((dataImage = realloc(dataImage, (getDC() + matSize) * sizeof(DataImageEntry))) == NULL)
        {
            yieldError("dataImgReallocFailed");
            return FALSE;
        }
        printf("1.1.2\n");
    }
    printf("2\n");

    printf("3\n");

    if (argsCount){
        for(i = 0; i < argsCount; i++){
            printf("matWord: %d\n", (short int)matNums[i]);
            dataImage[DC].word = (short int)matNums[i];
            dataImage[DC].address = getDC();
            increaseDC(1);
        }
    }
    printf("4\n");
    
    return TRUE;
    
}

void printDataImg(void){
    int i;

    printf("Data image: \n");

    for(i = 0; i < DC; i++){
        printf("Address: %4d | word: ",dataImage[i].address);
        printBinary(dataImage[i].word);
        printf("\n");
    }
    printf("\n");
}

Bool allocInstructionImg(int srcAddMode, int destAddMode){
    int numOfWords = 1;

    /*src*/
    if(
        srcAddMode == INSTANT_ADDRESSING || 
        srcAddMode == DIRECT_ADDRESSING ||
        srcAddMode == REGISTER_ADDRESSING
    ) numOfWords++;
    else if ( srcAddMode == MATRIX_ADDRESSING) numOfWords += 2;

    /*dest*/
    if (
        destAddMode == INSTANT_ADDRESSING ||
        destAddMode == DIRECT_ADDRESSING ||
        srcAddMode == REGISTER_ADDRESSING
    ) numOfWords++;
    else if(destAddMode == MATRIX_ADDRESSING) numOfWords += 2;
    
    if(getIC == IC_START){
        if((instructionsImage = malloc(numOfWords * sizeof(short int))) == NULL){
            yieldError("memoryAllocationFailed");
            return FALSE;
        }
    }else{
        if((instructionsImage = realloc(instructionsImage, (IC + numOfWords) * sizeof(short int))) == NULL){
            yieldError("memoryAllocationFailed");
            return FALSE;
        }
    }
    return TRUE;
}

static unsigned short build_first_word(int opCode, int srcMode, int dstMode, AREFlag are){
    unsigned short w = 0;

    w |= ((opCode & FOUR_BIT_MASK) << 6);
    w |= ((srcMode & TWO_BIT_MASK) << 4);
    w |= ((dstMode & TWO_BIT_MASK) << 2);
    w |= (are & TWO_BIT_MASK);

    return w;
}

static unsigned short build_reg_word(int srcReg, int destReg){
    unsigned short w = 0;

    if(srcReg >= 0) w |= ((srcReg & TWO_BIT_MASK) << 2);
    if(destReg >= 0) w |= ((destReg & TWO_BIT_MASK) << 6);

    return w;
}

Bool addWordToInstractionImg(unsigned short val, AREFlag are, unsigned short *img, int *IC){
    unsigned short w; 
    w = (val & CLEAR_ARE_MASK) | (are & TWO_BIT_MASK);
    img[*IC - IC_START] = w;
    (*IC)++;
}

