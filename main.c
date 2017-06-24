#include <stdio.h>
#include <stdlib.h>

#include "select.h"

void callback(char *, int);

int main(int argc, char **argv)
{
    select(callback, "Select your response:", "one", "two", NULL);

    return 0;
}

void callback(char *value, int index)
{
    printf("\n[%d] %s\n", index, value);
}
