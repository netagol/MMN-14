#include "../headers/labels.h"

Label *labelsTableHead = NULL;
int labelCount = 0;


Label *getLabelsTableHead(void){
    return labelsTableHead;
}

Bool addLabelToLabelsTable(char *line, Label *label){

    char *lineCopy, *labelName;
    char tempLabelName[MAX_LABEL_NAME];
    int type;
    Label *temp;

    printf("INSIDE addLabelToLabelsTable\n");
    printf("line: %s\n",line);
    labelName = NULL;
    temp = NULL;

    printf("1\n");

    if ((lineCopy = malloc((strlen(line)+1) * sizeof(char))) == NULL)
    {
        yieldError("memoryAllocationFailed");
        return FALSE;
    }
    printf("2\n");
      
    strcpy(lineCopy,line);
    
    printf("lineCopy = %s\n",lineCopy);

    getLabelName(lineCopy,&labelName);
    strncpy(tempLabelName,labelName,MAX_LABEL_NAME);
    tempLabelName[strlen(labelName)] = '\0';
    printf("lineCopy = %s\nLabelName = %s\n",lineCopy,labelName);

    type = getLabelType();

    printf("label->name: %s !!!, type: %d\n", label->name, type);
    if(label == (Label *)NULL){
        printf("3\n");
        if (type == DATA_SYMBOL || type == CODE_SYMBOL)
        {
            temp = malloc(sizeof(Label));
            printf("4\n");
            if (temp != NULL)
            {
                strncpy(temp->name, labelName, MAX_LABEL_NAME);
                printf("5\n");
                temp->address = (type == DATA_SYMBOL ? getDC() : getIC());
                temp->next = labelsTableHead;
                temp->type = type;
                labelsTableHead = temp;
                printf("6\n");

                if (labelsTableHead){
                    printf("7\n");
                    free(lineCopy);
                    printf("7.1\n");
                    printf("return true add label\n");
                    return TRUE;
                }else{
                    yieldError("labelNotAdded", labelName);
                    free(lineCopy);
                    return FALSE;
                }
            }else{
                yieldError("labelAllocError", labelName);
                free(lineCopy);
                return FALSE;
            }
        }else{
            free(lineCopy);
            return FALSE;
        }
    }else{
        if (type == DATA_SYMBOL || type == CODE_SYMBOL)
        {
            label->address = (type == DATA_SYMBOL ? getDC() : getIC());
            label->type = type;
            free(lineCopy);
            return TRUE;
        }else{
            free(lineCopy);
            return FALSE;
        }
    }
}

Bool isLabelNameValid(char *word){
    Label *tempLabel;

    printf("inside isLabelNameValid\n");
    
    if(isMacroName(word)){
        yieldError("labelNameMacroName", word);
        return FALSE;

    }else if(isRegisterName(word)){
        yieldError("labelNameRegisterName", word);
        return FALSE;

    }else if(isOppName(word)){
        yieldError("labelNameOppName", word);
        return FALSE;

    }else if(isLabelName(word)){
        if((tempLabel = getLabelByName(word)) != NULL){
            if(tempLabel->address != UNRESOLVED_ADDRESS){
                yieldError("labelNameRedefined", word);
                return FALSE;
            }
        }
    }else if(strlen(word) > MAX_LABEL_NAME){
        yieldError("labelNameTooLong", word);
        return FALSE;
    }
    return TRUE;
}

void getLabelName(char *line, char **labelName){

    if(!labelName || !line){
        yieldError("invalidInputOfLabel");
    }else{
        *labelName = strtok(line,":");
    }
}

int getLabelType(void){
    char *word;

    printf("inside getLabelType\n");
    word = strtok(NULL, " \n\t");
    printf("word: %s\n", word);

    if (!strcmp(word, ".data") || !strcmp(word, ".string") || !strcmp(word, ".mat")) {
        printf("DATA_SYMBOL!!!!\n");
        return DATA_SYMBOL;
    }else if (!strcmp(word, ".extern")) {
        return EXTERN_SYMBOL;
    } else if (!strcmp(word, ".entry")) {
        return ENTRY_SYMBOL;
    } else {
        printf("CODE_SYMBOL!!!!\n");
        return CODE_SYMBOL;
    }
}

void printLabelTable(Label *temp){
    if(temp == NULL){
        printf("***Labels Tabel***\n");
        return;
    }
    printLabelTable(temp->next);

    printf("Address %4d | Name: %s\n",temp->address,temp->name);
}

Label *getLabelByName(char *name){

    Label *temp;
    printf("inside getLabelByName, name:%s, strlen(name): %lu\n", name, strlen(name));
    temp = getLabelsTableHead();
    
    while (temp != NULL)
    {
        printf("while, temp->name: %s, strlen(temp->name): %lu\n", temp->name, strlen(temp->name));
        if(!strcmp(temp->name,name)) return temp;
        temp = temp->next;
    }
    printf("return NULL\n");
    return (Label *)NULL;
}

Bool addLabelPlaceholder(char *arg){
    Label *temp;

    printf("inside addLabelPlaceholder\n");

    if((temp = malloc(sizeof(Label))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(temp->name,arg);
    temp->address = UNRESOLVED_ADDRESS;
    temp->next = labelsTableHead;
    labelsTableHead = temp;

    return TRUE;
}

Bool isUnresolvedLabel(char *line, char **labelVar){
    Label *label = (Label *)NULL;
    char *lineCopy, *temp, *labelName;
    printf("inside isUnresolvedLabel\n");
    if((lineCopy = malloc(strlen(line) * sizeof(char))) == NULL){
        yieldError("memoryAllocationFailed");
        return FALSE;
    }

    strcpy(lineCopy,line);

    if(strchr(line,':') != NULL){
        temp = strtok(lineCopy," \t\n");
        printf("temp = %s\n", temp);
        if(*(temp+(strlen(temp)-1)) == ':'){
            if((labelName = malloc((strlen(temp)-1)*sizeof(char))) == NULL){
                yieldError("memoryAllocationFailed");
                return FALSE;
            }
            strncpy(labelName,temp,(strlen(temp)-1));
            labelName[strlen(labelName)] = '\0';
            printf("labelName = %s\n", labelName);
            trimWhiteSpaces(&labelName);
            if((label = getLabelByName(labelName)) == NULL){
                free(lineCopy);
                printf("not unresolved label\n");
                return FALSE;
            }else{
                if(label->address >=0){
                    yieldError("labelNameRedefined", label->name);
                    return FALSE;
                }
                printf("Label->Name = %s\n", label->name);
                printf("1\n");
                if((*labelVar = malloc(strlen(labelName)*sizeof(char))) == NULL){
                    yieldError("memoryAllocationFailed");
                    return FALSE;
                }

                strcpy(*labelVar, labelName);
                printf("2\n");
                free(lineCopy);
                printf("3\n");
                return TRUE;
            }
        }
    }


    free(lineCopy);
    return FALSE;
}


