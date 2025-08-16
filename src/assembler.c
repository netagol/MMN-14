#include "../headers/headers.h"


int main(int argc, char *argv[]){
    char asFileName[MAX_FILE_NAME + MAX_POSTFIX];
    int i;
    if(argc == 1){
        yieldError("missingFileNames");
        return 0;
    }

    for (i = 1; i < argc; i++)
    {
        if(strlen(argv[i]) > (MAX_FILE_NAME + MAX_POSTFIX)){
            yieldError("fileNameTooLong", argv[i]);
            yieldError("fileNotProcessed",argv[i]);
        }else{
            sprintf(asFileName,"filesToProcess/%s.as",argv[i]);

            if(!processFile(asFileName))
            {
                yieldError("fileNotProcessed",asFileName);
                prepareForNextFile();
            }else{
                prepareForNextFile();
            }
        }
    }
    printf("All files processed, quitting the program\n");

   return 0;
    
} /*main*/


Bool processFile(char *asFileName){

    FILE *asFile, *amFile, *objFile;
    char amFileName[MAX_FILE_NAME], objFileName[MAX_FILE_NAME];

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
                if(!firstRun(amFile)){
                    return FALSE;
                }
                printf("FIRST RUN SUMMARY, Errors: %d, fileName: %s\n", errFlag, getCurrentFileName());
                printLabelTable(getLabelsTableHead());
                printDataImg();
                printInstructionImg();

                /*prep for second pass*/
                rewindRowNum();
                rewindIC();
                rewind(amFile);
                
                if(getErrFlag()) {
                    return FALSE;
                }
                if(!secondRun(amFile)) {
                    return FALSE;
                }
                printf("SECOND RUN SUMMARY\n");
                printLabelTable(getLabelsTableHead());
                printDataImg();
                printInstructionImg(); 
                fclose(amFile); 

                createNewFileName(amFileName,objFileName,".ob");
                updateCurrentFileName(objFileName);
                printf("fileName: %s\n", currentFileName);
                

                if((objFile = fopen(objFileName,"w")) != NULL){
                    if(!buildObjFile(objFile)){
                        return FALSE;
                    }
                    else{
                        printf("File %s Processed successfully\n", asFileName);
                    }
                    fclose(objFile);
                }else{
                    yieldError("FileOpenError", objFile);
                    return FALSE;
                }
            }else{
                fclose(asFile);
                fclose(amFile);
                remove(amFileName);
                return FALSE;
            }
        }else{
            yieldError("FileOpenError", amFileName);
            return FALSE;
        }
    }else{
        yieldError("FileOpenError", asFileName);
        return FALSE;
    }

    return TRUE;
}

