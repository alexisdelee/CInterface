#ifndef INTERFACE_H_INCLUDED

#define INTERFACE_H_INCLUDED

#define PS3_BULLET_POINT 0x01
#define PS3_NUMBERING 0x02

#define true 1
#define false 0

typedef struct Options Options;
struct Options
{
    char *title;
    char *PS3;
    int PS3mode;
    int quit;
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
void interface_free(Options *);

#endif // INTERFACE_H_INCLUDED
