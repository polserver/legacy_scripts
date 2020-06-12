#ifndef __BSCRIPT_BASICIO_H
#define __BSCRIPT_BASICIO_H
#define __BASICIO_H

#ifndef __EXECMODL_H
#include "execmodl.h"
#endif

#include <iosfwd>

class BasicIoExecutorModule;

typedef BObjectImp* (BasicIoExecutorModule::*BasicIoExecutorModuleFn)();

struct BasicIoFunctionDef
{
	char funcname[ 33 ];
	BasicIoExecutorModuleFn fptr;
};
 

class BasicIoExecutorModule : public ExecutorModule
{
  public:
    BObjectImp* print();

    BasicIoExecutorModule(Executor& exec,
                          ostream& cout) : ExecutorModule("BasicIo", exec),
                                                cout_(cout) { }

	// class machinery
  protected:
	virtual BObjectImp* execFunc( unsigned idx );
	virtual int functionIndex( const char *func );
	static BasicIoFunctionDef function_table[];
  private:
    ostream& cout_;
};

#endif
