#include "clib/stl_inc.h"

#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include <limits.h>


#include "clib/cfgfile.h"
#include "clib/logfile.h"
#include "clib/passert.h"
#include "clib/random.h"
#include "clib/stlutil.h"

#include "client.h"
#include "charactr.h"
#include "core.h"
#include "gflag.h"
#include "itemdesc.h"
#include "multi.h"
#include "profile.h"
#include "ucfg.h"
#include "ufunc.h"
#include "umanip.h"
#include "uvars.h"
#include "uworld.h"

#include "containr.h"


UContainer::UContainer( const ContainerDesc& descriptor ) :
	ULockable( descriptor, CLASS_CONTAINER ),
    desc( find_container_desc( objtype_ ) ), // NOTE still grabs the permanent descriptor.
    held_weight_(0),
    held_item_count_(0)
{
}

UContainer::~UContainer()
{
    passert_always( contents_.empty() );
}

void UContainer::destroy_contents()
{
    while (!contents_.empty())
    {
        Contents::value_type item = contents_.back();
        if (ITEM_ELEM_PTR( item ) != NULL)  // this is really only for wornitems.
        {
            item->container = NULL;
            item->destroy();
        }
        contents_.pop_back();
    }
}
void UContainer::destroy()
{
    destroy_contents();
    base::destroy();
}
// Consider: writing an "item count" property.  On read,
// recursively read items (eliminate a lot of searching)
void UContainer::printOn( ostream& os ) const
{
    base::printOn( os );
    printContents( os );
}

void UContainer::printSelfOn( ostream& os ) const
{
    base::printOn( os );
}

void UContainer::printContents( ostream& os ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
    {
        const Item *item = GET_ITEM_PTR( itr );
        if (item != NULL) 
        {
            if (!dont_save_itemtype[item->objtype_])
            {
                os << *item;
                item->clear_dirty();
            }
        }
    }
}

bool UContainer::can_add_bulk( long tli_diff, long item_count_diff, long weight_diff ) const
{
    if (gflag_enforce_container_limits)
    {
        if (contents_.size()+tli_diff >= MAX_CONTAINER_ITEMS)
            return false;

        if (weight() + weight_diff > USHRT_MAX /* fucking gcc...std::numeric_limits<unsigned short>::max()*/)
            return false;

        if (desc.max_weight && (held_weight_ + weight_diff > desc.max_weight))
            return false;

        if (desc.max_items && (held_item_count_ + item_count_diff > desc.max_items))
            return false;

        if (container != NULL)
            return container->can_add_bulk( 0, item_count_diff, weight_diff );
        else
            return true;
    }
    else
    {
        return (contents_.size() < MAX_CONTAINER_ITEMS);
    }
}

bool UContainer::can_add( const Item& item ) const
{
    return can_add_bulk( 1, item.item_count(), item.weight() );
}

bool UContainer::can_add( unsigned short more_weight ) const
{
    return can_add_bulk( 0, 0, more_weight );
}

void UContainer::add( Item *item )
{
	// passert( can_add( *item ) );
	
    INC_PROFILEVAR( container_adds );
	item->realm = realm;
	item->container = this;
    item->set_dirty();
    contents_.push_back( Contents::value_type(item) );
    
    add_bulk( item );
}
void UContainer::add_bulk( const Item* item )
{
    add_bulk( item->item_count(), item->weight() );
}
void UContainer::remove_bulk( const Item* item )
{
    add_bulk( -static_cast<long>(item->item_count()), -static_cast<long>(item->weight()) );
}

void UContainer::add_bulk( long item_count_delta, long weight_delta )
{
    held_item_count_ += item_count_delta;
    
    // passert( !gflag_enforce_container_limits || (held_weight_ + weight_delta <= MAX_WEIGHT) );
    
    held_weight_ += static_cast<unsigned short>(weight_delta);
    // cout << "Adding " << delta << " stones to container " << serial << endl;
    if (container != NULL)
    {
        container->add_bulk( item_count_delta, weight_delta );
    }
}


