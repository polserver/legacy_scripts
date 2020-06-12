#include "clib/stl_inc.h"

#include <string.h>

#include "clib/clib.h"
#include "clib/mlog.h"
#include "clib/stlutil.h"

#include "eprog.h"
#include "executor.h"
#include "execmodl.h"
#include "impstr.h"
 

const string& ExecutorModule::scriptname() const
{
    return exec.prog_->name;
}

/*
int ExecutorModule::makeString(unsigned param)
{
    return exec.makeString(param);
}
*/

/*
int ExecutorModule::makeDouble(unsigned param)
{
    return exec.makeDouble(param);
}
*/

/*
const char *ExecutorModule::paramAsString(unsigned param)
{
    return exec.paramAsString(param);
}
*/

/*
double ExecutorModule::paramAsDouble(unsigned param)
{
    return exec.paramAsDouble(param);
}
*/

/*
long ExecutorModule::paramAsLong(unsigned param)
{
    return exec.paramAsLong(param);
}
*/


/*
BObjectImp* ExecutorModule::getParamImp(unsigned param)
{
    return exec.getParamImp(param);
}
*/

