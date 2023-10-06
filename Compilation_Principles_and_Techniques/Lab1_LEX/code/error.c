#include "error.h"

void error(enum ErrorType error_type, int line, int col, char *buffer)
{
    printf("Error: %d:%d: ", line, col);
    switch (error_type)
    {
    case UNKNOWN_SYMBOL:
        printf("Unknown symbol: ");
        break;
    case UNCLOSED_STRING:
        printf("Unclosed const string: ");
        break;
    case UNCLOSED_CHAR:
        printf("Unclosed const char: ");
        break;
    case UNCLOSED_ANNOTATION:
        printf("Unclosed annotation: ");
        break;
    case EMPTY_CHAR:
        printf("Empty const char: ");
        break;
    default:
        break;
    }
    printf("%s\n", buffer);
}