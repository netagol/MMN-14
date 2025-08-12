#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "headers.h"

#define OP_TABLE_SIZE 20
#define MAX_OP_NAME 5

typedef struct
{
    char name[MAX_OP_NAME];
    int opCode;
    int numOfArgs;
    int leagalSrcModes[4];
    int leagalDestModes[4];
}Operation;

extern Operation opTable[OP_TABLE_SIZE];

typedef enum AddMode{
    INSTANT_ADDRESSING = 0,
    DIRECT_ADDRESSING = 1,
    MATRIX_ADDRESSING = 2,
    REGISTER_ADDRESSING = 3,
    INVALID_ADDRESSING = 4
} AddMode;

Operation *getOperationByName(char *opName);
Bool oppRouter(Operation *opp, char *line);
Bool handleZeroArgsOpp(Operation *opp, char *line);
Bool handleOneArgOpp(Operation *opp, char *line);
Bool handleTwoArgsOpp(Operation *opp, char *line);

Bool parseOneOperand(char *line, char **dest);

int getAddressingMode(char *arg);
Bool isMatrixFormat(char *arg);
Bool isValidAddrMode(int opCode, int srcAddrMode, int destAddrMode);
Bool encodeInstruction(int opCode, int srcAddMode, int destAddMode,char *src, char *dest);

Bool handleRtsOpp(void);
void handleStopOpp(void);

#endif