unsigned long UContainer::weight() const
{
    return Item::weight() + held_weight_;
}

unsigned long UContainer::item_count() const
{
    return Item::item_count() + held_item_count_;
}

void UContainer::add_at_random_location( Item* item )
{
    u16 rx, ry;
    get_random_location( &rx, &ry );
	
	item->x = rx;
	item->y = ry;
    item->z = 0;

    add( item );
}

void UContainer::extract( Contents& cnt )
{
    contents_.swap( cnt );
    add_bulk( -static_cast<long>(held_item_count_), -static_cast<long>(held_weight_) );
}

bool UContainer::can_swap( const UContainer& cont ) const
{
    long weight_diff0 = cont.weight() - weight();
    long item_count_diff0 = cont.item_count() - item_count();
    
    long weight_diff1 = -weight_diff0;
    long item_count_diff1 = -item_count_diff0;

/*
    if (weight_diff0 < 0)
        weight_diff0 = 0;
    if (item_count_diff0 < 0)
        item_count_diff0 = 0;

    if (weight_diff1 < 0)
        weight_diff1 = 0;
    if (item_count_diff1 < 0)
        item_count_diff1 = 0;
*/
    
    return (can_add_bulk( 0, item_count_diff0, weight_diff0 ) &&
            cont.can_add_bulk( 0, item_count_diff1, weight_diff1 ));
}

void UContainer::swap( UContainer& cont )
{
    assert( can_swap(cont) );

    long weight_diff = cont.weight() - weight();
    long item_count_diff = cont.item_count() - item_count();

    add_bulk( item_count_diff, weight_diff );
    cont.add_bulk( -item_count_diff, -weight_diff );

    contents_.swap( cont.contents_ );
}

Item* UContainer::find_toplevel_polclass( unsigned int polclass ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item *item = GET_ITEM_PTR( itr );
		if (item && (item->script_isa(polclass)))
			return item;
	}
	return NULL;
}

Item* UContainer::find_toplevel_objtype( u16 objtype ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item *item = GET_ITEM_PTR( itr );
		if (item && (item->objtype_ == objtype))
			return item;
	}
	return NULL;
}
Item* UContainer::find_toplevel_objtype_noninuse( u16 objtype ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item *item = GET_ITEM_PTR( itr );
		if (item && (item->objtype_ == objtype) && !item->inuse())
			return item;
	}
	return NULL;
}

Item* UContainer::find_toplevel_objtype( u16 objtype, unsigned short maxamount ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item *item = GET_ITEM_PTR( itr );
		if (item && (item->objtype_ == objtype) && (item->getamount() <= maxamount))
			return item;
	}
	return NULL;
}
Item* UContainer::find_toplevel_objtype_noninuse( u16 objtype, unsigned short maxamount ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item *item = GET_ITEM_PTR( itr );
		if (item && (item->objtype_ == objtype) && (item->getamount() <= maxamount) && !item->inuse())
			return item;
	}
	return NULL;
}

Item* UContainer::find_addable_stack( const Item* adding_item ) const
{
    long maxamount = MAX_STACK_ITEMS - adding_item->getamount();
    if (maxamount > 0)
    {
        for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	    {
		    Item *item = GET_ITEM_PTR( itr );
		    if (item &&
                !item->inuse() &&
                item->objtype_ == adding_item->objtype_ && 
                item->getamount() <= maxamount &&
                item->newbie() == adding_item->newbie() &&
                item->color == adding_item->color)
            {
			    return item;
            }
	    }
    }

	return NULL;
}


