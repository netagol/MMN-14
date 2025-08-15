#include "../headers/headers.h"


int main(void){

    char asFileName[MAX_FILE_NAME];
    strcpy(asFileName,"tests/mov.as");
    if (!processFile(asFileName))
    {
        yieldError("fileNotProcessed",asFileName);
    }
    return 0;
} /*main*/


Bool processFile(char *asFileName){

    FILE *asFile, *amFile;
    char amFileName[MAX_FILE_NAME];

    macroCount = 0;

    printf("File name: %s\n",asFileName);

    if((asFile = fopen(asFileName,"r"))!= NULL){
        createNewFileName(asFileName,amFileName,".am");
        if ((amFile = fopen(amFileName,"w"))!= NULL)
        {
            updateCurrentFileName(asFileName);
            if(parseMacros(asFile, amFile)){
                fclose(asFile);
                fclose(amFile);

                amFile = fopen(amFileName,"r");

                updateCurrentFileName(amFileName);
                rewindRowNum();
                if(!firstRun(amFile)) return FALSE;
                printf("FIRST RUN SUMMARY, Errors: %d\n", errFlag);
                printLabelTable(getLabelsTableHead());
                printDataImg();
                printInstructionImg();

                /*prep for second pass*/
                rewindRowNum();
                rewindIC();
                rewind(amFile);

                if(getErrFlag()) return FALSE;
                if(!secondRun(amFile)) return FALSE;
                printf("SECOND RUN SUMMARY\n");
                printLabelTable(getLabelsTableHead());
                printDataImg();
                printInstructionImg();

            }else{
                fclose(amFile);
                remove(amFileName);
                return FALSE;
            }
        }else{
            yieldError("amFileOpenError", amFileName);
            return FALSE;
        }
    }else{
        yieldError("asFileOpenError", asFileName);
        return FALSE;
    }

    return TRUE;

}

