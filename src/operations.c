#include "../headers/operations.h"

Operation opTable[OP_TABLE_SIZE] = {
    {"mov",  0,  2, {0,1,2,3,NULL_ADDRESSING},{1,2,3,NULL_ADDRESSING}},
    {"cmp",  1,  2, {0,1,2,3,NULL_ADDRESSING},{0,1,2,3,NULL_ADDRESSING}},
    {"add",  2,  2, {0,1,2,3,NULL_ADDRESSING},{1,2,3,NULL_ADDRESSING}},
    {"sub",  3,  2, {0,1,2,3,NULL_ADDRESSING},{1,2,3,NULL_ADDRESSING}},
    {"not",  4,  1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"clr",  5,  1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"lea",  6,  2, {1,2}                    ,{1,2,3}},
    {"inc",  7,  1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"dec",  8,  1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"jmp",  9,  1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"bne",  10, 1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"red",  11, 1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"prn",  12, 1, {NULL_ADDRESSING}        ,{0,1,2,3}},
    {"jsr",  13, 1, {NULL_ADDRESSING}        ,{1,2,3}},
    {"rts",  14, 0, {NULL_ADDRESSING}        ,{NULL_ADDRESSING}},
    {"stop", 15, 0, {NULL_ADDRESSING}        ,{NULL_ADDRESSING}}
};


Operation *getOperationByName(char *opName){
    int i;
    Operation *op;

    printf("Inside getOperationByName, opName: %s\n", opName);
    op = (Operation *)NULL;

    for(i = 0; i < OP_TABLE_SIZE; i++){
        if(!strcmp(opName,opTable[i].name)){
            op = &(opTable[i]);
            printf("return op\n");
            return op;
        }
    }

    yieldError("opNameNotFound", opName);
    return op;
}

Bool oppRouter(Operation *opp, char *line, int pass){

    printOperation(opp);

    trimWhiteSpacesAtStart(&line);
    printf("inside oppRouter, numOfArgs: %d, oppName: %s, line: %s\n", opp->numOfArgs, opp->name,line);

    switch (opp->numOfArgs)
    {
    case 0: return handleZeroArgsOpp(opp, line, pass);
    case 1: return handleOneArgOpp(opp, line, pass);
    case 2: return handleTwoArgsOpp(opp, line, pass);
    default:
            yieldError("unsupportedArgCount", opp->name);
            return FALSE;
    }
}

Bool handleZeroArgsOpp(Operation *opp, char *line, int pass){
    char *rest, *lineCopy;
    printf("inside handleZeroArgsOpp\n");

    if(pass == FIRST_PASS){
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

        addWordToInstractionImg(IC,ARE_A,instructionsImage);
        return TRUE;
    }else{
        increaseIC(ONE_IMG_WORD);
    }

    
}

Bool handleOneArgOpp(Operation *opp, char *line, int pass){
    char *dest, *lineCopy;
    int destAddMode;

    printf("Inside handleOneArgOpp\n");

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }
    strcpy(lineCopy,line);
    if(!parseOneOperand(lineCopy,&dest)){
        yieldError("invalidArgsForCommand", opp->name);
        free(lineCopy);
        return FALSE;
    }
    if((destAddMode = getAddressingMode(dest)) == INVALID_ADDRESSING){
        yieldError("illegalAddressingMode", opp->name);
        free(lineCopy);
        return FALSE;
    }
    if (!isValidAddrMode(opp->opCode, NULL_ADDRESSING ,destAddMode))
    {
        yieldError("illegalAddressingMode", opp->name);
        free(lineCopy);
        return FALSE;
    }
    if(encodeInstruction(opp->opCode,NULL_ADDRESSING, destAddMode, NULL, dest, pass)){
        free(lineCopy);
        return TRUE;
    }
    free(lineCopy);
    return FALSE;
}

Bool handleTwoArgsOpp(Operation *opp, char *line, int pass){
    char *src, *dest, *lineCopy;
    int srcAddMode, destAddMode;

    printf("Inside handleTwoArgsOpp, line: %s\n", line);
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
    printf("srcAddrMode: %d, destAddrMode: %d\n", srcAddMode,destAddMode);

    if(!isValidAddrMode(opp->opCode, srcAddMode, destAddMode)){
        yieldError("illegalAddressingMode", opp->name);
        free(lineCopy);
        return FALSE;
    }

    if(encodeInstruction(opp->opCode,srcAddMode, destAddMode, src, dest, pass)){
        free(lineCopy);
        return TRUE;
    }

    return FALSE;
}


