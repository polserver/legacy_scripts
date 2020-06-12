#include "clib/stl_inc.h"

#include <string.h>

#include "impstr.h"


ostream& operator << (ostream& os, const BObject& obj)
{
    obj.impptr()->printOn(os);
    return os;
}

ostream& operator << (ostream& os, const BObjectImp& objimp )
{
    objimp.printOn(os);
    return os;
}
void BObject::printOn(ostream& os) const
{
	os << "{ BObject: " << BObjectImp::typestr(objimp->type()) << " }";
}

void BObjectImp::printOn(ostream& os) const
{
	os << "{ BObjectImp: " << typestr(type()) << " }";
}

void UninitObject::printOn( ostream& os ) const
{
	os << "{ Uninitialized Object }";
}

void ObjArray::printOn( ostream& os ) const
{
	os << " { Array[" << ref_arr.size() << "] }";
}

/*
void ArrayElem::printOn( ostream& os ) const
{
	os << " { ArrayElem }";
}
*/

void String::printOn(ostream& os) const
{
   os << '\"' << value_ << '\"';
}

#if 0
void SubString::printOn( ostream& os ) const
{
	//int length;
	// length = strlen( string->strVal );
    os << "{ \"";
    os << ref_String->value_.substr( begin-1, len );

/*
    for( int i = 0; i < len; i++ )
    { 
        int idx = begin + i - 1;
		if (idx < length)
			os << string->strVal[idx];
    }
*/
    os << "\" }";
}
#endif

void Double::printOn(ostream& os) const
{
    // CHECKME: consider setting precision
   os << dval_;
}

void BLong::printOn(ostream& os ) const
{
    os << lval_;
}
/*
void Variable::printOn(ostream& os) const
{
   os << varName << ": ";
   if (obj) os << *obj;
}
*/

