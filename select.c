#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>

#include "select.h"

void select(void (*callback)(char *, int), char *title, ...)
{
    char *choice;
    char **choices = malloc(sizeof(char *));
    int counter = 0, index;
    va_list args;

    va_start(args, title);

    printf("%s\n", title);
    while((choice = va_arg(args, char *))) {
        choices[counter] = choice;
        printf("%d) %s\n", ++counter, choice);
        choices = realloc(choices, sizeof(char *) * counter);
    }

    va_end(args);

    const int LIMIT_STRING = (int)floor(log10(counter)) + 1;
    char response[LIMIT_STRING + 1];

    printf("\n");
    do {
        printf("#? ");
        fgets(response, LIMIT_STRING + 1, stdin);
        fflush(stdin); // clean stdin

        _trim(&response[0]);
        index = _getInteger(&response[0]);
    } while(index < 1 || index > counter);

    callback(choices[index - 1], index); // cast char[] to char*
    _garbageCollector(&choices, counter + 1);
}

void _trim(char *value)
{
    int wordSize = strlen(value);
    char lastCharacter = value[wordSize - 1];

    if(lastCharacter == 0x20 || lastCharacter == 0x09 || lastCharacter == 0x0A || lastCharacter == 0x0D || lastCharacter == 0x0B) {
        value[wordSize - 1] = '\0';
    }
}

int _getInteger(char *value)
{
    char *endptr;
    long index = strtol(value, &endptr, 10);

    if((errno == ERANGE && (index == LONG_MIN || index == LONG_MAX))
        || (errno != 0 && index == 0)) {
        return -1;
    } else if(endptr == value) {
        return -1;
    }

    return index;
}

void _garbageCollector(char ***element, int counter)
{
    int index = 0;

    for( ; index < counter; index++) {
        free(element[counter]);
    }

    free(element);
    free(*element);
}
