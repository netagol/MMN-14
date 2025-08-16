#include "../headers/tables.h"

DataImageEntry *dataImage = NULL;
unsigned short *instructionsImage;

Bool initiateDataImage(void){
    dataImage = (DataImageEntry *)calloc(INITIAL_CAPACITY,sizeof(DataImageEntry));

    if (dataImage == NULL)
    {
        yieldError("dataImgInitFailed");
        return FALSE;
    }

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

    if (getDC() == 0)
    {
        if(!initiateDataImage()) {
            free(lineCopy);
            return FALSE;
        }
    }

    argCount = countArgs(line);
    
    if ((dataImage = realloc(dataImage, (getDC() + argCount) * sizeof(DataImageEntry))) == NULL)
    {
        yieldError("dataImgReallocFailed");
        free(lineCopy);
        return FALSE;
    }
    
    num_s = strtok(lineCopy,",");

    while (num_s != NULL)
    {
        num_i = (short int)atoi(num_s);
        dataImage[DC].word = num_i;
        dataImage[DC].address = getDC();
        increaseDC(1);

        num_s = strtok(NULL, ",");
    }

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
        dataImage[DC].word = (short int)(lineCopy[i]);
        dataImage[DC].address = getDC();
        increaseDC(1);
    }

    free(lineCopy);

    return TRUE;

}

Bool addMatToDataImage(int matSize, int argsCount, int *matNums){

    int i;

    printf("inside addMatToDataImage, matSize: %d, argsCount: %d\n", matSize,argsCount);

    if (getDC() == 0)
    {
        if(!initiateDataImage()) return FALSE;
    }else{
        if ((dataImage = realloc(dataImage, (getDC() + matSize) * sizeof(DataImageEntry))) == NULL)
        {
            yieldError("dataImgReallocFailed");
            return FALSE;
        }
    }

    if (argsCount){
        for(i = 0; i < argsCount; i++){
            printf("matWord: %d\n", (short int)matNums[i]);
            dataImage[DC].word = (short int)matNums[i];
            dataImage[DC].address = getDC();
            increaseDC(1);
        }
    }
    
    return TRUE;
    
}

void printDataImg(void){
    int i;

    printf("Data image: \n");

    for(i = 0; i < DC; i++){
        if(dataImage[i].address >= 0){
            printf("Address: %4d | word: ",dataImage[i].address);        
            printBinary(dataImage[i].word);
            printf("\n");
        }else{
            printf("Address:      | word: ");
            printBinary(dataImage[i].word);
            printf("\n");   
        }
    }
    printf("\n");
}

void printInstructionImg(void){
    int i;

    printf("Instruction image:\n");

    for(i = 0; i < (IC - IC_START); i++){
        printf("Address: %4d | word: ",i + IC_START);
        if (instructionsImage[i] >= 0)
        {
            printBinary(instructionsImage[i]);
            printf("\n");
        }
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
        destAddMode == REGISTER_ADDRESSING
    ) numOfWords++;
    else if(destAddMode == MATRIX_ADDRESSING) numOfWords += 2;
    
    if(getIC() == IC_START){
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

unsigned short buildFirstWord(int opCode, int srcMode, int dstMode, AREFlag are){
    unsigned short w = 0;
    w |= ((opCode & FOUR_BIT_MASK) << 6);
    w |= ((srcMode & TWO_BIT_MASK) << 4);
    w |= ((dstMode & TWO_BIT_MASK) << 2);
    w |= (are & TWO_BIT_MASK);

    return w;
}

unsigned short buildRegWord(int srcReg, int destReg){
    unsigned short w = 0;

    if(destReg >= 0) w |= ((destReg & FOUR_BIT_MASK) << 2);
    if(srcReg >= 0) w |= ((srcReg & FOUR_BIT_MASK) << 6);
    return w;
}

void addWordToInstractionImg(unsigned short val, AREFlag are, unsigned short *img){
    unsigned short w; 
    w = (val & CLEAR_ARE_MASK) | (are & TWO_BIT_MASK);
    img[getIC() - IC_START] = w;
    increaseIC(1);
}

void updateDataImgAddrs(void){
    int i;
    for(i = 0; i < DC; i++){
        dataImage[i].address += IC;
    }
}



