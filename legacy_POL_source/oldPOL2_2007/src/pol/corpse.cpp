#include "clib/stl_inc.h"
#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include "clib/cfgelem.h"
#include "clib/endian.h"

#include "containr.h"
#include "layers.h"
#include "ufunc.h"

UCorpse::UCorpse( const ContainerDesc& descriptor ) :
    UContainer( descriptor ),
    corpsetype(0),
    take_contents_to_grave(false)
{
    movable_ = false;
}

u16 UCorpse::get_senditem_amount() const
{
    return corpsetype;
}

void UCorpse::spill_contents( UMulti* multi )
{
    // destroy hair and beards
    bool any;
    do {
        any = false;
        for( iterator itr = begin(); itr != end(); ++itr )
        {
            Item *item = GET_ITEM_PTR( itr );
            if (item->layer == LAYER_HAIR || item->layer == LAYER_BEARD)
            {
                ::destroy_item( item );
                any = true;
                break; // our iterators are now useless, so start over
            }
        }
    } while (any);

    if (!take_contents_to_grave)
        base::spill_contents( multi );
}


void UCorpse::printProperties( std::ostream& os ) const
{
    base::printProperties( os );
    os << "\tCorpseType\t" << corpsetype << pf_endl;
	os << "\tOwnerSerial\t" << ownerserial << pf_endl;
    os << "\tTakeContentsToGrave\t" << take_contents_to_grave << pf_endl;
}

void UCorpse::readProperties( ConfigElem& elem )
{
        // corpses can be the same color as mobiles
    u16 savecolor = elem.remove_ushort( "COLOR", 0 );
    
    base::readProperties( elem );

    color = savecolor;
    color_ext = ctBEu16( savecolor );

    elem.remove_prop( "CorpseType", &corpsetype );
	elem.remove_prop("OwnerSerial", &ownerserial);
    take_contents_to_grave = elem.remove_bool( "TakeContentsToGrave", false );
    movable_ = false;
}


