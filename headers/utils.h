#ifndef UTILS_H
#define UTILS_H

#include "headers.h"

#define FOUR_BASE_MASK_START 768
#define FOUR_BASE_ITERATIONS 4
#define FOUR_BASE_WORD_LEN 6

void setFirstPass(void);
void setSecondPass(void);
int getCurrentPass(void);

void createNewFileName(char *originName, char *destName, char *extension);
char *getCurrentFileName(void);
void updateCurrentFileName(char *name);


Bool isMacroName(char *word);
Bool isRegisterName(char *word);
Bool isOppName(char *word);
Bool isLabelName(char *word);
Bool isLabelDefinition(char *line, char **word, int pass);
void increaseRowNum(void);
void rewindRowNum(void);
void rewindIC(void);
void increaseDC(int num);
void increaseIC(int num);
int getRowNum(void);
int getIC(void);
int getDC(void);
void getNextWord(char *line, char **command);
Bool validateCommand(char *command);
commandType getCommandType(char *command);
Bool dataLineHelper(char *line, char *command);
int countArgs(char *line);
Bool validateStrLine(char *line);
Bool validateDataLine(char *line);
Bool validateMatLine(char *line,int *argsCount, int *matSize, int matNums[MAX_ALLOWED_LINE_LEN]);
void printBinary(short int word);
void trimWhiteSpaces(char **line);
void trimWhiteSpacesAtStart(char **line);
Bool isCodeCommand(char *command);
void binToFourBase(unsigned short num, char *word);


#endif
