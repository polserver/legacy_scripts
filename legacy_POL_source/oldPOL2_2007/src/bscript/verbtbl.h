#ifndef __VERBTBL_H
#define __VERBTBL_H

/* need a return type here.-- no, don't care about types */
/* this is obviusly an in-memory structure only. */

class Verb 
{
public:
    char *verb;
    int narg;

	Verb( const char *name, int nargs );
};

int isInTable(Verb *table, int tableSize, const char *search, Verb **result);

#endif
