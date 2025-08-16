#include "../headers/errors.h"

Bool errFlag = FALSE;

Bool getErrFlag(void){
    return errFlag;
}

void setErrFlag(Bool flag){
    errFlag = flag;
}

Err errorTable[] = {
    {"FileOpenError","%s File could not be opened or created, try another file"},
    {"missingFileName","Missing name for %s file. %s File could not be created"},
    {"missingMcroName","Missing macro name"},
    {"extraChrAfterMcroName","Extra characters after macro name"},
    {"tooManyMcros","There are too many macros in the .as file. The maximum allowed number of macros is %d"},
    {"macroRedefined", "Macro %s redifined"},
    {"tooManyMcroLines", "Macro lines limitation exceeded. Maximum %d lines are allowd in one macro"},
    {"lineTooLong", "line %d is too long, Maximum allowed line length is %d"},
    {"mcroOpName", "Illeagal macro name, %s is a name of operation and can not be used as a macro name"},
    {"extraCharsAfterMcroEnd", "Extra characters after macro end, on macro %s"},
    {"fileNotProcessed", "The file %s could not be processed."},
    {"labelNameMacroName", "The label %s is already defined as a macro name"},
    {"illeagalColumnInLine", "Column placed illeagaly in the line"},
    {"labelNameRegisterName", "%s is a register name and can not be used as a label"},
    {"labelNameOppName", "%s is an operation name and can not be used as a label"},
    {"labelNameRedefined", "%s is already defined as a label name and can not be redefined"},
    {"labelAllocError","Could not allocate memory for label %s"},
    {"labelNotAdded","Encounted error while adding label %s  to labels table"},
    {"labelNameTooLong","Label name %s is too long"},
    {"opNameNotFound", "The operation name %s is not a valid opearation name"},
    {"missingDotOnCommand", "Missing dot at the beginning of the command %s"},
    {"illeagalCharInCommand", "Illeagal char %c on command %s"},
    {"invalidNumArg", "Invalid number argument %s"},
    {"multipleConsecutiveCommas", "Multiple consecutive commas in arguments declaration"},
    {"illeagalCommaAtEnd", "Illegal comma at the end of the line"},
    {"dataImgInitFailed", "Memory allocation for data image failed"},
    {"dataImgReallocFailed", "Memory re-allocation for data image failed"},
    {"missingArgsForCommand", "Missing arguments for command"},
    {"strMissingOpeningQuote", "String %s is missing opening quotation mark"},
    {"strMissingClosingQuote", "String %s is missing closing quotation mark"},
    {"illeagalQuoteInStr", "Illeagal quotation mark inside string %s"},
    {"matArrMissingOpeningBracket","Definition of matrix is missing opening bracket"},
    {"matArrMissingClosingBracket", "Definition of matrix is missing closing bracket"},
    {"matArrBracketMisplaced", "Missplaced brackets in the matrix definition"},
    {"invalidNumArgInMat", "Definition of matrix - found invalid character in number argument"},
    {"tooManyNumbersInMatDef","Too many numbers in matrix definition, expected %d but got %d"},
    {"commNotFoundInLine", "Command %s could not be found in line %s"},
    {"memoryAllocationFailed", "Memory allocation failed"},
    {"errorGetCommType", "Could not identify the command type"},
    {"invalidInputOfLabel", "Invalid input for getLabelName function"},
    {"unsupportedArgCount", "Unsupported argeuments count for opp %s"},
    {"unexpectedChrsAfterCommand","Unexpected chars %s after command %s"},
    {"invalidArgsForCommand", "Invalid argument(s) for command %s"},
    {"labelNotFound", "Label %s was not found"},
    {"illegalAddressingMode", "Illeagal addressing mode for operation %s"},
    {"noOperandFound", "Missing operand"},
    {"unrecognizedOppCode", "Unrecognized OpCode %d"},
    {"invalidMatrixRegister", "Invalid matrix register, the matrix register %s is not a vaild register"},
    {"noNextWord", "Could not find any word in the line"},
    {"illeagalNumberInstant", "The number in the argument %s is not a valid number"},
    {"noNumbersFound", "No numbers were found after the # sign in the arg %s"},
    {"missingFileNames", "No file names entered, please enter file names to process. Try again"},
    {"fileNameTooLong", "File name %s is too long. Please shorten it and try again"}

};

void yieldError(char *errName, ...){
    int i;
    va_list args;

    errFlag = currentPass == FIRST_PASS ? TRUE : FALSE;

    for(i = 0; i < MAX_ERR_TABLE_SIZE; i++){
        if(!strcmp(errName,errorTable[i].name)) {
            fprintf(stderr, "[%d] [%s] Error! ", getRowNum(), getCurrentFileName());

            va_start(args,errName);

            vfprintf(stderr,errorTable[i].message,args);

            va_end(args);
            fprintf(stderr,"\n");
        }
    }
}
