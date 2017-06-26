#ifndef INTERFACE_H_INCLUDED

#define INTERFACE_H_INCLUDED

#define PS3_BULLET_POINT 0x01
#define PS3_NUMBERING 0x02

typedef struct Options Options;
struct Options
{
    char *title;
    int PS3mode;
    char *PS3;
};

typedef struct Interface Interface;
struct Interface
{
    void (*load)(Options *, char *);
    void (*prompt)(void (*callback)(char *, int), Options, ...);
    Options options;
};

Interface interface_init();
void interface_load(Options *, char *);
void interface_prompt(void (*callback)(char *, int), Options, ...);
void _trim(char *);
int _getInteger(char *);
void _garbageCollector(char ***, int);

#endif // INTERFACE_H_INCLUDED
