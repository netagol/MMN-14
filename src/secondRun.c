#include "../headers/secondRun.h"

Bool secondRun(FILE *amFile){
    char line[MAX_LINE_LEN];

    setSecondPass();
    printf("*** STATE: SECOND_RUN ***\nFileName: %s\nrowNum: %d, IC: %d, DC: %D\n", getCurrentFileName(),getRowNum(), getIC(),getDC());


    while ((fgets(line, MAX_LINE_LEN, amFile)) != NULL)
    {
        if (*line != '\n' && line[0] != ';')
        {
            printf("[pass: %d, rowNum: %d] Line: %s \n", getCurrentPass(), getRowNum(), line);
            if(!readLineSecondPass(line)) return FALSE;
        }
        increaseRowNum();
    }
    return TRUE;
}

Bool readLineSecondPass(char *line){
    char *lineCopy, *label, *command = NULL;
    Operation *currOp;

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    if(isLabelDefinition(lineCopy,&label,SECOND_PASS)){
        getNextWord(lineCopy + strlen(label)+1, &command);
    }else{
       getNextWord(lineCopy, &command); 
    }

    if(isCodeCommand(command)){
        printf("code command\n");
            currOp = getOperationByName(command);
            if(currOp->numOfArgs >= 0){
                return oppRouter(currOp, lineCopy + (label != NULL ? strlen(label) +1 : 0), SECOND_PASS);
            }
        }
   return TRUE;
}

Bool buildObjFile(FILE *file){
    unsigned short i;
    char add[FOUR_BASE_WORD_LEN], word[FOUR_BASE_WORD_LEN];
    fprintf(file,"%-4d  %6d\n\n",IC - IC_START,DC);

    for(i = 0; i < IC - IC_START; i++){
        binToFourBase(i + IC_START,add);
        binToFourBase(instructionsImage[i], word);
        fprintf(file,"%-6s %6s\n",add, word);

    }
    for(i = 0 ;i < DC ; i++){
        binToFourBase(dataImage[i].address,add);
        binToFourBase(dataImage[i].word, word);
        fprintf(file,"%-6s %6s\n",add, word);
    }
    return TRUE;
}