Item* UContainer::find_objtype_noninuse( u16 objtype ) const
{
    Item* item = find_toplevel_objtype_noninuse( objtype );
    if (item != NULL)
        return item;

    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
    {
        Item* item = GET_ITEM_PTR( itr );

        if (item &&
            item->isa( UObject::CLASS_CONTAINER ) && 
            !item->inuse())
        {
            UContainer* cont = static_cast<UContainer*>(item);
            if (!cont->locked_)
            {
                item = cont->find_objtype_noninuse( objtype );
                if (item != NULL)
                    return item;
            }
        }
    }
    return NULL;
}

unsigned long UContainer::find_sumof_objtype_noninuse( u16 objtype ) const
{
    unsigned long amt = 0;

    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
    {
        Item* item = GET_ITEM_PTR( itr );

        if (item && 
            !item->inuse())
        {
            if (item->objtype_ == objtype)
                amt += item->getamount();

            if (item->isa( UObject::CLASS_CONTAINER ))
            {
                UContainer* cont = static_cast<UContainer*>(item);
                if (!cont->locked_)
                {
                    amt += cont->find_sumof_objtype_noninuse( objtype );
                }
            }
        }
    }
    return amt;
}

void UContainer::consume_sumof_objtype_noninuse( u16 objtype, unsigned long amount )
{
    while (amount != 0)
    {
        Item* item = find_objtype_noninuse( objtype );
        passert_always( item != NULL );

        unsigned short thisamt = item->getamount();
        if (thisamt > amount)
            thisamt = static_cast<unsigned short>(amount);
        subtract_amount_from_item( item, thisamt );
        amount -= thisamt;
    }
}

Item *UContainer::remove( u32 serial, UContainer** found_in )
{
	Item* item;
	iterator itr;

	item = find( serial, itr );
	if (item != NULL)
	{
		if (found_in != NULL)
			*found_in = item->container;
		
        item->container->remove( itr );
	}
	return item;
}

void UContainer::remove( Item* item )
{
    if (item->container != this)
    {
        Log( "UContainer::remove(Item*), serial=0x%lx, item=0x%lx, item->cont=0x%lx\n",
                      serial, item->serial, item->container->serial );
	    passert_always( item->container == this );
         int* p = 0;
        *p = 6;
    }
    
    iterator itr = find_in( contents_, item );
    passert_always( itr != contents_.end() );

	//DAVE added this 11/17. refresh owner's weight on delete
	Character* chr_owner = item->GetCharacterOwner();

    remove( itr );

	if(chr_owner != NULL)
	{
		chr_owner->refresh_ar();
	}
}

void UContainer::remove( iterator itr )
{
    INC_PROFILEVAR( container_removes );
    Item* item = GET_ITEM_PTR( itr );
    contents_.erase( itr );
    item->container = NULL;
    item->set_dirty();
    remove_bulk( item );
}

// FIXME this is depth-first.  Not sure I like that.
UContainer *UContainer::find_container( u32 serial ) const
{
	for( const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item *item = GET_ITEM_PTR( itr );
		if (item &&
            item->isa(UObject::CLASS_CONTAINER))
        {
		    UContainer *cont = static_cast<UContainer *>(item);
		    if (cont->serial == serial)
			    return cont;
		    cont = cont->find_container( serial );
		    if (cont != NULL) 
			    return cont;
        }
	}
	return NULL;
}

Item *UContainer::find( u32 serial, iterator& where_in_container )
{
	for( iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item *item = GET_ITEM_PTR( itr );
        passert( item != NULL );
		if (item != NULL)
        {
            if (item->serial == serial)
		    {
			    where_in_container = itr;
			    return item;
		    }
		    if (item->isa(UObject::CLASS_CONTAINER))
		    {
			    UContainer* cont = static_cast<UContainer*>(item);
                if (!cont->locked_)
                {
                    item = cont->find( serial, where_in_container );
			        if (item != NULL) 
				        return item;
                }
		    }
        }
	}
	return NULL;
}

