// History
//   2007/12/09 Shinigami: removed ( is.peek() != EOF ) check from String::unpackWithLen()
//                         will not work with Strings in Arrays, Dicts, etc.
//   2008/02/08 Turley: String::unpackWithLen() will accept zero length Strings

#include "clib/stl_inc.h"


#include <stdlib.h>
#include <string.h>

#ifdef __GNUG__
#include <streambuf>
#endif

#include "berror.h"
#include "bobject.h"
#include "impstr.h"


String::String(BObjectImp& objimp) : BObjectImp( OTString )
{
    value_ = objimp.getStringRep();
}

String::String(const char *s, int len) :
    BObjectImp( OTString ),
    value_( s, len )
{
}

String *String::StrStr(int begin, int len)
{
	return new String( value_.substr( begin-1, len ) );
}

string String::pack() const
{
    return "s" + value_;
}

void String::packonto( ostream& os ) const
{
    os << "S" << value_.size() << ":" << value_;
}
void String::packonto( ostream& os, const string& value )
{
    os << "S" << value.size() << ":" << value;
}

BObjectImp* String::unpack( const char* pstr )
{
    return new String( pstr );
}

BObjectImp* String::unpack( istream& is )
{
    string tmp;
    getline( is, tmp );

    return new String( tmp );
}

BObjectImp* String::unpackWithLen( istream& is )
{
    unsigned len;
	char colon; 
    if (! (is >> len >> colon))
    {
        return new BError( "Unable to unpack string length." );
    }
	if ( (int)len < 0 )
    {
		return new BError( "Unable to unpack string length. Invalid length!" );
    }
	if ( colon != ':' ) 
	{ 
		return new BError( "Unable to unpack string length. Bad format. Colon not found!" ); 
	}

	// is >> ios::noskipws;
    is.unsetf( ios::skipws );
    string tmp;
    tmp.reserve( len );
    while (len--)
    {
		char ch = '\0'; 
		if(!(is >> ch) || ch == '\0') 
		{ 
			return new BError( "Unable to unpack string length. String length excessive."); 
		}
		tmp += ch;
    }

	is.setf( ios::skipws );
    return new String( tmp );
}

unsigned long String::sizeEstimate() const
{
    return sizeof(String) + value_.capacity();
}

// 0-based string find
    // find( "str srch", 2, "srch"):
    //        01^-- start
int String::find(int begin, const char *target)
{
    // TODO: check what happens in string if begin position is out of range
    string::size_type pos;
    pos = value_.find( target, begin );
    if (pos == string::npos)
        return -1;
    else
        return pos;
}

// Returns the amount of alpha-numeric characters in string.
unsigned int String::alnumlen(void) const
{
	unsigned int c=0;
	while (isalnum(value_[c]))
	{
		c++;
	}
	return c;
}

void String::reverse(void)
{
    ::reverse( value_.begin(), value_.end() );
}

/*
void String::build(const char *str1, const char *str2)
{
    if (strVal) delete strVal; strVal = NULL;
    int strLen = (str1 ? strlen(str1) : 0) + (str2 ? strlen(str2) : 0);
    strVal = new char[strLen+1];
    if (strVal) {
        strVal[0] = '\0';
        if (str1) strcpy(strVal, str1);
        if (str2) strcat(strVal, str2);
    }
}
*/

void String::set( char *newstr )
{
    value_ = newstr;
    delete newstr;
}


BObjectImp* String::selfPlusObjImp(const BObjectImp& objimp) const
{
    return new String( value_ + objimp.getStringRep() );
}

void String::remove(const char *rm)
{
    int len = strlen(rm);
    string::size_type pos = value_.find( rm );
    if (pos != string::npos)
        value_.erase( pos, len );
}

BObjectImp* String::selfMinusObjImp(const BObjectImp& objimp) const
{
    String *tmp = (String *) copy();
    if (objimp.isa( OTString )) 
    {
        const String& to_remove = (String&) objimp;
        tmp->remove(to_remove.value_.data());
    } 
    else /* if (objimp.isa("Double"))  */
    {
        tmp->remove( objimp.getStringRep().data() );
    }
    return tmp;
}


bool String::isEqual(const BObjectImp& objimp) const
{
    if (objimp.isa( OTString ))
    {
        return ( value_ == static_cast<const String&>(objimp).value_ );
    }
    else
    {
        return false;
    }
}

