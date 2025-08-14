#include "../headers/utils.h"

int rowNum = 1;
int DC = 0;
int IC = IC_START;
char currentFileName[MAX_FILE_NAME] = "";


void increaseRowNum(void){
    rowNum ++;
}

void rewindRowNum(void){
    rowNum = 1;
}

void increaseDC(int num){
    DC += num;
}
void increaseIC(int num){
    IC += num;
}

int getRowNum(void){
    return rowNum;
}

int getIC(void){
    return IC;
}

int getDC(void){
    printf("getDC\n");
    return DC;
}

void createNewFileName(char *originName, char *destName, char *extension){
    char fileNameCopy[MAX_FILE_NAME];
    char *fileName;

    strcpy(fileNameCopy,originName);
    fileName = strtok(fileNameCopy,".");
    if(fileName != NULL){
        strcpy(destName,fileName);
        strcat(destName,extension);  
    }else{
        yieldError("missingFileName", extension, extension);
    }
}

char *getCurrentFileName(void){
    return currentFileName;
}

void updateCurrentFileName(char *name){
    strcpy(currentFileName,name);
}


Bool isMacroName(char *word){
    int i;

    for(i = 0; i < MAX_MACROS; i++){
        if(!strcmp(macroTable[i].name,word)){
            return TRUE;
        }
    }

    return FALSE;
}

Bool isRegisterName(char *word){
    
    if (strlen(word) == 2)
    {
        if (word[0] == 'r' && word[1] >= FIRST_REG_NUM && word[1] <= LAST_REG_NUM) return TRUE;
        else return FALSE;
    }else return FALSE;

}

Bool isOppName(char *word){
    int i; 
    for(i = 0; i < OP_TABLE_SIZE; i++){
        if (!strcmp(word,opTable[i].name))
        {
            return TRUE;
        }
    }

    return FALSE;
}

Bool isLabelName(char *word){
    Label *temp;
    printf("inside isLabelName, word: %s\n", word);
    printf("1\n");
    temp = getLabelsTableHead();
    printf("2\n");
    while (temp != NULL){
        if (!strcmp(word,temp->name)) return TRUE;

        temp = temp->next;
    }
    printf("3\n");

    return FALSE;
}

Bool isLabelDefinition(char *line, char **word){

    char lineCopy[MAX_LINE_LEN], labelName[MAX_LABEL_NAME], *temp;

    printf("INSIDE isLabelDefinition\nLine: %s\n",line);

    printf("1\n");
    if(strchr(line,':') != NULL){
        printf("2\n");
        strcpy(lineCopy,line);
        printf("3\n");
        temp = strtok(lineCopy," \t\n");
        printf("temp: %s\n", temp);
        
        if(*(temp+(strlen(temp)-1)) == ':'){
            strncpy(labelName,temp,(strlen(temp)-1));
            labelName[strlen(temp)-1] = '\0';
            printf("labelName: %s\n", labelName);

            if(isLabelNameValid(labelName)){
                if((*word = malloc((strlen(labelName) * sizeof(char)) + 1)) == NULL){
                    yieldError("labelAllocError");
                    return FALSE;
                }
                strcpy(*word,labelName);
                (*word)[strlen(labelName)] = '\0';
                printf("return true word: %s\n", *word);
                return TRUE;
            }else{
                return FALSE;
            }  
        }else{
            yieldError("illeagalColumnInLine");
            return FALSE;
        }
    }else{
        return FALSE;
    }
}

void trimWhiteSpaces(char **line){
    int i, j;
    char temp[MAX_LINE_LEN];

    j = 0;
    for (i = 0; i < strlen(*line) + 1; i++)
    {
        if(!isspace((*line)[i])){
            temp[j++] = (*line)[i];
        }
    }
    temp[j] = '\0';
    strcpy(*line, temp);
}

void trimWhiteSpacesAtStart(char **line){
    int i;

    i = 0;

    while (isspace((*line)[i]))
    {
        (*line)++;
    }

}

