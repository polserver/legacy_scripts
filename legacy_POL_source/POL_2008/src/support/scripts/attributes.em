//
// Attribute related functions
//

// To-Fix: Uses a skillid - should take in an attribute name.
CheckSkill( character, skillid, difficulty, points );

AlterAttributeTemporaryMod( character, attrname, delta_tenths );

GetAttributeName( alias_name );

GetAttribute( character, attrname );
GetAttributeBaseValue( character, attrname );
GetAttributeIntrinsicMod( character, attrname );
GetAttributeTemporaryMod( character, attrname );

SetAttributeBaseValue( character, attrname, basevalue_tenths ); // obsoletes SetRawSkill
SetAttributeTemporaryMod( character, attrname, tempmod_tenths ); // obsoletes mob.strength_mod etc


BaseSkillToRawSkill( baseskill );
RawSkillToBaseSkill( rawskill );