#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_LINE_LEN 100
#define MAX_ALLOWED_LINE_LEN 80
#define MAX_FILE_NAME 253
#define MAX_POSTFIX 3
#define LAST_REG_NUM 7
#define FIRST_REG_NUM 1
#define IC_START 100
#define FIRST_PASS 1
#define SECOND_PASS 2
#define MAX_SHORT_NUM 32768

extern int DC, IC, rowNum;
extern char currentFileName[MAX_FILE_NAME];
extern int currentPass;

typedef enum Booleans{
    TRUE = 1,
    FALSE = 0
} Bool;

typedef enum commandType{
    ENTRY_COMMAND = 1,
    DATA_COMMAND, 
    EXTERN_COMMAND, 
    CODE_COMMAND
} commandType;

#endif
