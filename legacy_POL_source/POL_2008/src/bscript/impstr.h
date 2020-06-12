#ifndef H_BSCRIPT_IMPSTR_H
#define H_BSCRIPT_IMPSTR_H
#define H_IMPSTR_H

#ifndef H_BOBJECT_H
#include "bobject.h"
#endif

#include <string>

class SubString;

class String : public BObjectImp
{
  public:
    String() : BObjectImp(OTString), value_("") { }
	
    String(const char *str, int nchars);
    explicit String(const char *str) : BObjectImp(OTString), value_(str) { }
    explicit String(const string& str) : BObjectImp(OTString), value_(str) { }
    explicit String(const string& str, string::size_type pos, string::size_type n ) : BObjectImp(OTString), value_(str,pos,n) {} 
    String(const char *left, const char *right) : 
        BObjectImp(OTString), 
        value_( string(left) + string(right) )
        { }

    String(const String& left, const String& right) : 
        BObjectImp(OTString), 
        value_( left.value_ + right.value_ )
        { }
    explicit String(BObjectImp& objimp);
    String(const String& str) : BObjectImp(OTString), value_( str.value_ )
        { }
    
    static BObjectImp* unpack( const char* pstr );
    static BObjectImp* unpack( istream& is );
    static BObjectImp* unpackWithLen( istream& is );
    virtual string pack() const;
    virtual void packonto( ostream& os ) const;
    static void packonto( ostream& os, const string& value );
    virtual BObjectImp *copy() const
      {
		  return new String(*this);
      }
    virtual unsigned long sizeEstimate() const; 

	//Possibly eliminate this later and have [ ] operator support?
	//Or stick to functions, overload them with other substring getting methods.
	String *StrStr(int begin, int len);

    // void build(const char *str1, const char *str2);
	void set( char *newstr ); /* String now owns newstr */
    const char *data() const { return value_.c_str(); }
    const string& value() const { return value_; }
    unsigned length() const { return value_.length(); }
    void toUpper(void);
    void toLower(void);
	
    virtual ~String()
        { }
    
	String& operator=(const char *s) { value_ = s; return *this; }
	String& operator=(const String& str) { value_ = str.value_; return *this; }
    void copyvalue( const String& str ) { value_ = str.value_; }
	operator const char *() const { return value_.data(); }
		
    void remove(const char *s);
    virtual bool isTrue() const { return !value_.empty(); }
  public:
    virtual BObjectImp* selfPlusObjImp( const BObjectImp& ) const;
    virtual BObjectImp* selfMinusObjImp( const BObjectImp& ) const;
    virtual BObjectRef OperSubscript( const BObject& obj );
    virtual BObjectRef OperMultiSubscript( stack<BObjectRef>& indices );
    virtual BObjectRef OperMultiSubscriptAssign( stack<BObjectRef>& indices, BObjectImp* target );

    int find(int begin, const char *target);
    unsigned int alnumlen() const;
    void reverse();
    void unary_minus() { reverse(); }

    virtual BObjectImp* array_assign( BObjectImp* idx, BObjectImp* target );
    int find(char *s, int *posn);

    virtual string getStringRep() const { return value_; }
    virtual string getFormattedStringRep() const { return "\"" + value_ + "\""; }
    void printOn(ostream&) const;

  protected:

//    virtual SubString* getsubstr( int begin, int len );
    virtual bool isEqual(const BObjectImp& objimp) const;
    virtual bool isLessThan(const BObjectImp& objimp) const;
  //  virtual void assign(const Object& obj); // only valid if obj is a string

  private:
    string value_;
      // char *strVal;
    String *midstring(int begin, int len) const;
    friend class SubString;
};

class ConstString : public String
{
public:
    explicit ConstString(const string& str) : String(str) { }
//    virtual SubString* getsubstr( int begin, int len );
};

#endif