/* since non-Strings show up here as not equal, we make them less than. */
bool String::isLessThan(const BObjectImp& objimp) const
{
    if (objimp.isa( OTString ))
    {
        return ( value_ < static_cast<const String&>(objimp).value_ );
    }
    else if (objimp.isa( OTUninit ) || objimp.isa( OTError ))
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
int String::find(char *s, int *posn)
{
    char *t;
    t = strstr(strVal, s);
    if (!t) return -1;

    *posn = (int) (t - strVal);
    return 0;
}
*/

String *String::midstring(int begin, int len) const
{
    return new String( value_.substr( begin-1, len ) );
/*    int length = strlen(strVal);
    if (begin > length || begin < 1 || len < 1 ) return new String("");
    return new String( strVal + begin - 1, len );
*/
}

void String::toUpper( void )
{
    // UNTESTED
    // FIXME ??
//    toupper( value_ );
/*
    string::size_type len = value_.length();
    for( string::size_type i = 0; i < len; i++ )
    {
        value_[i] = toupper 
    }
*/
    for( string::iterator itr = value_.begin(); itr != value_.end(); itr++ )
    {
        *itr = toupper(*itr);
    }
}

void String::toLower( void )
{
#if 0
    // UNTESTED

     using use_facet;
    // FIXME oh, this c++ internationalization crap is pissing me off.
     // why no simply way to lower/upper a string? come on.
    _USE( locale::locale(), ctype<char> ).tolower( value_.begin(), value_.end() );
    // use_facet< ctype<char> >( locale::locale() ).tolower(value_.begin(), value_.end());
#else
    for( string::iterator itr = value_.begin(); itr != value_.end(); itr++ )
    {
        *itr = tolower(*itr);
    }
#endif
}

BObjectImp* String::array_assign( BObjectImp* idx, BObjectImp* target )
{
    string::size_type pos, len;

    // first, determine position and length.
    if (idx->isa( OTString ))
    {
		String& rtstr = (String&) *idx;
        pos = value_.find( rtstr.value_ );
        len = rtstr.length();
    }
    else if (idx->isa( OTLong ))
    {
        BLong& lng = (BLong&) *idx;
        pos = lng.value()-1;
        len = 1;
    }
    else if (idx->isa( OTDouble ))
    {
        Double& dbl = (Double&)*idx;
        pos = static_cast<string::size_type>(dbl.value());
        len = 1;
    }
    else
    {
        return UninitObject::create();
    }

    if (pos != string::npos)
    {
        if (target->isa( OTString ))
        {
            String* target_str = (String*) target;
            value_.replace( pos, len, target_str->value_ );
        }
        return this;
        //return BObjectRef(getsubstr( pos+1, rtstr.value_.length() ));
    }
    else
    {
        return UninitObject::create();
    }
}

BObjectRef String::OperMultiSubscriptAssign( stack<BObjectRef>& indices, BObjectImp* target )
{
    BObjectRef start_ref = indices.top();
    indices.pop();
    BObjectRef length_ref = indices.top();
    indices.pop();

    BObject& length_obj = *length_ref;
    BObject& start_obj = *start_ref;

    BObjectImp& length = length_obj.impref();
    BObjectImp& start = start_obj.impref();

    // first deal with the start position.
    unsigned index;
    if (start.isa( OTLong ))
    {
        BLong& lng = (BLong&) start;
        index = (unsigned) lng.value();
        if (index == 0 || index > value_.size())
            return BObjectRef(new BError( "Subscript out of range" ));

    }
    else if (start.isa( OTString ))
    {
		String& rtstr = (String&) start;
        string::size_type pos = value_.find( rtstr.value_ );
        if (pos != string::npos)
            index = pos+1;
        else
            return BObjectRef(new UninitObject);
    }
    else
    {
        return BObjectRef(copy());
    }

    // now deal with the length.
    int len;
    if (length.isa( OTLong ))
    {
		BLong& lng = (BLong &) length;

        len = (int)lng.value();
    }
    else if (length.isa( OTDouble ))
    {
		Double& dbl = (Double &) length;

        len = (int)dbl.value();
    }
    else
    {
        return BObjectRef( copy() );
    }

    if (target->isa( OTString ))
    {
        String* target_str = (String*) target;
        value_.replace( index-1, len, target_str->value_ );
    }
    else
    {
        return BObjectRef( UninitObject::create() );
    }

    return BObjectRef( this );
}


BObjectRef String::OperMultiSubscript( stack<BObjectRef>& indices )
{
    BObjectRef start_ref = indices.top();
    indices.pop();
    BObjectRef length_ref = indices.top();
    indices.pop();

    BObject& length_obj = *length_ref;
    BObject& start_obj = *start_ref;

    BObjectImp& length = length_obj.impref();
    BObjectImp& start = start_obj.impref();

    // first deal with the start position.
    unsigned index;
    if (start.isa( OTLong ))
    {
        BLong& lng = (BLong&) start;
        index = (unsigned) lng.value();
        if (index == 0 || index > value_.size())
            return BObjectRef(new BError( "Subscript out of range" ));

    }
    else if (start.isa( OTString ))
    {
		String& rtstr = (String&) start;
        string::size_type pos = value_.find( rtstr.value_ );
        if (pos != string::npos)
            index = pos+1;
        else
            return BObjectRef(new UninitObject);
    }
    else
    {
        return BObjectRef(copy());
    }

    // now deal with the length.
    int len;
    if (length.isa( OTLong ))
    {
		BLong& lng = (BLong &) length;

        len = (int)lng.value();
    }
    else if (length.isa( OTDouble ))
    {
		Double& dbl = (Double &) length;

        len = (int)dbl.value();
    }
    else
    {
        return BObjectRef( copy() );
    }

    String* str = new String( value_, index-1, len );
    return BObjectRef( str );
}

BObjectRef String::OperSubscript( const BObject& rightobj )
{
    const BObjectImp& right = rightobj.impref();
    if (right.isa( OTLong ))
    {
		BLong& lng = (BLong& ) right;

		unsigned index = (unsigned)lng.value();
		
        if (index == 0 || index > value_.size())
            return BObjectRef(new BError( "Subscript out of range" ));

		return BObjectRef( new BObject( new String( value_.c_str()+index-1, 1 ) ) );
    }
    else if (right.isa( OTDouble ))
	{
		Double& dbl = (Double& ) right;

		unsigned index = (unsigned)dbl.value();
		
        if (index == 0 || index > value_.size())
            return BObjectRef(new BError( "Subscript out of range" ));

		return BObjectRef( new BObject( new String( value_.c_str()+index-1, 1 ) ) );
	}
	else if (right.isa( OTString ))
	{
		String& rtstr = (String&) right;
        string::size_type pos = value_.find( rtstr.value_ );
        if (pos != string::npos)
            return BObjectRef( new BObject( new String( value_, pos, 1 ) ) );
        else
            return BObjectRef(new UninitObject);
    }
    else
    {
        return BObjectRef(new UninitObject);
//        return BObjectRef(copy());
    }
}

