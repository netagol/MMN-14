#include "../headers/headers.h"

#define MAX_ERR_NAME 50
#define MAX_ERR_MSG 200
#define MAX_ERR_TABLE_SIZE 100
#define MAX_FILE_NAME 100

Bool firstRun(FILE *amFile);
Bool readLine(char *line);


int main(void){

    FILE  *amFile;
    char *amFileName = "../tests/demoInput.am";
    firstRun(amFile);


    return 0;
} /*main*/

Bool firstRun(FILE *amFile){
    char line[100];

    while ((fgets(line, MAX_LINE_LEN, amFile)) != NULL)
    {
        readLine(line);
    }
}

Bool readLine(char *line){

    char *word;

    if (isLabelDefinition(line, &word))
    {
        printf("heyyy\n");
    }else{
        printf("Ohhhh\n");
    }
    
    
    return TRUE;

}







