use uo;
use os;

include "include/client";

function summon_creature( caster, circle := 8, npctemplate, cast_loc )
    
	local thecreature;
	local thecritter;
	local IsMob := 0;
	local magery := GetSkill(caster, SKILLID_MAGERY);
	local duration := 20 + (magery * 2);

	local parms := {};

	if (!caster.npctemplate)
		parms .+ script := "tamed";
	else
		IsMob := 1;
	endif

	thecritter := CreateNPCfromTemplate(npctemplate, cast_loc.x, cast_loc.y,cast_loc.z, parms);
	if (!IsMob)
		SetObjProperty( thecritter , "master" , caster.serial );
		SetObjProperty( thecritter , "script" , thecritter.script );
   
		thecritter.setmaster( caster );

		thecritter.script := "tamed";
		RestartScript(thecritter);
	endif

	SetObjProperty(thecritter, "summoned",1);
	SetRawSkill( thecritter, SKILLID_MAGICRESISTANCE, BaseSkillToRawSkill( magery*10 ) );
	PlaySoundEffect(caster, SFX_SPELL_SUMMON_ELEMENTAL);

	detach();

	local ev Array;
	ev.+type := 0x0002;
	ev.+source := 0;

	if (IsMob)
	foreach critter in ListMobilesInLineOfSight( thecritter, 5 )
		if ((!critter.npctemplate) or (critter.script = "tamed"))
                	ev.source := critter;
			SendEvent(thecritter, ev);
		endif
	endforeach
	endif

	sleep(duration);
	PlaySoundEffect( thecritter , SFX_SPELL_MANA_VAMPIRE );
	PlayStationaryEffect( thecritter.x, thecritter.y, thecritter.z, FX_SMOKE, 0xA, 0xA );
	MoveCharacterToLocation(thecritter,0,0,0,MOVECHAR_FORCELOCATION);

	ApplyRawDamage(thecritter, thecritter.hp + 3);

endfunction
