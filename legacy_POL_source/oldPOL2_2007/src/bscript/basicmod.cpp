// History
//   2005/11/24 Shinigami: added itoa for Linux because it is not ANSI C/C++
//   2005/11/29 Shinigami: mf_SplitWords will now accept each type of to-split-value as same as in the past
//   2006/10/07 Shinigami: FreeBSD fix - changed __linux__ to __unix__
//   2006/12/29 Shinigami: mf_SplitWords will not hang server on queue of delimiter

#include "clib/stl_inc.h"

#include <stdio.h>
#include <string.h>


#include "clib/clib.h"
#include "clib/rawtypes.h"
#include "clib/stlutil.h"
#include "clib/strutil.h"

#include "berror.h"
#include "bobject.h"
#include "execmodl.h"
#include "executor.h"
#include "impstr.h"

#include "basicmod.h"

BObjectImp* BasicExecutorModule::len()
{
	BObjectImp* imp = exec.getParamImp( 0 );
	if (imp->isa( BObjectImp::OTArray ))
	{
		ObjArray* arr = static_cast<ObjArray*>(imp);
		return new BLong( arr->ref_arr.size() );
	}
	else if (imp->isa(BObjectImp::OTString))
	{
		return new BLong( imp->getStringRep().length() );
	}
	else if (imp->isa( BObjectImp::OTError ))
	{
		BError* err = static_cast<BError*>(imp);
		return new BLong( err->mapcount() );
	}
	else
	{
		return new BLong(0);
	}
}

BObjectImp* BasicExecutorModule::left()
{
	const char *s = exec.paramAsString(0);
	double d = exec.paramAsDouble(1);

	return new String( s, (int) d );
}

BObjectImp* BasicExecutorModule::mid()
{
	return new String( "<MID is broken, use substrings>" );
/*
	BObject *oparm1;

	unsigned int begin;
	makeString(0);
	String *string = (String *) getParam(0);

	oparm1 = getParam(1);
	if (oparm1->isA("String"))
	{
		String *str2 = (String *) oparm1;
		begin = string->find(0,str2->strVal); substr
		if (!begin) return new String("");
	}
	else if (oparm1->isA("Double"))
	{
		Double *dbl = (Double *) oparm1;
		begin = dbl->dval ? (unsigned int)(dbl->dval-1) : 0;
	}
	else
	{
		begin = 0;
	}

	if (begin < 0 ||
		begin >= strlen(string->strVal))
	{
		return new String("");
	}
	return new String(string->strVal + begin);
*/
}

BObjectImp* BasicExecutorModule::find()
{
	exec.makeString(0);
	String* str = static_cast<String*>(exec.getParamImp(0));
	const char *s = exec.paramAsString(1);
	int d = static_cast<int>(exec.paramAsLong(2));


	int posn = str->find(d ? (d - 1) : 0, s) + 1;

	return new BLong (posn);
}

BObjectImp* BasicExecutorModule::lower()
{
	String *string = new String( exec.paramAsString(0) );
	string->toLower();
	return string;
}

BObjectImp* BasicExecutorModule::upper()
{
	String *string = new String( exec.paramAsString(0) );
	string->toUpper();
	return string;
}

BObjectImp* BasicExecutorModule::mf_CInt()
{
	BObjectImp* imp = exec.getParamImp( 0 );
	if (imp->isa( BObjectImp::OTLong ))
	{
		return imp->copy();
	}
	else if (imp->isa( BObjectImp::OTString ))
	{
		String* str = static_cast<String*>(imp);
		return new BLong( strtoul( str->data(), NULL, 0 ) );
	}
	else if (imp->isa( BObjectImp::OTDouble ))
	{
		Double* dbl = static_cast<Double*>(imp);
		return new BLong( static_cast<long>(dbl->value()) );
	}
	else
	{
		return new BLong(0);
	}
}

BObjectImp* BasicExecutorModule::mf_CDbl()
{
	BObjectImp* imp = exec.getParamImp( 0 );
	if (imp->isa( BObjectImp::OTLong ))
	{
		BLong* lng = static_cast<BLong*>(imp);
		return new Double( lng->value() );
	}
	else if (imp->isa( BObjectImp::OTString ))
	{
		String* str = static_cast<String*>(imp);
		return new Double( strtod( str->data(), NULL ) );
	}
	else if (imp->isa( BObjectImp::OTDouble ))
	{
		return imp->copy();
	}
	else
	{
		return new Double(0);
	}
}

