#include "../headers/firstRun.h"


Bool firstRun(FILE *amFile){

    char line[MAX_LINE_LEN];


    printf("*** STATE: FIRST_RUN ***\nFileName: %s\nrowNum: %d, IC: %d, DC: %D\n", getCurrentFileName(),getRowNum(), getIC(),getDC());
    while ((fgets(line, MAX_LINE_LEN, amFile)) != NULL)
    {
        if (*line != '\n' && line[0] != ';')
        {
            printf("[%d] Line: %s \n",getRowNum(),line);
            readLine(line);
        }
        increaseRowNum();
    }
    printf("***Labels Tabel***\n");
    printLabelTable(getLabelsTableHead());
    return TRUE;
}

Bool readLine(char *line){
    char lineCopy[MAX_LINE_LEN];
    char *label, *command = NULL;
    Operation *currOp;
    commandType commType;

    strcpy(lineCopy,line);

    printf("*** inside read line ***\n");
    printf("line: %s\n", lineCopy);

    if(isUnresolvedLabel(lineCopy, &label)){
        printf("readline, is unresolved\n");
        if(addLabelToLabelsTable(line,getLabelByName(label)))
        getNextWord(lineCopy + strlen(label) + 1, &command);
    }else if(isLabelDefinition(lineCopy, &label))
    {
        printf("Label: %s\nline: %s\n", label,lineCopy);
        addLabelToLabelsTable(lineCopy, NULL);

        printf("line: %s, label: %s\n", lineCopy, label);
        getNextWord(lineCopy + strlen(label)+1, &command);
    }else{
        getNextWord(lineCopy, &command);
    }


    printf("command123: %s\n",command);

    if(validateCommand(command)){
        if((commType = getCommandType(command))){
            switch (commType)
            {
            case DATA_COMMAND: return dataLineHelper(lineCopy, command);

            case CODE_COMMAND:
                printf("CODE_COMMAND\n");
                currOp = getOperationByName(command);
                return oppRouter(currOp, lineCopy + (label != NULL ? strlen(label) +1 : 0));
            
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
    return TRUE;
}


