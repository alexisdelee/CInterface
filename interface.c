#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>

#include "interface.h"

Interface interface_init()
{
    Interface interface;

    interface.options.title = "[?] Please select";
    interface.options.PS3mode = PS3_NUMBERING;
    interface.options.PS3 = ">";
    interface.options.exit = false;

    interface.load = interface_load;
    interface.prompt = interface_prompt;

    return interface;
}

void interface_load(Options *options, char *configPath)
{
    // FILE *configFile;
}

void interface_prompt(void (*callback)(char *, int), Options options, ...)
{
    char *choice;
    char **choices = malloc(sizeof(char *));
    int counter = 0, index, isExitOption = false;
    va_list args;

    if(choices == NULL) {
        printf("Exception: error with malloc\n");
        exit(-1);
    }

    va_start(args, options);

    printf("%s\n", options.title);
    while((choice = va_arg(args, char *))) {
        if((choices[counter] = malloc(sizeof(char) * 501)) == NULL) {
            printf("Exception: error with malloc\n");
            exit(-1);
        } else {
            sprintf(choices[counter], "%s", choice);
        }

        if(options.PS3mode & PS3_BULLET_POINT) {
            printf("%s %s\n", options.PS3, choice);
            counter++;
        } else {
            printf("%d) %s\n", ++counter, choice);
        }

        if((choices = realloc(choices, sizeof(char *) * (counter + 1))) == NULL) {
            printf("Exception: error with malloc\n");
            exit(-1);
        }
    }

    if(options.exit) {
        if(options.PS3mode & PS3_BULLET_POINT) {
            printf("%s quit\n", options.PS3);
            counter++;
        } else {
            printf("0) quit\n");
        }

        isExitOption = true;
    }

    va_end(args);

    const int LIMIT_STRING = (int)floor(log10(counter)) + 1;
    char response[LIMIT_STRING + 1];

    printf("\n");
    do {
        printf("#? ");
        fgets(response, 101, stdin);
        fflush(stdin); // clean stdin

        _trim(&response[0]);
        if(strlen(response) > LIMIT_STRING) {
            continue;
        }

        index = _getInteger(&response[0]);
        if(isExitOption && index == 0) {
            _garbageCollector(&choices, counter + 1);
            return;
        }
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
