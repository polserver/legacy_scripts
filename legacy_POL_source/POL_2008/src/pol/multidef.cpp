#include "clib/stl_inc.h"
#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include <algorithm>
#include <iostream>

#include "clib/cfgelem.h"
#include "clib/cfgfile.h"
#include "clib/passert.h"
#include "clib/stlutil.h"
#include "clib/strutil.h"

#include "plib/mapcell.h"
#include "plib/mapshape.h"

#include "itemdesc.h"
#include "objtype.h"
#include "udatfile.h"
#include "uofilei.h"
#include "uofile.h"
#include "multidef.h"
#include "ustruct.h"
#include "uofilei.h"
//#include "uobject.h"

bool BoatShapeExists( u16 graphic );

MultiDef::MultiDef( ConfigElem& elem, u16 multiid ) :
    multiid(multiid),
    type(UNKNOWN),
    graphic( elem.remove_ushort( "Graphic" ) ),
    elems(),

    xbase(0),
    xsize(0),
    ybase(0),
    ysize(0),

    minrx(0),
    minry(0),
    minrz(0),
    maxrx(0),
    maxry(0),
    maxrz(0)
{
    if (elem.type_is("BOAT"))
    {
        if (!BoatShapeExists(graphic))
            elem.throw_error( "Entry for Boat " 
                                + hexint(multiid) 
                                + " (graphic " + hexint(graphic)
                                + ") not found in boats.cfg" );
        type = BOAT;
    }
    else if (elem.type_is("HOUSE"))
    {
        type = HOUSE;
    }
    else if (elem.type_is("STAIRS"))
    {
        type = STAIRS;
    }

    string tmp;
    while (elem.remove_prop( "static", &tmp ))
    {
        istringstream is( tmp );
        MULTI_ELEM multielem;
        multielem.is_static = true;
        if (is >> hex >> multielem.objtype >> dec >> multielem.x >> multielem.y >> multielem.z)
        {
            elems.push_back( multielem );
        }
        else
        {
            string message = "Badly formed static directive: '" + tmp + "'";
            elem.throw_error( message );
        }
    }
    while (elem.remove_prop( "dynamic", &tmp ))
    {
        istringstream is( tmp );
        MULTI_ELEM multielem;
        multielem.is_static = false;
        if (is >> hex >> multielem.objtype >> dec >> multielem.x >> multielem.y >> multielem.z)
        {
            elems.push_back( multielem );
        }
        else
        {
            string message = "Badly formed dynamic directive: '" + tmp + "'";
            elem.throw_error( message );
        }
    }
}

MultiDef::~MultiDef()
{
}

inline unsigned short MultiDef::getkey( int rx, int ry ) const
{
    unsigned char crx = static_cast<unsigned char>(rx);
    unsigned char cry = static_cast<unsigned char>(ry);
    
    unsigned short key = (crx << 8) | cry;
    return key;
}

/*
void MultiDef::decode_hullpt( unsigned short hullpt, int& rx, int& ry )
{
    signed char sx = hullpt >> 8;
    signed char sy = hullpt & 0xff;
    rx = sx;
    ry = sy;
}
*/

bool MultiDef::findcomponents( Components::const_iterator& beg, Components::const_iterator& end,
                            int rx, int ry ) const
{

    ItrPair pr = components.equal_range( getkey(rx,ry) );
    if (pr.first == components.end())
    {
        return false;
    }
    else
    {
        beg = pr.first;
        end = pr.second;
        return true;
    }
}

bool MultiDef::body_contains( int rx, int ry ) const
{
    return (components.count( getkey(rx,ry) ) != 0);
}
const MULTI_ELEM* MultiDef::find_component( int rx, int ry ) const
{
    if (body_contains( rx,ry ))
        return (*components.find( getkey(rx,ry) )).second;
    else
        return NULL;
}

void MultiDef::add_to_hull( const MULTI_ELEM* elem )
{
    unsigned short k = getkey( elem->x, elem->y );

    if (!hull2.count(k))
    {
        hull.push_back( elem );
        hull2.insert(k);
    }

    
    if (type == BOAT)
    {
        int int_rx = elem->x, int_ry = elem->y;
        if ((multiid & 1) == 0) // N/S hull, so squeeze X
        {
            if (elem->x == minrx)
                int_rx = minrx+1;
            else if (elem->x == maxrx)
                int_rx = maxrx-1;
        }
        else
        {
            if (elem->y == minry)
                int_ry = minry+1;
            else if (elem->y > 0)
                int_ry = maxry-1;
        }
        elem = find_component( int_rx, int_ry );
        if (elem)
            add_to_internal_hull( elem );
    }

}

