#include "../headers/firstRun.h"


Bool firstRun(FILE *amFile){

    char line[MAX_LINE_LEN];


    printf("*** STATE: FIRST_RUN ***\nFileName: %s\nrowNum: %d, IC: %d, DC: %D\n", getCurrentFileName(),getRowNum(), getIC(),getDC());
    setFirstPass();

    while ((fgets(line, MAX_LINE_LEN, amFile)) != NULL)
    {
        if (*line != '\n' && line[0] != ';')
        {
            printf("[%d] Line: %s \n",getRowNum(),line);
            readLineFirstPass(line);
        }
        increaseRowNum();
    }
    updateDataImgAddrs();
    updateLabelsTableAddrs();

    return TRUE;
}

Bool readLineFirstPass(char *line){
    char lineCopy[MAX_LINE_LEN];
    char *label, *command = NULL;
    Operation *currOp;
    commandType commType;

    strcpy(lineCopy,line);

    printf("*** inside read line ***\n");
    printf("line: %s\n", lineCopy);

    if(isUnresolvedLabel(lineCopy, &label)){
        if(addLabelToLabelsTable(line,getLabelByName(label)))
        getNextWord(lineCopy + strlen(label) + 1, &command);
    }else if(isLabelDefinition(lineCopy, &label, FIRST_PASS))
    {
        addLabelToLabelsTable(lineCopy, NULL);

        getNextWord(lineCopy + strlen(label)+1, &command);
    }else{
        getNextWord(lineCopy, &command);
    }



    if(validateCommand(command)){
        if((commType = getCommandType(command))){
            switch (commType)
            {
            case DATA_COMMAND: return dataLineHelper(lineCopy, command);

            case CODE_COMMAND:
                currOp = getOperationByName(command);
                return oppRouter(currOp, lineCopy + (label != NULL ? strlen(label) +1 : 0), FIRST_PASS);
            
            default:
                return FALSE;
                break;
            }
        }else{
            return FALSE;
        }
    }
    
    label = NULL;
    free(command);
    free(label);
    return TRUE;
}


