#ifndef __BSCRIPT_BASICMOD_H
#define __BSCRIPT_BASICMOD_H
#define __BASICMOD_H

#ifndef __EXECMODL_H
#include "execmodl.h"
#endif

// NOTE, this is currently misnamed, should be StringExecutorModule

class BasicExecutorModule;

typedef BObjectImp* (BasicExecutorModule::*BasicExecutorModuleFn)();

struct BasicFunctionDef
{
	char funcname[ 33 ];
	BasicExecutorModuleFn fptr;
};


class BasicExecutorModule : public ExecutorModule
{
  public:
        /* These probably belong in a string module */
	BObjectImp* find();
    BObjectImp* len();
	BObjectImp* upper();
	BObjectImp* lower();
	BObjectImp* mf_substr();
    BObjectImp* mf_CInt();
    BObjectImp* mf_CStr();
    BObjectImp* mf_CDbl();
    BObjectImp* mf_CAsc();
    BObjectImp* mf_CAscZ();
    BObjectImp* mf_CChr();
    BObjectImp* mf_CChrZ();
	
	BObjectImp* mf_Bin();
    BObjectImp* mf_Hex();
    BObjectImp* mf_SplitWords();

    BObjectImp* mf_Pack();
    BObjectImp* mf_Unpack();

    BObjectImp* mf_TypeOf();
    BObjectImp* mf_SizeOf();

    BasicExecutorModule(Executor& exec) : ExecutorModule("Basic", exec) { }

	// class machinery
  protected:
	virtual BObjectImp* execFunc( unsigned idx );
	virtual int functionIndex( const char *func );
	static BasicFunctionDef function_table[];
};

#endif