void MultiDef::add_to_internal_hull( const MULTI_ELEM* elem )
{
    unsigned short k = getkey( elem->x, elem->y );

    if (!internal_hull2.count(k))
    {
        internal_hull.push_back( elem );
        internal_hull2.insert(k);
    }
}

void MultiDef::add_body_tohull()
{
    int rx, ry;
    for( ry = minry; ry <= maxry; ++ry )
    {
        for( rx = minrx; rx <= maxrx; ++rx )
        {
            const MULTI_ELEM* elem = find_component( rx, ry );
            if (elem != NULL)
            {
                add_to_hull(elem);
               
                break;
            }
        }
        for( rx = maxrx; rx >= minrx; --rx )
        {
            const MULTI_ELEM* elem = find_component( rx, ry );
            if (elem != NULL)
            {
                add_to_hull(elem);

                break;
            }
        }
    }


    for( rx = minrx; rx <= maxrx; ++rx )
    {
        for( ry = minry; ry <= maxry; ++ry )
        {
            const MULTI_ELEM* elem = find_component( rx, ry );
            if (elem != NULL)
            {
                add_to_hull(elem);
                break;
            }
        }
        for( ry = maxry; ry >= minry; --ry )
        {
            const MULTI_ELEM* elem = find_component( rx, ry );
            if (elem != NULL)
            {
                add_to_hull(elem);
                break;
            }
        }
    }
}


void MultiDef::computehull()
{
    add_body_tohull();
}

void MultiDef::addrec( const MULTI_ELEM* elem )
{
    if (elem->x < minrx) minrx = elem->x;
    if (elem->y < minry) minry = elem->y;
    if (elem->z < minrz) minrz = elem->z;

    if (elem->x > maxrx) maxrx = elem->x;
    if (elem->y > maxry) maxry = elem->y;
    if (elem->z > maxrz) maxrz = elem->z;

    if (elem->x < global_minrx) global_minrx = elem->x;
    if (elem->y < global_minry) global_minry = elem->y;
    if (elem->z < global_minrz) global_minrz = elem->z;

    if (elem->x > global_maxrx) global_maxrx = elem->x;
    if (elem->y > global_maxry) global_maxry = elem->y;
    if (elem->z > global_maxrz) global_maxrz = elem->z;

    components.insert( Components::value_type( getkey(elem->x,elem->y), elem ) );
}

void MultiDef::init()
{
    for( unsigned i = 0; i < elems.size(); i++ )
    {
        addrec( &elems[i] );
    }
    computehull();
}


static MultiDefs my_multidefs_by_graphic;
MultiDefs multidefs_by_multiid;
ObjtypeByMultiID objtype_by_multiid;

int MultiDef::global_minrx;
int MultiDef::global_minry;
int MultiDef::global_minrz;
int MultiDef::global_maxrx;
int MultiDef::global_maxry;
int MultiDef::global_maxrz;

bool MultiDefByGraphicExists( u16 graphic )
{
    return my_multidefs_by_graphic.count( graphic ) != 0;
}
const MultiDef* MultiDefByGraphic( u16 graphic )
{
    passert( my_multidefs_by_graphic.count( graphic ) != 0 );

    MultiDefs::const_iterator citr = my_multidefs_by_graphic.find(graphic);
    if (citr != my_multidefs_by_graphic.end())
    {
        return (*citr).second;
    }
    else
    {
        return NULL;
    }
}

void read_multidefs()
{
    ConfigFile cf( "config/multis.cfg", "BOAT HOUSE STAIRS" );
    ConfigElem elem;
    while (cf.read( elem ))
    {
        u16 multiid = static_cast<u16>(strtoul( elem.rest(), NULL, 0 ));

        MultiDef* mdef = NULL;
        mdef = new MultiDef( elem, multiid ); 
        mdef->init();

        my_multidefs_by_graphic[ mdef->graphic ] = mdef;
        multidefs_by_multiid[ mdef->multiid ] = mdef;
    }
}

/*
typedef map<unsigned short, MultiDef*> MultiDefs;
MultiDefs multidefs;
*/

// statics of multi at wx, wy
/*
void readstatics( StaticList& slist, const UObject& multi, unsigned short wx, unsigned short wy )
{
    int rx, ry;
    rx = static_cast<int>(wx) - static_cast<int>(multi.x);
    ry = static_cast<int>(wy) - static_cast<int>(multi.y);

    MultiDefs::const_iterator itr = multidefs.find( multi.objtype );
    if (itr != multidefs.end())
    {
        itr->second->readstatics( slist, rx, ry );
    }
}
*/
