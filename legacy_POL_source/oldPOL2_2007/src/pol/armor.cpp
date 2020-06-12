#include "clib/stl_inc.h"

#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include "bscript/bstruct.h"
#include "bscript/impstr.h"

#include "clib/cfgelem.h"
#include "clib/cistring.h"
#include "clib/endian.h"
#include "plib/pkg.h"

#include "armrtmpl.h"
#include "armor.h"
#include "tiles.h"
#include "clidata.h"
#include "itemdesc.h"
#include "layers.h"
#include "ufunc.h"



unsigned short layer_to_zone( unsigned short layer );
unsigned short zone_name_to_zone( const char *zname );

/*
ArmorTemplate::ArmorTemplate() : ar(0)
{
}
*/


ArmorDesc::ArmorDesc( u16 objtype, ConfigElem& elem, const Package* pkg ) :
	EquipDesc( objtype, elem, ARMORDESC, pkg ),
	ar( elem.remove_ushort( "AR" ) ),
	zones(),
	on_hit_script( elem.remove_string( "ONHITSCRIPT", "" ), pkg, "scripts/items/" )
{
	string coverage;
	while ( elem.remove_prop("COVERAGE", &coverage) )
	{
		try
		{
			zones.insert(zone_name_to_zone(coverage.c_str()));
		}
		catch ( runtime_error err )
		{
			cerr << "Error in ObjType 0x" << hex << objtype << ". Package " << pkg->name() <<"." << endl;
			throw err;
		}
	}
	
	if (zones.empty())
	{
		// No 'COVERAGE' entries existed.
		// default coverage based on object type/layer
		unsigned layer = tile[graphic].layer; // tilelayer( graphic );
		// special case for shields - they effectively have no coverage.
		if (layer != LAYER_HAND1 && layer != LAYER_HAND2)
		{
			try
			{
				zones.insert(layer_to_zone(layer));
			}
			catch( runtime_error err )
			{
				cerr << "Error in ObjType 0x" << hex << objtype << ". Package " << pkg->name() <<"." << endl;
				throw err;
			}
		}
	}
}

const char* zone_to_zone_name( unsigned short zone );
void ArmorDesc::PopulateStruct( BStruct* descriptor ) const
{
	base::PopulateStruct( descriptor );
	descriptor->addMember( "OnHitScript", new String(on_hit_script.relativename(pkg)) );
	descriptor->addMember( "AR", new BLong(ar) );

	ObjArray* arr_zones = new ObjArray();
	std::set<unsigned int>::const_iterator itr;
	for(itr = zones.begin(); itr != zones.end(); ++itr)
		arr_zones->addElement( new String( zone_to_zone_name(*itr) ) ); //dave changed 7/10, armordesc expects string zone names


	if(arr_zones->ref_arr.size() > 0) //dave added 7/10, bug with creating shields with CreateItemDescriptor, as shields should have no coverage zones
		descriptor->addMember( "Coverage", arr_zones );
   
}

UArmor::UArmor( const ArmorDesc& descriptor, const ArmorDesc* permanent_descriptor ) :
	Equipment( descriptor, CLASS_ARMOR ),
	tmpl( permanent_descriptor ),
	ar_mod_(0),
	onhitscript_( descriptor.on_hit_script )
{
}

unsigned short UArmor::ar() const
{
	long ar = tmpl->ar * hp_ / maxhp();
	if (ar_mod_ != 0)
	{
		ar += ar_mod_;
	}

	if (ar < 0)
		return 0;
	else if (ar <= USHRT_MAX)
		return static_cast<unsigned short>(ar);
	else
		return USHRT_MAX;
}

bool UArmor::covers( unsigned layer ) const
{
	return tmpl->zones.find( layer ) != tmpl->zones.end();
}

Item* UArmor::clone() const
{
	UArmor* armor = static_cast<UArmor*>(base::clone());
	armor->ar_mod_ = ar_mod_;
	armor->onhitscript_ = onhitscript_;
	armor->tmpl = tmpl;							//dave 12-20
	return armor;
}

void UArmor::printProperties( ostream& os ) const
{
	base::printProperties( os );
	if (ar_mod_)
		os << "\tAR_mod\t" << ar_mod_ << pf_endl;
	if (! (onhitscript_ == tmpl->on_hit_script) )
		os << "\tOnHitScript\t" << onhitscript_.relativename( tmpl->pkg ) << pf_endl;
}

void UArmor::readProperties( ConfigElem& elem )
{
	base::readProperties( elem );
	ar_mod_ = elem.remove_int( "AR_MOD", 0 );
	set_onhitscript( elem.remove_string( "ONHITSCRIPT", "" ) );
}

void UArmor::set_onhitscript( const string& scriptname )
{
	if (scriptname.empty())
	{
		onhitscript_.clear();
	}
	else
	{
		onhitscript_.config( scriptname,
							 itemdesc().pkg,
							 "scripts/items/",
							 true );
	}
}
