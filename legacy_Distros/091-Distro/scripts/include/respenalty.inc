use uo;

var maximumloss := 0.20; //20% loss for 20 shortterm murders or more

function ResPenalties(who)

	MurdererStatLoss(who);
	
	//more stuff to do on successful res
	
endfunction

function MurdererStatLoss(who)

	var shortc := GetObjProperty(who,"shortmurders");
	if(!shortc)
		return;
	endif
	if(shortc>4) //then murderer]
		var multiplier := cint(shortc)/100.0;
		if(multiplier > maximumloss)
			multiplier := maximumloss;
		endif
		multiplier := 1.0 - multiplier;
		
		var str := who.strength;
		var dex := who.dexterity;
		var inte := who.intelligence;

		str := cint(str * multiplier);
		dex := cint(dex * multiplier);
		inte := cint(inte * multiplier);

		who.SetStr(str);
		who.SetDex(dex);
		who.SetInt(inte);

		who.hp := who.maxhp;
		who.stamina := who.maxstamina;
		who.mana := who.maxmana;
		
		SendSysmessage(who,"Thy body pays the price for your sins.");
	endif
endfunction
