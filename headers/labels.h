#ifndef LABELS_H
#define LABELS_H

#include "headers.h"

#define MAX_LABEL_NAME 30
#define UNRESOLVED_ADDRESS -1
extern int labelCount;

typedef enum symbolType{
    ENTRY_SYMBOL = 0,
    DATA_SYMBOL, 
    EXTERN_SYMBOL, 
    CODE_SYMBOL
} SymbolType;

typedef struct Label_s
{
    char name[MAX_LABEL_NAME + 1];
    int address;
    SymbolType type;
    struct Label_s *next; 
} Label;

extern int labelCount;
extern Label *labelsTableHead;

typedef struct labelsTable
{
    Label **Labels;
}labelsTable;


Bool addLabelToLabelsTable(char *line);
Bool isLabelNameValid(char *word);
void getLabelName(char *line, char **labelName);
int getLabelType(void);
Label *getLabelsTableHead(void);
void printLabelTable(Label *temp);
Label *getLabelByName(char *name);
Bool addLabelPlaceholder(char *arg);


#endif
