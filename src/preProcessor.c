#include "../headers/preProcessor.h"

Macro macroTable[MAX_MACROS];
int macroCount;

Bool parseMacros(FILE *asFile, FILE *amFile){
    char line[MAX_LINE_LEN];
    char macroName[MAX_MACRO_NAME + 1];
    int  macroFlag, i, lineCount;
    char lineCopy[MAX_LINE_LEN + 1];
    Macro *macro;


    printf("*** STATE: PREPROCESSOR ***\n");
    macroFlag = i = 0;
    lineCount = 1;
    macro = NULL;

    while(fgets(line,MAX_LINE_LEN,asFile) != NULL){
        printf("[%d] Line: %s \n",getRowNum(),line);
        
        line[strcspn(line, "\n")] = '\0';

        if(isEmptyLine(line)) {
            fprintf(amFile,"\n");
            increaseRowNum();
            continue;
        }


        strcpy(lineCopy,line);

        if(isLineMacro(lineCopy, &macro)){
            printMacro(macro,amFile);
        }else{
            if((isLineMacroStart(lineCopy)) && !macroFlag){
                if(addMacro(macroName)){
                    macroFlag = TRUE;
                    macro = getMacroByName(macroName);
                }else{
                    return FALSE;
                }
            
            }else if(isLineMacroEnd(line, macro->name) && macroFlag){
                macroFlag = FALSE;
                macro = NULL;

            }else if(macroFlag){
                if(!addLineToMacro(line,macro,lineCount)){
                    return FALSE;
                }
            }else{
                fprintf(amFile,"%s\n",line);
            }
        }
        lineCount++;
        increaseRowNum();
    }
    return TRUE;
}

Bool isLineMacroStart(char *line){
    char *word;
    word = strtok(line," ");
    if (!strcmp(word,"mcro")) return TRUE;  
    else return FALSE;
}

Bool getMacroName(char *macroName){
    char *word;
    if((word = strtok(NULL," \n\t")) == NULL){
        yieldError("missingMcroName");
        return FALSE;
    }else{
        if(isMacroNameValid(word)){
            strcpy(macroName,word);
        }else
        {
            return FALSE;
        }
        
        if ((word = strtok(NULL," \n\t")) != NULL)
        {
            yieldError("extraChrAfterMcroName");
            macroName = NULL;
            return FALSE;
        }
    }
    return TRUE;
}

Bool isMacroNameValid(char *macroName){
    
    if(isOppName(macroName)){
        return FALSE;
    }
    
    return TRUE;
}


Bool addMacro(char *macroName){
    int i;

    if (macroCount >= MAX_MACROS)
    {
        yieldError("tooManyMcros",MAX_MACROS);
        return FALSE;
    }else{
        if(getMacroName(macroName)){
            if(macroName != NULL){
                for(i = 0; i < macroCount; i++){
                    if(!strcmp(macroTable[i].name, macroName)){
                        yieldError("macroRedefined",macroName);
                        return FALSE;
                    }
                }
            }
        }else{
            return FALSE;
        }
    }



    strncpy(macroTable[macroCount].name, macroName, MAX_MACRO_NAME);
    macroTable[macroCount].name[MAX_MACRO_NAME] = '\0';
    macroTable[macroCount].lineCount = 0;
    macroCount++;
    return TRUE;

}

Bool addLineToMacro(char *line, Macro *currentMacro, int lineCount){
    if (currentMacro->lineCount >= MAX_MACRO_LINES){
        yieldError("tooManyMcroLines",MAX_MACRO_LINES);
        return FALSE;
    }
    if (strlen(line) > MAX_ALLOWED_LINE_LEN){
        yieldError("lineTooLong",lineCount,MAX_ALLOWED_LINE_LEN);
        return FALSE;
    }
    strcpy(currentMacro->lines[currentMacro->lineCount],line);
    currentMacro->lineCount++;
    return TRUE;
}


Macro *getMacroByName(char *macroName){
    int i;
    for(i = 0; i < macroCount; i++){
        if(!strcmp(macroTable[i].name,macroName)) return &macroTable[i];
    }
    return NULL;
}



Bool isLineMacroEnd(char *line, char *macroName){
    char lineCopy[MAX_LINE_LEN + 1];
    strcpy(lineCopy,line);
    trimLine(lineCopy);
    if (!strcmp(lineCopy,"mcroend")){
        strcpy(lineCopy,line);
        if(lineCopy[strcspn(lineCopy,"mcroend") + strlen("mcroend")] != '\0'){
            yieldError("extraCharsAfterMcroEnd", macroName);
            return FALSE;
        }else{
            return TRUE;
        }

    }
    

    else return FALSE;   
}

void printMacro(Macro *macro, FILE *file){
    int i;

    for(i = 0; i < macro->lineCount; i++){
        fprintf(file,"%s\n",macro->lines[i]);
    }
}

Bool isEmptyLine(char *line){
    if (line[0] == '\0' || strspn(line, " \t") == strlen(line)) return TRUE;
    else return FALSE;
}

Bool isLineMacro(char *line, Macro **macro){
    int i;
    char lineCopy[MAX_LINE_LEN];
    strcpy(lineCopy,line);
    trimLine(lineCopy);

    for(i = 0; i < macroCount; i++){
        if(!strcmp(macroTable[i].name,lineCopy)){
            *macro = &macroTable[i];
            return TRUE;
        } 
    }
    return FALSE;
} /*isLineMacro*/


void trimLine(char *line){
    int i, trimmedIndex;
    char trimmed[MAX_LINE_LEN];
    trimmedIndex = 0;
    
    for(i = 0; i < strlen(line); i++){
        if(!isspace(line[i])){
            trimmed[trimmedIndex++] = line[i];
        }
    }
    trimmed[trimmedIndex] = '\0';
    strcpy(line,trimmed);
} /*trimLine*/

