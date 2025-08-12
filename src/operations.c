#include "../headers/operations.h"

Operation opTable[OP_TABLE_SIZE] = {
    {"mov",  0,  2, {0,1,2,3},{1,2,3}},
    {"cmp",  1,  2, {0,1,2,3},{0, 1,2,3}},
    {"add",  2,  2, {0,1,2,3},{1,2,3}},
    {"sub",  3,  2, {0,1,2,3},{1,2,3}},
    {"not",  4,  1, NULL     ,{1,2,3}},
    {"clr",  5,  1, NULL     ,{1,2,3}},
    {"lea",  6,  2, {1,2}    ,{1,2,3}},
    {"inc",  7,  1, NULL     ,{1,2,3}},
    {"dec",  8,  1, NULL     ,{1,2,3}},
    {"jmp",  9,  1, NULL     ,{1,2,3}},
    {"bne",  10, 1, NULL     ,{1,2,3}},
    {"red",  11, 1, NULL     ,{1,2,3}},
    {"prn",  12, 1, NULL     ,{0,1,2,3}},
    {"jsr",  13, 1, NULL     ,{1,2,3}},
    {"rts",  14, 0, NULL     ,NULL},
    {"stop", 15, 0, NULL     ,NULL}
};


Operation *getOperationByName(char *opName){
    int i;
    Operation *op;

    printf("Inside getOperationByName\n");
    op = (Operation *)NULL;

    for(i = 0; i < OP_TABLE_SIZE; i++){
        if(!strcmp(opName,opTable[i].name)){
            op = (Operation *)malloc(sizeof(Operation));
            strcpy(op->name, opTable[i].name);
            op->opCode = opTable[i].opCode;
            printf("return opp\n");
            return op;
        }
    }

    yieldError("opNameNotFound", opName);
    return op;
}

Bool oppRouter(Operation *opp, char *line){
    printf("inside oppRouter\n");

    switch (opp->numOfArgs)
    {
    case 0: return handleZeroArgsOpp(opp, line);
    case 1: return handleOneArgOpp(opp, line);
    case 2: return handleTwoArgsOpp(opp, line);
    default:
            yieldError("unsupportedArgCount", opp->name);
            return FALSE;
    }
}

Bool handleZeroArgsOpp(Operation *opp, char *line){
    char *rest, *lineCopy;
    printf("inside handleZeroArgsOpp\n");

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);


    rest = strtok(lineCopy, " \n\t"); /*extract the command*/
    if((rest = strtok(NULL, " \n\t")) != NULL){
        yieldError("unexpectedChrsAfterCommand", rest, opp->name);
        return FALSE;
    }

    switch (opp->opCode)
    {
    case 14:
        /* code */
        break;

    /* stop */
    case 15: handleStopOpp();
    
    default:
        break;
    }

    return 0;
    
}

Bool handleOneArgOpp(Operation *opp, char *line){
    char *dest, *lineCopy;
    int destAddMode;

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    if(!parseOneOperand(lineCopy,&dest)){
        yieldError("invalidArgsForCommand", opp->name);
        return FALSE;
    }

    destAddMode = getAddressingMode(dest);
    if (!isValidAddrMode(opp->opCode, NULL,destAddMode))
    {
        yieldError("illegalAddressingMode", opp->name);
        return FALSE;
    }

    if(encodeInstruction(opp->opCode,NULL, destAddMode, NULL, dest)){
        return TRUE;
    }

    return FALSE;
}

Bool handleTwoArgsOpp(Operation *opp, char *line){
    char *src, *dest, *lineCopy;
    int srcAddMode, destAddMode;

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    if(!parseTwoOperands(lineCopy, &src, &dest)){
        yieldError("invalidArgsForCommand", opp->name);
        free(lineCopy);
        return FALSE;
    }

    srcAddMode = getAddressingMode(src);
    destAddMode = getAddressingMode(dest);

    if(!isValidAddrMode(opp->opCode, srcAddMode, destAddMode)){
        yieldError("illegalAddressingMode", opp->name);
        free(lineCopy);
        return FALSE;
    }

    if(encodeInstruction(opp->opCode,srcAddMode, destAddMode, src, dest)){
        free(lineCopy);
        return TRUE;
    }

    return FALSE;
}



Bool handleRtsOpp(void){
    return 0;
}

void handleStopOpp(void){
    printf("Stopping the program...\n");
    exit(EXIT_SUCCESS);
}

