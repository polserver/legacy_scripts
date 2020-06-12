#ifndef GUILDS_H

class Guild;
class BObjectImp;

#include "bscript/execmodl.h"

class GuildExecutorModule : public TmplExecutorModule<GuildExecutorModule>
{
public:
	GuildExecutorModule( Executor& exec ) :
	TmplExecutorModule<GuildExecutorModule>( "Guilds", exec ) {};

	BObjectImp* mf_ListGuilds();
	BObjectImp* mf_CreateGuild();
	BObjectImp* mf_FindGuild();
	BObjectImp* mf_DestroyGuild();
};

Guild* FindOrCreateGuild( unsigned long guildid, unsigned long memberserial );
BObjectImp* CreateGuildRefObjImp( Guild* guild );

bool AreAllies( Guild* g1, Guild* g2 );
bool AreEnemies( Guild* g1, Guild* g2 );

#endif
