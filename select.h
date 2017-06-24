#ifndef SELECT_H_INCLUDED

#define SELECT_H_INCLUDED

void select(void (*callback)(char *, int), char *, ...);
void _trim(char *);
int _getInteger(char *);
void _garbageCollector(char ***, int);

#endif // SELECT_H_INCLUDED