Item *UContainer::find( u32 serial ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item* item = GET_ITEM_PTR( itr );
        passert( item != NULL );
        if (item != NULL)
        {
		    if (item->serial == serial)
			    return item;

		    if (item->isa(UObject::CLASS_CONTAINER))
		    {
                UContainer* cont = static_cast<UContainer*>(item);
			    if (!cont->locked_)
                {
                    item = cont->find( serial );
			        if (item != NULL) 
				        return item;
                }
		    }
        }
	}
	return NULL;
}

Item *UContainer::find_toplevel( u32 serial ) const
{
    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
	{
		Item* item = GET_ITEM_PTR( itr );
        passert( item != NULL );
        if (item != NULL)
        {
		    if (item->serial == serial)
			    return item;

        }
	}
	return NULL;
}

void UContainer::for_each_item( void (*f)(Item* item, void* a), void* arg )
{
	for( UContainer::iterator itr = begin(); itr != end(); ++itr )
	{
		Item* item = GET_ITEM_PTR( itr );
		
		if (item->isa( UObject::CLASS_CONTAINER ))
		{
			UContainer* cont = static_cast<UContainer*>(item);
			cont->for_each_item( f, arg );
		}
		(*f)(item,arg);
	}
}

UContainer::const_iterator UContainer::begin() const
{
    return contents_.begin();
}
UContainer::const_iterator UContainer::end() const
{
    return contents_.end();
}

UContainer::iterator UContainer::begin()
{
    return contents_.begin();
}
UContainer::iterator UContainer::end()
{
    return contents_.end();
}

#include <stddef.h>
#include "clib/endian.h"
#include "objtype.h"
#include "umsgtype.h"
#include "ufunc.h"
#include "sockio.h"



/*
void send_corpse_contents( Client *client, const UContainer& cont )
{
	static MSG3C_CONTAINER_CONTENTS msg;

    int nfound;
	msg.msgtype = MSGOUT_3C_CONTAINER_CONTENTS;
	unsigned short msglen = offsetof( MSG3C_CONTAINER_CONTENTS, items ) + 
		                    cont.count() * sizeof msg.items[0];
	msg.msglen = ctBEu16( msglen );
	msg.count = ctBEu16( cont.count() );
	
	UContainerIterator itr(cont);
    nfound = 0;
	for( unsigned idx = 0; itr.exists(); itr.advance(), ++idx )
	{
		const Item *item = itr.item();
        if (item->layer == 0)
            continue;
		msg.items[nfound].serial = item->serial_ext;
		msg.items[idx].objtype = item->objtype_ext;
		msg.items[idx].unk6_00 = 0x00;

		
        msg.items[idx].amount = ctBEu16( item->get_senditem_amount() );

        msg.items[idx].x = ctBEu16( item->x );
		msg.items[idx].y = ctBEu16( item->y );
		msg.items[nfound].container_serial = cont.serial_ext;
		msg.items[nfound].color = item->color_ext;
        ++nfound;
	}

	transmit( client, &msg, msglen );

}
*/

void UContainer::builtin_on_use( Client *client )
{
	client->pause();
	
	if (!locked_)
    {
        send_open_gump( client, *this );
	    send_container_contents( client, *this );
    }
    else
    {
        send_sysmessage( client, "That is locked." );
    }
	
	client->restart();
}

u16 UContainer::gump() const
{
    return desc.gump;
}

void UContainer::get_random_location( u16* px, u16* py ) const
{
    if (desc.minx < desc.maxx)
    {
        *px = desc.minx + random_int( desc.maxx - desc.minx );
    }
    else
    {
        *px = desc.minx;
    }

    if (desc.miny < desc.maxy)
    {
        *py = desc.miny + random_int( desc.maxy - desc.miny );
    }
    else
    {
        *py = desc.miny;
    }
}

bool UContainer::is_legal_posn( const Item* item, u16 x, u16 y ) const
{
    return (x >= desc.minx && x <= desc.maxx &&
            y >= desc.miny && y <= desc.maxy);
}