Bool parseOneOperand(char *line, char **dest){
    char *lineCopy, *arg;

    printf("inside parseOneOperand\n");
    printf("1\n");
    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }
    printf("2\n");
    strcpy(lineCopy,line);
    printf("3\n");
    arg = strtok(lineCopy," \t"); /*remove the command*/
    printf("4\n");
    if((arg = strtok(NULL, " \t\n")) == NULL){
        return FALSE;
    }
    printf("5\n");

    trimWhiteSpaces(&arg);
    printf("6\n");

    if(((*dest) = malloc(strlen(arg) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }
    printf("7\n");

    strcpy(*dest,arg);
    printf("8\n");

    free(lineCopy);
    printf("9\n");

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
    printf("1. tempsrc: %s\n", tempSrc);
    if((tempSrc = strtok(NULL, ",")) == NULL){
        yieldError("noOperandFound");
        free(lineCopy);
        return FALSE;
    }
    printf("2. tempsrc: %s\n", tempSrc);

    if((tempDest = strtok(NULL," \n\t")) == NULL){
        yieldError("noOperandFound");
        free(lineCopy);
        return FALSE;
    }
    printf("2. dest: %s\n", tempDest);

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
    printf("inside getAddressingMode arg: %s\n", arg);

    if(arg[0] == 'r' && (arg[1] >= '0' && arg[1] <= '7')) return REGISTER_ADDRESSING;
    printf("0\n");
    if(arg[0] == '#') return INSTANT_ADDRESSING;
    printf("1\n");
    if(isMatrixFormat(arg)) return MATRIX_ADDRESSING;
    printf("2\n");
    if(isLabelName(arg)) return DIRECT_ADDRESSING;
    printf("3\n");
    if(isLabelNameValid(arg)){
        printf("labelNameValid!\n");
        if(!addLabelPlaceholder(arg)){
            yieldError("labelNotAdded",arg);
            return INVALID_ADDRESSING;
        }else return DIRECT_ADDRESSING;
    }
    printf("INVALID_ADDRESSING!\n");
    return INVALID_ADDRESSING;

}

Bool isMatrixFormat(char *arg){
    char *firstOpen, *secondOpen, *matName;

    printf("inside isMatrixFormat!\n");

    if((firstOpen = strchr(arg,'[')) != NULL){
        if((secondOpen = strchr(firstOpen + 1,'[')) != NULL){
            if((matName = malloc((strlen(arg)-strlen(firstOpen)) * sizeof(char))) == NULL){
                yieldError("memoryAllocationFailed");
                return FALSE;
            }

            strncpy(matName,arg,(strlen(arg)-strlen(firstOpen)));
            trimWhiteSpaces(&matName);
            printf("TRUE, matName: %s\n", matName);
            if(!isLabelName(matName)){
                if(isLabelNameValid(matName)){
                    if(!addLabelPlaceholder(matName)){
                        return FALSE;
                    }
                }
            }
            return TRUE;
        }
    }
    printf("FALSE\n");
    return FALSE;
}

Bool isValidAddrMode(int opCode, int srcAddrMode, int destAddrMode){
    Operation *op;
    int i = 0, srcValid = FALSE, destValid = FALSE;

    printf("inside isValidAddrMode, opCode: %d, srcAdd: %d, destAdd: %d\n", opCode, srcAddrMode, destAddrMode);
    op = getOppByOpcode(opCode);
    printOperation(op);

    if(op != NULL){
        if(op->numOfArgs == 2){
            while (op->leagalSrcModes[i] != NULL_ADDRESSING)
            {
                if(srcAddrMode == op->leagalSrcModes[i]){
                    srcValid = TRUE;
                    break;
                }
                i++;
            }
        }else{
            if(srcAddrMode == NULL_ADDRESSING){
                srcValid = TRUE;
            }
        }

        if((op->leagalDestModes[0] != NULL_ADDRESSING)){
            i = 0;
            while ((op->leagalDestModes[i]))
            {
                if(destAddrMode == op->leagalDestModes[i]){
                    destValid = TRUE;
                    break;
                }
                i++;
            }
        }else{
            if(destAddrMode == NULL_ADDRESSING){
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

    return FALSE;
}

Operation *getOppByOpcode(int opCode){
    int i;

    for(i = 0; i < OP_TABLE_SIZE; i++){
        if(opTable[i].opCode == opCode){
            return &opTable[i];
        }
    }
    return NULL;
}

Bool encodeInstruction(int opCode, int srcAddMode, int destAddMode,char *src, char *dest, int pass){

    unsigned short firstWord, word;
    long val, addr;
    int s, d, rowReg, colReg;
    Bool isExt;
    char label[MAX_LABEL_NAME];
    s = d = val = addr =0;

    if(pass == FIRST_PASS){
        printf("Inside encodeInstruction\n");
        printf("1\n");
        if(!allocInstructionImg(srcAddMode,destAddMode)) return FALSE;
        printf("2\n");
        firstWord = buildFirstWord(opCode,srcAddMode, destAddMode, ARE_A);
        printf("3 first Word = %d\n", firstWord);
        addWordToInstractionImg(firstWord, ARE_A, instructionsImage);
        printf("4\n");

        /*Extra words*/

        /*Both operands are registers*/
        if(srcAddMode == REGISTER_ADDRESSING && destAddMode == REGISTER_ADDRESSING){
            s = parseRegister(src);
            d = parseRegister(dest);
            word = buildRegWord(s,d);
            addWordToInstractionImg(word, ARE_A, instructionsImage);
            return TRUE;
        }
        printf("5\n");
        /*Source Operand*/
        if(srcAddMode == INSTANT_ADDRESSING){
            val = parseInstant(src);
            if(val < -MAX_SHORT_NUM || val > MAX_SHORT_NUM){
                yieldError("instantArgOutOfRang",val, MAX_SHORT_NUM);
                return FALSE;
            }
            word = (unsigned short)val;
            addWordToInstractionImg(word << 2, ARE_A, instructionsImage);
        }else if(srcAddMode == DIRECT_ADDRESSING){
            if(!(resolveLabel(src, &addr,&isExt))) return FALSE;
            addWordToInstractionImg((unsigned short)(addr << 2), isExt ? ARE_E : ARE_A, instructionsImage);
        }else if(srcAddMode == MATRIX_ADDRESSING){
            printf("5.1\n");
            /*first word - address*/
            if(!parseMatrix(src, label, &rowReg, &colReg)) return FALSE;
            printf("5.2 addr: %ld\n", addr);
            if(!resolveLabel(label, &addr, &isExt)) return FALSE;
            printf("5.3, addr: %ld\n", addr);
            addWordToInstractionImg((unsigned short)(addr <<2), isExt ? ARE_E : ARE_A, instructionsImage);
            printf("5.4\n");

            /*second word - registers*/
            word = buildRegWord(rowReg, colReg);
            addWordToInstractionImg(word, ARE_A, instructionsImage);

        }else if(srcAddMode == REGISTER_ADDRESSING){
            s = parseRegister(src);
            word = buildRegWord(s, INVALID_ADDRESSING);
            addWordToInstractionImg(word, ARE_A, instructionsImage);
        }

        /*Destination Operand*/
        if(destAddMode == INSTANT_ADDRESSING){
            val = parseInstant(dest);
            if(val < -MAX_SHORT_NUM || val > MAX_SHORT_NUM){
                yieldError("instantArgOutOfRang",val, MAX_SHORT_NUM);
                return FALSE;
            }
            word = (unsigned short)val;
            addWordToInstractionImg(word << 2, ARE_A, instructionsImage);
        }else if(destAddMode == DIRECT_ADDRESSING){
            if(!(resolveLabel(dest, &addr,&isExt))) return FALSE;
            printf("direct addr: %ld\n", addr << 2);
            printBinary((short)(addr << 2));
            printf("\n");
            addWordToInstractionImg((unsigned short)(addr << 2), isExt ? ARE_E : ARE_A, instructionsImage);
        }else if(destAddMode == MATRIX_ADDRESSING){
            printf("5.1\n");
            /*first word - address*/
            if(!parseMatrix(dest, label, &rowReg, &colReg)) return FALSE;
            printf("5.2 addr: %ld\n", addr);
            if(!resolveLabel(label, &addr, &isExt)) return FALSE;
            printf("5.3, addr: %ld\n", addr);
            addWordToInstractionImg((unsigned short)(addr <<2), isExt ? ARE_E : ARE_A, instructionsImage);
            printf("5.4\n");

            /*second word - registers*/
            word = buildRegWord(rowReg, colReg);
            addWordToInstractionImg(word, ARE_A, instructionsImage);

        }else if(destAddMode == REGISTER_ADDRESSING){
            d = parseRegister(dest);
            word = buildRegWord(d, INVALID_ADDRESSING);
            addWordToInstractionImg(word, ARE_A, instructionsImage);
        }


    }else if(pass == SECOND_PASS){
        increaseIC(ONE_IMG_WORD);/*pass the command*/

        if(srcAddMode == REGISTER_ADDRESSING && destAddMode == REGISTER_ADDRESSING){
            increaseIC(ONE_IMG_WORD);
            return TRUE;
        }

        if(srcAddMode == INSTANT_ADDRESSING){
            increaseIC(ONE_IMG_WORD);
        }else if(srcAddMode == DIRECT_ADDRESSING){
            if(!(resolveLabel(src, &addr,&isExt))) return FALSE;
            increaseIC(ONE_IMG_WORD);
            
        }else if(srcAddMode == MATRIX_ADDRESSING){
            printf("5.1\n");
            if(!resolveLabel(label, &addr, &isExt)) return FALSE;
            increaseIC(TWO_IMG_WORDS);

        }else if(srcAddMode == REGISTER_ADDRESSING){
            increaseIC(ONE_IMG_WORD);
        }

        /*Destination Operand*/
        if(destAddMode == INSTANT_ADDRESSING){
            increaseIC(ONE_IMG_WORD);
        }else if(destAddMode == DIRECT_ADDRESSING){
            if(!(resolveLabel(src, &addr,&isExt))) return FALSE;
            increaseIC(ONE_IMG_WORD);
            
        }else if(destAddMode == MATRIX_ADDRESSING){
            printf("5.1\n");
            if(!resolveLabel(label, &addr, &isExt)) return FALSE;
            increaseIC(TWO_IMG_WORDS);

        }else if(destAddMode == REGISTER_ADDRESSING){
            increaseIC(ONE_IMG_WORD);
        }  
    }
    return TRUE;
}

int parseRegister(char *arg){
    return atoi((arg+1));
}

long parseInstant(char *arg){
    return atol(arg+1); /*skip the # char*/
}

Bool resolveLabel(char *arg, long *addr, Bool *isExt){
    Label *temp;

    printf("inside resolveLabel, arg: %s\n", arg);
    printf("1\n");
        if(isLabelName(arg)){
            printf("2\n");
            if((temp = getLabelByName(arg)) == NULL){
                yieldError("labelNotFound", arg);
                return FALSE;
            }else{
                printf("3, temp address: %ld\n", (long)temp->address);
                *addr = temp->address;
                printf("addr: %ld\n", *addr);
                *isExt = temp->type == EXTERN_SYMBOL ? TRUE : FALSE;
                return TRUE;
            }
        }else return FALSE;
}

Bool parseMatrix(char *arg, char *label, int *rowReg, int *colReg){
    char *firstOpen, *firstClose, *secondOpen, *secondClose;
    int labelLen;
    char regStr[5];

    printf("inside parseMatrix\n");

    if(!arg) return FALSE;

    firstOpen = strchr(arg, '[');
    if (!firstOpen) {
        yieldError("invalidArgsForCommand",arg);
        return FALSE;
    }

    firstClose = strchr(firstOpen, ']');
    if (!firstClose){
        yieldError("invalidArgsForCommand",arg);
        return FALSE;
    }

    secondOpen = strchr(firstClose, '[');
    if (!secondOpen) {
        yieldError("invalidArgsForCommand",arg);
        return FALSE;
    }

    secondClose = strchr(secondOpen, ']');
    if (!secondClose) {
        yieldError("invalidArgsForCommand",arg);
        return FALSE;
    }

    labelLen = firstOpen - arg;
    if(labelLen <= 0 || labelLen > MAX_LABEL_NAME) {
        yieldError("invalidArgsForCommand",arg);
        return FALSE;
    }

    /*extract label name*/
    strncpy(label, arg, labelLen);
    label[labelLen] = '\0';

    /*extract first reg*/
    strncpy(regStr,firstOpen +1, firstClose - firstOpen - 1);
    regStr[firstClose - firstOpen - 1] = '\0';
    
    if(regStr[0] != 'r' || regStr[1] < '0' || regStr[1] > '7' || regStr[2] != '\0'){
        yieldError("invalidMatrixRegister", regStr);
        return FALSE;
    }

    *rowReg = regStr[1] - '0';

    /*extract second reg*/
    strncpy(regStr,secondOpen +1, secondClose - secondOpen - 1);
    regStr[secondClose - secondOpen - 1] = '\0';

    printf("regStr: %s\n", regStr);
    
    if(regStr[0] != 'r' || regStr[1] < '0' || regStr[1] > '7' || regStr[2] != '\0'){
        yieldError("invalidMatrixRegister", regStr);
        return FALSE;
    }

    *colReg = regStr[1] - '0';

    printf("rowReg: %d, colReg: %d\n", *rowReg, *colReg);

    return TRUE;
}

void printOperation(Operation *opp){
    printf("Opp Name: %s\nOpCode: %d\nNumOfArgs: %d\n",opp->name,opp->opCode,opp->numOfArgs);
}
