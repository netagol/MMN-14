#ifndef ERRORS_H
#define ERRORS_H

#include "headers.h"

#include <stdarg.h> /*for yieldError*/

#define MAX_ERR_NAME 50
#define MAX_ERR_MSG 200
#define MAX_ERR_TABLE_SIZE 100


typedef struct {
    char name[MAX_ERR_NAME];
    char message[MAX_ERR_MSG];
} Err;

extern Err errorTable[];

void yieldError(char *errName, ...);

#endif
