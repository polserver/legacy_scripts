#ifndef __CLIB_LOGFILE_H
#define __CLIB_LOGFILE_H

#include <stdio.h>


extern FILE *logfile;
int OpenLogFileName( const char* namebase, bool rollover );
int OpenLogFile(void);
void CloseLogFile(void);
void Log( char *fmt, ... );
void Log2( char *fmt, ... ); // outputs to stderr also


#endif