BObjectImp* BasicExecutorModule::mf_CStr()
{
	BObjectImp* imp = exec.getParamImp(0);
	return new String( imp->getStringRep() );
}

BObjectImp* BasicExecutorModule::mf_CAsc()
{
	BObjectImp* imp = exec.getParamImp( 0 );
	if (imp->isa( BObjectImp::OTString ))
	{
		String* str = static_cast<String*>(imp);
		return new BLong( static_cast<unsigned char>(str->data()[0]) );
	}
	else
	{
		return new BError( "Invalid parameter type" );
	}
}

BObjectImp* BasicExecutorModule::mf_CAscZ()
{
	BObjectImp* imp = exec.getParamImp( 0 );
	string tmp = imp->getStringRep();
	ObjArray* arr = new ObjArray;
	for( unsigned i = 0; i < tmp.size(); ++i )
	{
		arr->addElement( new BLong( static_cast<unsigned char>(tmp[i]) ) );
	}
	return arr;
}

BObjectImp* BasicExecutorModule::mf_CChr()
{
	long val;
	if (getParam( 0, val ))
	{
		char s[2];
		s[0] = static_cast<char>(val);
		s[1] = '\0';
		return new String( s );
	}
	else
	{
		return new BError( "Invalid parameter type" );
	}
}

BObjectImp* BasicExecutorModule::mf_CChrZ()
{
	string res;
	ObjArray* arr = static_cast<ObjArray*>(exec.getParamImp( 0, BObjectImp::OTArray ));
	if (!arr)
		return new BError( "Invalid parameter type" );
	for( ObjArray::const_iterator itr = arr->ref_arr.begin(); itr != arr->ref_arr.end(); itr++ )
	{
		BObjectImp* imp = (*itr).get()->impptr();
		if (imp)
		{
			if (imp->isa( BObjectImp::OTLong ))
			{
				BLong* blong = static_cast<BLong*>(imp);
				char s[2];
				s[0] = static_cast<char>(blong->value());
				s[1] = '\0';
				res += s;
			}
		}

	}
	return new String(res);

}

BObjectImp* BasicExecutorModule::mf_Hex()
{
	BObjectImp* imp = exec.getParamImp(0);
	if (imp->isa( BObjectImp::OTLong ))
	{
		BLong* plong = static_cast<BLong*>(imp);
		char s[20];
		sprintf( s, "0x%lX", plong->value() );
		return new String( s );
	}
	else if (imp->isa( BObjectImp::OTDouble ))
	{
		Double* pdbl = static_cast<Double*>(imp);
		char s[20];
		sprintf( s, "0x%lX", static_cast<unsigned long>(pdbl->value()) );
		return new String( s );
	}
	else if (imp->isa( BObjectImp::OTString ))
	{
		String* str = static_cast<String*>(imp);
		char s[20];
		sprintf( s, "0x%lX", strtoul( str->data(), NULL, 0 ) );
		return new String( s );
	}
	else
	{
		return new BError( "Hex() expects an Integer, Real, or String" );
	}
}

#ifdef __unix__
char* itoa( long value, char* result, long base ) {
	// check that the base if valid
	if (base < 2 || base > 16) { *result = 0; return result; }
	
	char* out = result;
	long quotient = value;
	
	do {
		*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
		++out;
		quotient /= base;
	} while ( quotient );
	
	// Only apply negative sign for base 10
	if ( value < 0 && base == 10) *out++ = '-';
	
	std::reverse( result, out );
	*out = 0;
	return result;
}
#endif

BObjectImp* BasicExecutorModule::mf_Bin()
{
	BObjectImp* imp = exec.getParamImp(0);
	if (imp->isa( BObjectImp::OTLong ))
	{
		BLong* plong = static_cast<BLong*>(imp);
		long number = plong->value();
		char buffer [sizeof(long)*8+1];
		return new String(itoa(number, buffer, 2));
	}
	else
	{
		return new BError( "Bin() expects an Integer" );
	}
}

