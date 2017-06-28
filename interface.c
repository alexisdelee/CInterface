#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>

#include "interface.h"

#ifdef __unix__
    #define OS_Windows false
#elif defined(_WIN32) || defined(WIN32)
    #define OS_Windows true
#endif

Interface interface_init()
{
    Interface interface;

    interface.options.title = "[?] Please select";
    interface.options.PS3mode = PS3_NUMBERING;
    interface.options.PS3 = ">";
    interface.options.quit = false;

    interface.load = interface_load;
    interface.prompt = interface_prompt;

    return interface;
}

void interface_load(Options *options, char *configPath)
{
    FILE *configFile = fopen(configPath, "r");
    char line[501], *command = NULL;
    int delimiter;

    if(configFile == NULL) {
        printf("Exception: error with the configuration file\n");
        return;
    }

    while(fgets(line, 501, configFile)) {
        _trim(&line[0]); // \r or \n for Mac, Unix and Windows
        if(OS_Windows) {
            _trim(&line[0]); // \n for Windows
        }

        if((delimiter = strcspn(line, "=")) == strlen(line)) {
            continue;
        }

        command = malloc(sizeof(char) * (delimiter + 1));
        if(command == NULL) {
            printf("Exception: error with malloc\n");
            exit(-1);
        } else {
            strncpy(command, line, delimiter);
        }

        if(!strcmp(command, "title")) {
            options->title = strdup(line + delimiter + 1);
        } else if(!strcmp(command, "PS3mode")) {
            if(!strcmp(line + delimiter + 1, "BULLET_POINT")) {
                options->PS3mode = PS3_BULLET_POINT;
            } else if(!strcmp(line + delimiter + 1, "PS3_NUMBERING")) {
                options->PS3mode = PS3_NUMBERING;
            }
        } else if(!strcmp(command, "PS3") && options->PS3mode == PS3_BULLET_POINT) {
            options->PS3 = strdup(line + delimiter + 1);
        } else if(!strcmp(command, "quit")) {
            if(!strcmp(line + delimiter + 1, "false")) {
                options->quit = false;
            } else if(!strcmp(line + delimiter + 1, "true")) {
                options->quit = true;
            }
        }

        free(command);
        command = NULL;
    }

    fclose(configFile);
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

    if(options.quit) {
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

        index = _getInteger(&response[0]); // cast char[] to char *
        if(isExitOption && index == 0) {
            _garbageCollector(&choices, counter + 1);
            return;
        }
    } while(index < 1 || index > counter);

    callback(choices[index - 1], index);
    _garbageCollector(&choices, counter + 1);
}

void interface_free(Options *options)
{
    free(options->title);
    free(options->PS3);
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