void UContainer::spill_contents( UMulti* multi )
{
    passert( container == NULL );
    if (!locked_)
    {
        while (!contents_.empty())
        {
            Item* item = ITEM_ELEM_PTR( contents_.back() );
            if (item->movable())
            {
                contents_.pop_back();
                item->set_dirty();
                item->x = x;
                item->y = y;
                item->z = z;
                item->container = NULL;
                add_item_to_world( item );
                move_item( item, x, y, z, NULL );
                if (multi)
                    multi->register_object( item );
            }
            else
            {
                destroy_item( item );
            }
        }
    }
}

#include "scrsched.h"
#include "uoscrobj.h"
void UContainer::on_remove( Character* chr, Item* item, MoveType move )
{
    if (!desc.on_remove_script.empty())
    {
        
        BObjectImp* chrParam = NULL;
        if (chr)// consider: move this into make_mobileref
            chrParam = new ECharacterRefObjImp(chr);
        else
            chrParam = UninitObject::create();
        call_script( desc.on_remove_script,
                       chrParam,
                       new EItemRefObjImp(this),
                       new EItemRefObjImp(item),
					   new BLong(move) );
    }
}

bool UContainer::can_insert_increase_stack( Character* mob, 
                                            MoveType movetype, 
                                            Item* existing_item, 
                                            unsigned short amt_to_add,
                                            Item* adding_item )
{
    if (!desc.can_insert_script.empty())
    {
		return call_script( desc.can_insert_script,
                            mob ? mob->make_ref() : UninitObject::create(),
                            make_ref(),
                            new BLong( movetype ),
                            new BLong( INSERT_INCREASE_STACK ),
                            adding_item ? adding_item->make_ref() : UninitObject::create(),
                            existing_item->make_ref(),
                            new BLong( amt_to_add ) );
    }
    else
    {
        return true;
    }
}

void UContainer::on_insert_increase_stack( Character* mob, 
                                           MoveType movetype, 
                                           Item* existing_item, 
                                           unsigned short amt_to_add )
{
    if (!desc.on_insert_script.empty())
    {
		call_script( desc.on_insert_script,
                     mob ? mob->make_ref() : UninitObject::create(),
                     make_ref(),
                     new BLong( movetype ),
                     new BLong( INSERT_INCREASE_STACK ),
                     UninitObject::create(),
                     existing_item->make_ref(),
                     new BLong( amt_to_add ) );
    }
}

bool UContainer::can_insert_add_item( Character* mob, MoveType movetype, Item* new_item )
{
    if (!desc.can_insert_script.empty())
    {
		return call_script( desc.can_insert_script,
                            mob ? mob->make_ref() : UninitObject::create(),
                            make_ref(),
                            new BLong( movetype ),
                            new BLong( INSERT_ADD_ITEM ),
                            new_item->make_ref() );
    }
    else
    {
        return true;
    }
}

void UContainer::on_insert_add_item( Character* mob, MoveType movetype, Item* new_item )
{
    if (!desc.on_insert_script.empty())
    {
		call_script( desc.on_insert_script,
                     mob ? mob->make_ref() : UninitObject::create(),
                     make_ref(),
                     new BLong( movetype ),
                     new BLong( INSERT_ADD_ITEM ),
                     new_item->make_ref() );
    }
}

bool UContainer::check_can_remove_script( Character* chr, Item* item, MoveType move )
{
    if (!desc.can_remove_script.empty())
    {
        BObjectImp* chrParam = NULL;
        if (chr != NULL) // TODO: consider moving this into make_mobileref
            chrParam = chr->make_ref();
        else
            chrParam = UninitObject::create();
        return call_script( desc.can_remove_script,
                                  chrParam,
                                  make_ref(),
                                  item->make_ref(),
								  new BLong(move) );
    }
    else
    {
        return true;
    }
}

void UContainer::readProperties( ConfigElem& elem )
{
    base::readProperties( elem );
}

