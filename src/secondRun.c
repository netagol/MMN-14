#include "../headers/secondRun.h"

Bool secondRun(FILE *amFile){
    char line[MAX_LINE_LEN];

    setSecondPass();
    printf("*** STATE: SECOND_RUN ***\nFileName: %s\nrowNum: %d, IC: %d, DC: %D\n", getCurrentFileName(),getRowNum(), getIC(),getDC());


    while ((fgets(line, MAX_LINE_LEN, amFile)) != NULL)
    {
        if (*line != '\n' && line[0] != ';')
        {
            printf("[%d] Line: %s \n",getRowNum(),line);
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
        getNextWord(lineCopy, &command);

        if(isCodeCommand(command)){
            currOp = getOperationByName(command);
            if(currOp->numOfArgs >= 0){
                return oppRouter(currOp, lineCopy + (label != NULL ? strlen(label) +1 : 0), SECOND_PASS);
            }
        }
    }
/* add here if not label def*/    
}



