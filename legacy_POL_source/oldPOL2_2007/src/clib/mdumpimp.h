#ifndef CLIB_MDUMPIMP_H
#define CLIB_MDUMPIMP_H

class HiddenMiniDumper
{
public:
	static Initialize( );

	//static LPCSTR m_szAppName;
    static bool _Initialized;
    static string _MiniDumpType;
    static unsigned _DumpCount;
    static char _StartTimestamp[32];

	static LONG WINAPI TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo );
};

#endif
