#include "../headers/secondRun.h"

Bool secondRun(FILE *amFile){
    char line[MAX_LINE_LEN];

    while ((fgets(line, MAX_LINE_LEN, amFile)) != NULL)
    {
        if (*line != '\n' && line[0] != ';')
        {
            printf("[%d] Line: %s \n",getRowNum(),line);
            readLineSecondPass(line);
        }
        increaseRowNum();
    }
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
            if(currOp->numOfArgs > 0){
                return oppRouter(currOp, lineCopy + (label != NULL ? strlen(label) +1 : 0), SECOND_PASS);
            }
        }
    }
}



