#include <stdio.h>
#include <stdlib.h>

#include "interface.h"

void displayFunc(char *, int);

int main(int argc, char **argv)
{
    Interface interface = interface_init();

    // interface.load(&interface.options, "conf.json");
    interface.prompt(displayFunc, interface.options, "one", "two", "three", NULL);

    return 0;
}

void displayFunc(char *value, int index)
{
    printf("\n[%d] %s\n", index, value);
}
