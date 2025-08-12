#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include "headers.h"


#define MAX_MACRO_NAME 50
#define MAX_MACRO_LINES 100
#define MAX_MACROS 50

typedef struct Macro
{
    char name[MAX_MACRO_NAME + 1];
    char lines [MAX_MACRO_LINES][MAX_LINE_LEN + 1];
    int lineCount;
} Macro;


extern Macro macroTable[MAX_MACROS];
extern int macroCount;

Bool isMacroNameValid(char *macroName);
Bool addMacro(char *macroName);
Bool addLineToMacro(char *line, Macro *currentMacro, int lineCount);
Macro *getMacroByName(char *macroName);
Bool isLineMacroStart(char *line);
Bool isLineMacroEnd(char *line, char *macroName);
Bool getMacroName(char *macroName);
Bool parseMacros(FILE *asFile, FILE *amFile);
void printMacro(Macro *macro, FILE *file);
Bool isEmptyLine(char *line);
Bool isLineMacro(char *line, Macro **macro);
void trimLine(char *line);

#endif
