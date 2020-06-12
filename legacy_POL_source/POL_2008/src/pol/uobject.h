#ifndef __UOBJECT_H
#define __UOBJECT_H

#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include <iosfwd>
#include <map>
#include <string>

#define pf_endl '\n'

#ifndef __UCONST_H
#include "uconst.h"
#endif

#ifndef __CLIB_RAWTYPES_H
#include "clib/rawtypes.h"
#endif

#include "clib/refptr.h"
#include "proplist.h"

class BObjectImp;
class ConfigElem;
class Character;
class Executor;
class Item;
class NPC;
class Realm;
class UMulti;
class UBoat;
class UContainer;
class UArmor;
class UWeapon;
class WornItemsContainer;

// ULWObject: Lightweight object.
// Should contain minimal data structures (and no virtuals)
// Note, not yet needed, so nothing has been moved here.
class ULWObject
{
};

/* NOTES: 
        if you add fields, be sure to update Item::create().
*/
class UObject : protected ref_counted
{
public:
    enum UOBJ_CLASS {       // UOBJ_CLASS is meant to be coarse-grained.
		CLASS_ITEM,         // It's meant as an alternative to dynamic_cast.
		CLASS_CONTAINER,    //
		CLASS_CHARACTER,    // Mostly used to go from UItem to UContainer.
        CLASS_NPC,          //
        CLASS_WEAPON,
        CLASS_ARMOR,
        CLASS_MULTI
	};

    
    virtual std::string name() const;
    virtual std::string description() const;

    bool specific_name() const;
    void setname( const std::string& );

    bool getprop( const std::string& propname, std::string& propvalue ) const;
    void setprop( const std::string& propname, const std::string& propvalue );
    void eraseprop( const std::string& propname );
    void copyprops( const UObject& obj );
    void copyprops( const PropertyList& proplist );
    void getpropnames( std::vector< std::string >& propnames ) const;
	const PropertyList& getprops() const;

    bool orphan() const;

    virtual void destroy();

    virtual u8 los_height() const = 0;
    virtual unsigned long weight() const = 0;


    virtual UObject* toplevel_owner(); // this isn't really right, it returns the WornItemsContainer
    virtual UObject* owner();
    virtual const UObject* owner() const;
    virtual UObject* self_as_owner();
    virtual const UObject* self_as_owner() const;
    virtual const UObject* toplevel_owner() const;
    virtual bool setgraphic( u16 newobjtype );
    virtual bool setcolor( u16 newcolor );
    virtual void on_color_changed();

    virtual void setfacing( u8 newfacing ) = 0;
    virtual void on_facing_changed();
    
    virtual void printOn(std::ostream&) const;
    virtual void printSelfOn( ostream& os ) const;

    virtual void printOnDebug( ostream& os ) const;
    virtual void readProperties( ConfigElem& elem );
    //virtual BObjectImp* script_member( const char *membername );
    virtual BObjectImp* make_ref() = 0;
    virtual BObjectImp* get_script_member( const char *membername ) const;
    virtual BObjectImp* get_script_member_id( const int id ) const; ///id test

    virtual BObjectImp* set_script_member( const char *membername, const std::string& value );
    virtual BObjectImp* set_script_member( const char *membername, long value );
    virtual BObjectImp* set_script_member_double( const char *membername, double value );

    virtual BObjectImp* set_script_member_id( const int id, const std::string& value );
    virtual BObjectImp* set_script_member_id( const int id, long value );
    virtual BObjectImp* set_script_member_id_double( const int id, double value );

    virtual BObjectImp* script_method( const char *methodname, Executor& ex );
    virtual BObjectImp* script_method_id( const int id, Executor& ex );

    virtual BObjectImp* custom_script_method( const char* methodname, Executor& ex );
    virtual bool script_isa( unsigned isatype ) const;
    virtual const char* target_tag() const;

    virtual const char *classname() const = 0;


    bool isa( UOBJ_CLASS uobj_class ) const;
    bool ismobile() const;
    bool isitem() const;
    bool ismulti() const;

    void ref_counted_add_ref();
    void ref_counted_release();
    unsigned ref_counted_count() const;
    ref_counted* as_ref_counted() { return this; }

    inline void increv(){_rev++;};
    inline u32 rev() const {return _rev;};

    bool dirty() const;
    void set_dirty() { dirty_ = true; }
    void clear_dirty() const;
    static unsigned long dirty_writes;
    static unsigned long clean_writes;
protected:

    virtual void printProperties( std::ostream& os ) const;
    virtual void printDebugProperties( std::ostream& os ) const;

    UObject(u16 objtype, UOBJ_CLASS uobj_class);
	virtual ~UObject();

    friend class ref_ptr<UObject>;
    friend class ref_ptr<Character>;
    friend class ref_ptr<Item>;
    friend class ref_ptr<UBoat>;
    friend class ref_ptr<UMulti>;
    friend class ref_ptr<NPC>;
    friend class ref_ptr<UContainer>;
    friend class ref_ptr<UWeapon>;
    friend class ref_ptr<UArmor>;
    friend class ref_ptr<WornItemsContainer>;

    friend class UObjectHelper;


// DATA:
public:
    u32 serial, serial_ext;     
	
    const u16 objtype_;         
    u16 graphic, graphic_ext;   
    u16 color, color_ext;       
	u16 x, y;                   
	s8 z;
    u8 height;

	u8 facing; // not always used for items.
                    // always used for characters
	Realm* realm;
private:
    const u8 uobj_class_;
    mutable bool dirty_;
    u32 _rev;
protected:
    std::string name_;
private:
    PropertyList proplist_;


private: // not implemented:
    UObject( const UObject& );
    UObject& operator=( const UObject& );
};

extern std::ostream& operator << (std::ostream&, const UObject& );

inline bool UObject::specific_name() const
{
    return !name_.empty();
}

inline bool UObject::isa( UOBJ_CLASS uobj_class ) const
{ 
    return uobj_class_ == uobj_class; 
}

inline bool UObject::ismobile() const
{
    return (uobj_class_ == CLASS_CHARACTER ||
            uobj_class_ == CLASS_NPC);
}

inline bool UObject::isitem() const
{
    return !ismobile();
}
inline bool UObject::ismulti() const
{
    return (uobj_class_ == CLASS_MULTI);
}

inline bool UObject::orphan() const
{
    return (serial == 0);
}

inline void UObject::ref_counted_add_ref()
{
    ref_counted::add_ref( REFERER_PARAM(this) );
}

inline void UObject::ref_counted_release()
{
    ref_counted::release( REFERER_PARAM(this) );
}

inline unsigned UObject::ref_counted_count() const
{
    return ref_counted::count();
}

inline bool IsCharacter( u32 serial )
{
	return (~serial & 0x40000000Lu) ? true : false;
}

inline bool IsItem( u32 serial )
{
	return (serial & 0x40000000Lu) ? true : false;
}


#endif
