// History
//   2005/03/01 Shinigami: added MAX_NUMER_REALMS check to prevent core crash (see MSGBF_SUB18_ENABLE_MAP_DIFFS)

#include "clib/stl_inc.h"
#include "clib/dirlist.h"
#include "clib/passert.h"
#include "clib/strutil.h"
#include "plib/realm.h"
#include "uofile.h"
#include "los.h"
#include "polcfg.h"
#include "realms.h"

Realm* main_realm = NULL;
vector<Realm*>* Realms = new vector<Realm*>();
void load_realms()
{
	//string realm_dir = "realm/";
	Realm* temprealm;
	for( DirList dl( config.realm_data_path.c_str() ); !dl.at_end(); dl.next() )
	{
		string realm_name = dl.name();
		if (realm_name[0] == '.') 
			continue;

		passert_r( Realms->size() < MAX_NUMER_REALMS,
			"You can't use more than " + decint(MAX_NUMER_REALMS) + " realms");

		cout << "Loading Realm " << realm_name << "." << endl;

		temprealm = new Realm( realm_name, config.realm_data_path+realm_name );
		Realms->push_back( temprealm );
		
		//To-Fix - Nasty kludge assuming 'britannia' is the default realm
		//May want to make this configurable in later core releases.
		if( realm_name == string("britannia") )
			main_realm = temprealm;
	}
//	main_realm = new DummyRealm();
}

Realm* find_realm(const string name)
{
	vector<Realm*>::iterator itr;
	for(itr = Realms->begin(); itr != Realms->end(); ++itr)
	{
		if( (*itr)->name() == name )
			return *itr;
	}
	return NULL;
}

#include "plib/mapserver.h"

UMulti* uo_find_supporting_multi( MultiList& mvec, int z );

#if ENABLE_OLD_MAPCODE

class DummyRealm : public Realm
{
public:
	DummyRealm() {}

	virtual bool walkheight( const Character* chr, int x, int y, int oldz,
				int* newz, UMulti** pmulti, Item** pwalkon )
	{
		return ::uo_walkheight( chr, x, y, oldz, newz, pmulti, pwalkon );
	}
	bool walkheight(int x, int y, int oldz,
				int* newz,
				UMulti** pmulti, Item** pwalkon,
				bool doors_block = true,
				MOVEMODE movemode = MOVEMODE_LAND )
	{
		return ::uo_walkheight( x, y, oldz, newz, pmulti, pwalkon, doors_block, movemode );
	}


	bool dropheight( unsigned short x,
					 unsigned short y,
					 int chrz,
					 int* newz,
					 UMulti** pmulti )
	{
		return ::uo_dropheight( x, y, chrz, newz, pmulti );
	}

	bool has_los( const LosObj& att, const LosObj& tgt )
	{
		return ::uo_has_los( att, tgt );
	}
	bool has_los( const UObject& iatt, const UObject& itgt )
	{
		return ::uo_has_los( iatt, itgt );
	}
	bool has_los( const Character& iatt, const UObject& itgt )
	{
		return ::uo_has_los( iatt, itgt );
	}
	bool passable( unsigned short x, unsigned short y )
	{
		return ::uo_passable( x, y );
	}
	UMulti* find_supporting_multi( unsigned short x, unsigned short y, int z )
	{
		return ::uo_find_supporting_multi( x, y, z );
	}
	UMulti* find_supporting_multi( MultiList& mvec, int z )
	{
		return ::uo_find_supporting_multi( mvec, z );
	}
};

#endif

