#ifndef __TARGET_H
#define __TARGET_H

#include "clib/rawtypes.h"

#include "umsgtype.h"

class Character;
class Client;
class UObject;

// The TargetCursor self-registers with the "MSG6C_TARGET_CURSOR" handler. 
 
class TargetCursor
{
public:
	explicit TargetCursor( bool inform_on_cancel );
	virtual ~TargetCursor() {};

    bool send_object_cursor( Client* client,
        MSG6C_TARGET_CURSOR::CURSOR_TYPE crstype = MSG6C_TARGET_CURSOR::CURSOR_TYPE_NEUTRAL );
    
    void cancel(Character* chr);


    void handle_target_cursor( Character* targetter, MSG6C_TARGET_CURSOR* msg );
    virtual void on_target_cursor( Character* targetter, MSG6C_TARGET_CURSOR* msg ) = 0;
	u32 cursorid_;
	
protected:
    bool inform_on_cancel_;
};

class FullMsgTargetCursor : public TargetCursor
{
public:
    FullMsgTargetCursor( void (*func)(Character*, MSG6C_TARGET_CURSOR*) );
	virtual ~FullMsgTargetCursor() {};

    virtual void on_target_cursor( Character* targetter, MSG6C_TARGET_CURSOR* msg );
private:
    void (*func)(Character* targetter, MSG6C_TARGET_CURSOR* msg);
};



/******************************************************/
class LosCheckedTargetCursor : public TargetCursor
{
public:
    LosCheckedTargetCursor( void (*func)(Character*, UObject*),
                            bool inform_on_cancel = false );
	virtual ~LosCheckedTargetCursor() {};

    virtual void on_target_cursor( Character*, MSG6C_TARGET_CURSOR* msg );
private:
    void (*func)(Character*, UObject* targetted);
};
/******************************************************/



/******************************************************/
class NoLosCheckedTargetCursor : public TargetCursor
{
public:
    NoLosCheckedTargetCursor( void (*func)(Character*, UObject*),
                            bool inform_on_cancel = false );
	virtual ~NoLosCheckedTargetCursor() {};

    virtual void on_target_cursor( Character*, MSG6C_TARGET_CURSOR* msg );
private:
    void (*func)(Character*, UObject* targetted);
};
/******************************************************/



/******************************************************/
class LosCheckedCoordCursor : public TargetCursor
{
public:
    LosCheckedCoordCursor( void (*func)(Character*,MSG6C_TARGET_CURSOR* msg),
                           bool inform_on_cancel = false );
	virtual ~LosCheckedCoordCursor() {};

    bool send_coord_cursor( Client* client ); 
    virtual void on_target_cursor( Character*, MSG6C_TARGET_CURSOR* msg );
private:
    void (*func_)(Character*,MSG6C_TARGET_CURSOR*);
};
/*******************************************************/



/******************************************************/
class MultiPlacementCursor : public TargetCursor
{
public:
    MultiPlacementCursor( void (*func)(Character*,MSG6C_TARGET_CURSOR* msg) );
	virtual ~MultiPlacementCursor() {};

    virtual void send_placemulti( Client* client, unsigned short objtype, long flags, s16 xoffset, s16 yoffset ); 
    virtual void on_target_cursor( Character*, MSG6C_TARGET_CURSOR* msg );
private:
    void (*func_)(Character*,MSG6C_TARGET_CURSOR*);
};
/*******************************************************/




/******************************************************/
/* NoLosCharacterCursor - not used anymore            */
class NoLosCharacterCursor : public TargetCursor
{
public:
    NoLosCharacterCursor( void (*func)(Character* targetter, Character* targetted) );
	virtual ~NoLosCharacterCursor() {};
    virtual void on_target_cursor( Character*, MSG6C_TARGET_CURSOR* msg );
private:
    void (*func)(Character* targetter, Character* targetted);
};
/******************************************************/




/******************************************************/
class NoLosUObjectCursor : public TargetCursor
{
public:
    NoLosUObjectCursor( void (*func)(Character*, UObject*),
                        bool inform_on_cancel = false );
	virtual ~NoLosUObjectCursor() {};
    virtual void on_target_cursor( Character* chr, MSG6C_TARGET_CURSOR* msg );
private:
    void (*func)(Character*, UObject* obj);
};
/******************************************************/


#endif