unsigned long UContainer::find_sumof_objtype_noninuse( u16 objtype, u32 amtToGet, Contents& saveItemsTo ) const
{
    unsigned long amt = 0;

    for( Contents::const_iterator itr = contents_.begin(); itr != contents_.end(); ++itr )
    {
        Item* item = GET_ITEM_PTR( itr );

        if (item && 
            !item->inuse())
        {
            if (item->objtype_ == objtype)
			{
				saveItemsTo.push_back(item);
                amt += item->getamount();
			}

            if (item->isa( UObject::CLASS_CONTAINER ))
            {
                UContainer* cont = static_cast<UContainer*>(item);
                if (!cont->locked_)
                {
                    amt += cont->find_sumof_objtype_noninuse( objtype, amtToGet - amt, saveItemsTo );
                }
            }
        }
		if (amt >= amtToGet)
			return amt;
    }
    return amt;
}



WornItemsContainer::WornItemsContainer() : 
    UContainer( find_container_desc(EXTOBJ_WORNITEMS_CONTAINER)), 
    chr_owner(NULL) 
{
    contents_.resize( HIGHEST_LAYER + 1, EMPTY_ELEM );
}
//WornItemsContainer::WornItemsContainer(u16 objtype) : 
//    UContainer(objtype), 
//    chr_owner(NULL)
//{
//}

void WornItemsContainer::SetInitialSize()
{
}

void WornItemsContainer::for_each_item( void (*f)(Item* item, void* a), void* arg )
{
	for( UContainer::iterator itr = begin(); itr != end(); ++itr )
	{
		Item* item = GET_ITEM_PTR( itr );

		if (item != NULL)
		{
			if (item->isa( UObject::CLASS_CONTAINER ))
			{
				UContainer* cont = static_cast<UContainer*>(item);
				cont->for_each_item( f, arg );
			}
			(*f)(item, arg);
		}
	}
}

void WornItemsContainer::PutItemOnLayer( Item* item )
{
    item->set_dirty();
	item->container = this;
	item->realm = realm;
    contents_[ item->layer ] = Contents::value_type( item );
    add_bulk( item );
}

void WornItemsContainer::RemoveItemFromLayer( Item* item )
{
    item->set_dirty();
	item->container = NULL;
	contents_[ item->layer ] = EMPTY_ELEM;
    remove_bulk( item );
}

void WornItemsContainer::print( ostream& ofs_pc, ostream& ofs_equip ) const
{
    // FIXME doesn't do anything with gotten_item.
    for( unsigned layer = 0; layer < contents_.size(); ++layer )
    {
        const Item* item = contents_[ layer ];
        if (item)
        {
            if (dont_save_itemtype[item->objtype_])
                continue;

            if ((layer == LAYER_HAIR) || 
                (layer == LAYER_BEARD) ||
                (layer == LAYER_ROBE_DRESS && item->objtype_ == UOBJ_DEATH_SHROUD))
            {
                ofs_pc << *item;
                item->clear_dirty();
            }
            else if (layer == LAYER_BACKPACK)
            {
                    // write the backpack to the PC file,
                    // and the backpack contents to the PCEQUIP file
                const UContainer* cont = static_cast<const UContainer*>(item);
                cont->UContainer::base::printOn( ofs_pc );
                cont->clear_dirty();
                cont->printContents( ofs_equip) ;
            }
            else
            {
                ofs_equip << *item;
                item->clear_dirty();
            }
        }
    }
}

BObjectImp* WornItemsContainer::make_ref()
{
    passert_always( chr_owner != NULL );
    return chr_owner->make_offline_ref();
}

UObject* WornItemsContainer::owner()
{
    return chr_owner;
}

const UObject* WornItemsContainer::owner() const
{
    return chr_owner;
}

UObject* WornItemsContainer::self_as_owner()
{
    return chr_owner;
}

const UObject* WornItemsContainer::self_as_owner() const
{
    return chr_owner;
}