int bpx;
void bphelp()
{
	++bpx;
}

BObjectImp* BasicExecutorModule::mf_SplitWords()
{
	BObjectImp* bimp_split = exec.getParamImp(0);
	string source = bimp_split->getStringRep();

	const String* bimp_delimiter;
	if ( !exec.getStringParam(1, bimp_delimiter) )
	{
		return new BError("Invalid parameter type.");
	}
	string delimiter = bimp_delimiter->getStringRep();

	ObjArray* objarr = new ObjArray;

	// Support for how it previously worked.
	// Kept to support spaces and tabs as the same.
	if ( delimiter == " " )
	{
		ISTRINGSTREAM is(source);
		string tmp;
		while ( is >> tmp )
		{
			objarr->addElement(new String(tmp));
			tmp = "";
		}
		return objarr;
	}

	// New delimiter support.
	string new_string = source;
	string tmp_string, add_string;
	string::size_type found;
	do
	{
		found = new_string.find(delimiter, 0);
		if ( found == string::npos )
			break;
		
		add_string = new_string.substr(0, found);
		
		// Shinigami: will not hang server on queue of delimiter
		//if ( add_string.empty() )
		//	continue;
		
		objarr->addElement(new String(add_string));
		tmp_string = new_string.substr(found+delimiter.length(), new_string.length());
		new_string = tmp_string;
	}
	while ( found != string::npos/*-1*/ );

	// Catch leftovers here.
	if ( !new_string.empty() )
		objarr->addElement(new String(new_string));

	return objarr;
}

BObjectImp* BasicExecutorModule::mf_Pack()
{
	BObjectImp* imp = exec.getParamImp(0);
	return new String( imp->pack() );
}

BObjectImp* BasicExecutorModule::mf_Unpack()
{
	const String* str;

	if (exec.getStringParam( 0, str ))
	{
		return BObjectImp::unpack( str->data() );
	}
	else
	{
		return new BError( "Invalid parameter type" );
	}
}
BObjectImp* BasicExecutorModule::mf_TypeOf()
{
	BObjectImp* imp = exec.getParamImp(0);
	return new String( imp->typeOf() );
}
BObjectImp* BasicExecutorModule::mf_SizeOf()
{
	BObjectImp* imp = exec.getParamImp(0);
	return new BLong( imp->sizeEstimate() );
}

BasicFunctionDef BasicExecutorModule::function_table[] =
{
	{ "mid",		&BasicExecutorModule::mid },
	{ "left",		&BasicExecutorModule::left },
	{ "find",		&BasicExecutorModule::find },
	{ "len",		&BasicExecutorModule::len },
	{ "upper",		&BasicExecutorModule::upper },
	{ "lower",		&BasicExecutorModule::lower },
	{ "CInt",		&BasicExecutorModule::mf_CInt },
	{ "CStr",		&BasicExecutorModule::mf_CStr },
	{ "CDbl",		&BasicExecutorModule::mf_CDbl },
	{ "CAsc",		&BasicExecutorModule::mf_CAsc },
	{ "CChr",		&BasicExecutorModule::mf_CChr },
	{ "CAscZ",		&BasicExecutorModule::mf_CAscZ },
	{ "CChrZ",		&BasicExecutorModule::mf_CChrZ },
	{ "Bin",		&BasicExecutorModule::mf_Bin },
	{ "Hex",		&BasicExecutorModule::mf_Hex },
	{ "SplitWords", &BasicExecutorModule::mf_SplitWords },
	{ "Pack",		&BasicExecutorModule::mf_Pack },
	{ "Unpack",		&BasicExecutorModule::mf_Unpack },
	{ "TypeOf",		&BasicExecutorModule::mf_TypeOf },
	{ "SizeOf",		&BasicExecutorModule::mf_SizeOf }
};

int BasicExecutorModule::functionIndex( const char *name )
{
	for( unsigned idx = 0; idx < arsize(function_table); idx++ )
	{
		if (stricmp( name, function_table[idx].funcname ) == 0)
			return idx;
	}
	return -1;
}

BObjectImp* BasicExecutorModule::execFunc( unsigned funcidx )
{
	return callMemberFunction(*this, function_table[funcidx].fptr)();
};
