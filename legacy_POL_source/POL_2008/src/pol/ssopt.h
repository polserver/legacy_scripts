// History
//   2005/02/23 Shinigami: ServSpecOpt DecayItems to enable/disable item decay
//   2005/03/05 Shinigami: ServSpecOpt UseAAnTileFlags to enable/disable "a"/"an" via Tiles.cfg in formatted Item Names
//   2005/06/15 Shinigami: ServSpecOpt UseWinLFH to enable/disable Windows XP/2003 low-fragmentation Heap
//   2005/08/29 Shinigami: ServSpecOpt UseAAnTileFlags renamed to UseTileFlagPrefix
//   2005/12/05 MuadDib: ServSpecOpt InvulTage using 0, 1, 2 for method of invul showing.

#ifndef SSOPT_H
#define SSOPT_H

#include <string>

struct ServSpecOpt {
    bool allow_secure_trading_in_warmode;
	unsigned long dblclick_wait;
    bool decay_items;
	unsigned long default_decay_time;
    unsigned short default_doubleclick_range;
    unsigned short default_light_level;
	bool event_visibility_core_checks;
	unsigned long max_pathfind_range;
    bool movement_uses_stamina;
    bool use_tile_flag_prefix;
	unsigned short default_container_max_items;
	unsigned short default_container_max_weight;
	bool hidden_turns_count;
    unsigned short invul_tag;
	unsigned short uo_feature_enable;
	unsigned short starting_gold;
	unsigned short item_color_mask;
    bool use_win_lfh;
	std::vector<std::string> total_stats_at_creation;
};

extern ServSpecOpt ssopt;

void read_servspecopt();
void ssopt_parse_totalstats(ConfigElem& elem);

#endif