void getNextWord(char *line, char **command){
    char *lineCopy, *opName;
    printf("inside getNextWord\n");

    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return;
    }
    
    strcpy(lineCopy, line);

    opName = strtok(lineCopy, " \t\n");
    if((*command = malloc(strlen(opName)*sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return;
    }
    strcpy(*command,opName);
    
    free(lineCopy);
    printf("outside getNextWord\n");
}

Bool validateCommand(char *command){
    int i;

    if (!strcmp(command,".string") || !strcmp(command,".data") || !strcmp(command,".mat") || !strcmp(command,".extern") || !strcmp(command,".entry"))
    {
        if(!(command[0] == '.')){
            yieldError("missingDotOnCommand", command);
            return FALSE;
        }

        for(i = 1; i < strlen(command); i++){
            if (!(command[i] >= 'a' && command[i] <= 'z'))
            {
                yieldError("illeagalCharInCommand", command[i], command);
                return FALSE;
            }
        }
    }else{
        for(i = 0; i < strlen(command); i++){
            if (!(command[i] >= 'a' && command[i] <= 'z'))
            {
                yieldError("illeagalCharInCommand", command[i], command);
                return FALSE;
            }else{
            }
        }
    }
        return TRUE;
}


commandType getCommandType(char *command){
    printf("inside getCOmmandType, command: %s\n", command);

    if (command)
    {
        if(!strcmp(command, ".data") || !strcmp(command, ".string") || !strcmp(command, ".mat")) return DATA_COMMAND;
        else if(!strcmp(command, ".entry")) return ENTRY_COMMAND;
        else if(!strcmp(command, ".extern")) return EXTERN_COMMAND;
        else if(isOppName(command)) return CODE_COMMAND;
        else{
            yieldError("unokwnCommand",command);
            return 0;
        }
    }else{
        yieldError("errorGetCommType");
        return 0;
    }
}

Bool dataLineHelper(char *line, char *command){
    char *lineArgs, *commandPos;
    int matSize,argsCount;
    static int matNums[MAX_ALLOWED_LINE_LEN];

    if(!(commandPos = strstr(line,command))){
        yieldError("commNotFoundInLine",command,line);
        return FALSE;
    }

    lineArgs = commandPos + strlen(command);

    while (isspace(*lineArgs)) lineArgs++;

    printf("Inside dataLineHelper, lineArgs = %s, len = %lu\n", lineArgs, strlen(command));
    matSize = argsCount = 0;

    if (!strcmp(command,".data"))
    {
        printf(".data\n");
        if(validateDataLine(lineArgs)){
            if(addDataToDataImage(lineArgs)) return TRUE;
        }
    }else if(!strcmp(command,".string")){
        printf(".string\n");
        if(validateStrLine(lineArgs)){
            if(addStrToDataImage(lineArgs)) return TRUE;
        }
    }else if(!strcmp(command,".mat")){
        if(validateMatLine(lineArgs,&argsCount, &matSize, matNums)){
            if(addMatToDataImage(matSize, argsCount, matNums)) return TRUE;
        }
    }

    return FALSE;
}

Bool vaildateCommas(char *line){
    int i;

    for(i = 0; i < strlen(line) -1; i++){
        if(line[i] == ',' && line[i+1] == ','){
            yieldError("multipleConsecutiveCommas");
            return FALSE;
        }
    }

    if (line[strlen(line)-1] == ',')
    {
        yieldError("illeagalCommaAtEnd");
        return FALSE;
    }
    return TRUE;
}

Bool validateMatLine(char *line, int *argsCount, int *matSize, int matNums[MAX_ALLOWED_LINE_LEN]){

    char *lineCopy, *firstOpen, *firstClose, *secondOpen, *secondClose, *num1_s, *num2_s, *num1_end, *num2_end, *num_s, *end;
    int num1_i, num2_i, num_i, i;
    int nums[MAX_ALLOWED_LINE_LEN];

    firstOpen = firstClose = secondOpen = secondClose = num1_s = num2_s = num1_end = num2_end = num_s = end = NULL;

    *argsCount = 0;

    printf("insideValidateMat\n");
    
    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    trimWhiteSpaces(&lineCopy);

    printf("1\n");

    if((firstOpen = strchr(lineCopy,'['))){
        printf("1.1\n");
        if((firstClose = strchr(lineCopy,']'))){
            printf("1.2\n");
            if(firstOpen < firstClose){
                printf("1.3\n");
                if ((num1_s = malloc(firstClose - firstOpen)) == NULL)
                {
                    yieldError("memoryAllocationFailed");
                    free(lineCopy);
                    return FALSE;
                }
                
                strncpy(num1_s,firstOpen + 1, (firstClose - firstOpen - 1));
                printf("1.4\n");
                printf("2\n");
                if((secondOpen = strchr(firstClose + 1,'['))){
                    printf("2.1\n");
                    if ((secondClose = strchr(firstClose + 1, ']'))){
                        printf("2.2\n");
                        if(secondOpen < secondClose){
                            printf("2.3\n");
                            if((num2_s = malloc(secondClose - secondOpen)) == NULL){
                                yieldError("memoryAllocationFailed");
                                free(lineCopy);
                                free(num1_s);
                                return FALSE;
                            }
                            printf("2.4\n");
                            strncpy(num2_s,secondOpen + 1, (secondClose - secondOpen - 1));
                            printf("2.5\n");
                            num1_i = (int)strtol(num1_s, &num1_end, 10);
                            num2_i = (int)strtol(num2_s, &num2_end, 10);
                            
                            if(*num1_end == '\0' && *num2_end == '\0'){
                                *matSize = (num1_i + num2_i);
                                if(*(secondClose + 1) != '\0'){
                                    if(vaildateCommas(secondClose + 1)){
                                        printf("3\n");
                                        printf("argsCount: %d\n", *argsCount);
                                        num_s = strtok(secondClose + 1, ",");
                                        printf("4\n");

                                        while (num_s != NULL){
                                            printf("argsCount: %d\n", *argsCount);
                                            printf("4.1\n");
                                            num_i = strtol(num_s, &end, 10);
                                            printf("4.2\n");
                                            printf("4.3\n");
                                            if((end = malloc(strlen(num_s)*sizeof(char))) == NULL){
                                                yieldError("memoryAllocationFailed");
                                                free(lineCopy);
                                                free(num1_s);
                                                free(num2_s);
                                                return FALSE;
                                            }
                                            printf("4.4\n");

                                            if(*end == '\0'){
                                                num_s = strtok(NULL, ",");
                                                free(end);
                                                nums[(*argsCount)++] = num_i;
                                                printf("4.5\n");
                                            }else{
                                                yieldError("invalidNumArgInMat");
                                                free(lineCopy);
                                                free(num1_s);
                                                free(num2_s);
                                                free(end);
                                                return FALSE;
                                            }
                                        }
                                        printf("5\n");
                                        if (*argsCount > *matSize)
                                        {
                                            yieldError("tooManyNumbersInMatDef", *matSize, *argsCount);
                                            free(lineCopy);
                                            free(num1_s);
                                            return FALSE;
                                        }else{
                                            for(i = 0; i < *argsCount; i++){
                                                matNums[i] = nums[i];
                                            }
                                            free(lineCopy);
                                            free(num1_s);
                                            printf("retrun true mat1\n");
                                            return TRUE;
                                        }
                                    }
                                }else{
                                    free(lineCopy);
                                    free(num1_s);
                                    printf("retrun true mat2\n");
                                    return TRUE;
                                }
                            }else{
                                yieldError("invalidNumArgInMat");
                            }
                        }else{
                            yieldError("matArrBracketMisplaced");
                        }   
                    }else{
                        yieldError("matArrMissingClosingBracket");
                    }
                }else{
                    yieldError("matArrMissingOpeningBracket");
                }
            }else{
                yieldError("matArrBracketMisplaced");
            }
        }else{
            yieldError("matArrMissingClosingBracket");
        }
    }else{
        yieldError("matArrMissingOpeningBracket");
    }


    return FALSE;
}

Bool validateStrLine(char *line){
    char *lineCopy = NULL;
    int i;

    if((lineCopy = malloc(MAX_LINE_LEN * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    printf("Inside validateStrLine, line: %s, lineCopy: %s\n",line,lineCopy);

    trimWhiteSpaces(&lineCopy);
    printf("Inside validateStrLine2, line: %s, lineCopy: %s\n",line,lineCopy);
    

    if (lineCopy[0] == '"')
    {
        if(lineCopy[strlen(lineCopy)-1] == '"'){
            for(i = 1; i < strlen(lineCopy)-2 ; i++){
                if(lineCopy[i] == '"'){
                    yieldError("illeagalQuoteInStr", line);
                    free(lineCopy);
                    return FALSE;
                }
            }
            printf("return true\n");
            free(lineCopy);
            return TRUE;

        }else{
            yieldError("strMissingClosingQuote", line);
            free(lineCopy);
            return FALSE;
        }
    }else{
        yieldError("strMissingOpeningQuote", line);
        free(lineCopy);
        return FALSE;
    }
    


}

Bool validateDataLine(char *line){
    char *lineCopy, *num_s, *end;

    num_s = end = NULL;
    
    if((lineCopy = malloc(MAX_LINE_LEN * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    trimWhiteSpaces(&lineCopy);

    if(!vaildateCommas(lineCopy)) {
        free(lineCopy);
        return FALSE;
    }

    num_s = strtok(lineCopy,",");
    if (*num_s == '\0')
    {
        yieldError("missingArgsForCommand");
    }
    
    while (num_s != NULL)
    {
        strtol(num_s, &end, 10);
        if(*end != '\0'){
            yieldError("invalidNumArg", num_s);
            free(lineCopy);
            return FALSE;
        }else{
            num_s = strtok(NULL, ",");
        }
    }

    free(lineCopy);
    return TRUE;
}

int countArgs(char *line){
    char *lineCopy, *num_s;
    int count;

    count = 0;

    lineCopy = num_s = NULL;


    if ((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL)
    {
        yieldError("memoryAllocationFailed");
        return FALSE;
    }
    
    strcpy(lineCopy,line);

    num_s = strtok(lineCopy,",");
    while (num_s != NULL)
    {
        count++;
        num_s = strtok(NULL,",");
    }

    free(lineCopy);
    return count;
}

void printBinary(short int word){
    int i;

    for(i = 15; i >= 0; i--){
        putchar((word & (1 << i)) ? '1' : '0');
        if (i % 4 == 0 && i != 0) putchar(' ');
    }
}