Bool parseOneOperand(char *line, char **dest){
    char *lineCopy, *arg;

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    arg = strtok(lineCopy," \t"); /*remove the command*/
    if((arg = strtok(NULL, " \t\n")) == NULL){
        return FALSE;
    }

    trimWhiteSpaces(&arg);

    if((dest = malloc(strlen(arg) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(*dest,arg);

    free(lineCopy);

    return TRUE;
}

Bool parseTwoOperands(char *line, char **src, char **dest){
    char *lineCopy, *tempSrc, *tempDest;

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    tempSrc = strtok(lineCopy, " \t"); /*remove the command*/
    if((tempSrc = strtok(NULL, ",")) == NULL){
        yieldError("noOperandFound");
        free(lineCopy);
        return FALSE;
    }

    if((tempDest = strtok(NULL," \n\t")) == NULL){
        yieldError("noOperandFound");
        free(lineCopy);
        return FALSE;
    }

    if(((*src) = malloc(strlen(tempSrc) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        free(lineCopy);
        return FALSE;
    }

    if(((*dest) = malloc(strlen(tempDest) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        free(*src);
        free(lineCopy);
        return FALSE;
    }

    trimWhiteSpaces(&tempSrc);
    trimWhiteSpaces(&tempDest);

    strcpy(*src,tempSrc);
    strcpy(*dest,tempDest);

    if(src != NULL && dest != NULL){
        free(lineCopy);
        return TRUE;
    }

    free(lineCopy);
    return FALSE;
}



int getAddressingMode(char *arg){
    Label *temp;
    char argTemp[strlen(arg)];
    char *rest;

    if(arg[0] == 'r' && (arg[1] >= '0' && arg[1] <= '7')) return REGISTER_ADDRESSING;

    if(arg[0] == '#') return INSTANT_ADDRESSING;

    if(isMatrixFormat(arg)) return MATRIX_ADDRESSING;

    if(isLabelName(arg)) return DIRECT_ADDRESSING;

    if(isLabelNameValid(arg)){
        if(!addLabelPlaceholder(arg)){
            yieldError("labelNotAdded",arg);
            return INVALID_ADDRESSING;
        }else return DIRECT_ADDRESSING;
    }

    return INVALID_ADDRESSING;

}

Bool isMatrixFormat(char *arg){
    char *firstOpen, *secondOpen;

    if((firstOpen = strchr(arg,'[')) != NULL){
        if((secondOpen = strchr(firstOpen + 1,'[')) != NULL){
            return TRUE;
        }
    }

    return FALSE;
}

Bool isValidAddrMode(int opCode, int srcAddrMode, int destAddrMode){
    Operation *op;
    int i = 0, srcValid = FALSE, destValid = FALSE;


    op = getOppByOpcode(opCode);

    if(op != NULL){
        if(op->leagalSrcModes != NULL){
            while ((op->leagalSrcModes[i]) != NULL)
            {
                if(srcAddrMode == op->leagalSrcModes[i]){
                    srcValid = TRUE;
                    break;
                }
                i++;
            }
        }else{
            if(srcAddrMode == NULL){
                srcValid = TRUE;
            }
        }

        if((op->leagalDestModes != NULL)){
            i = 0;
            while ((op->leagalDestModes[i]) != NULL)
            {
                if(destAddrMode == op->leagalDestModes[i]){
                    destValid = TRUE;
                    break;
                }
                i++;
            }
        }else{
            if(destAddrMode == NULL){
                destValid = TRUE;
            }
        }

        if (srcValid && destValid)
        {
            return TRUE;
        }
        

    }else{
        return FALSE;
    } 
}

Operation *getOppByOpcode(int opCode){
    int i;
    Operation *op = (Operation *)NULL;

    for (i = 0; i < OP_TABLE_SIZE; i++){
        if(opTable[i].opCode == opCode){
            op = &opTable[i];
            return op;
        }
    }
    return op;
}

Bool encodeInstruction(int opCode, int srcAddMode, int destAddMode,char *src, char *dest){

    unsigned short firstWord;

    if(!allocInstructionImg(srcAddMode,destAddMode)) return FALSE;

    firstWord = build_first_word(opCode,srcAddMode, destAddMode, ARE_A);
    addWordToInstractionImg(firstWord,ARE_A,&instructionsImage,&IC);

    return TRUE;

}
