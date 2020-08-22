SCRIPT animload_skater_m
	animload_skater_m_fliptricks
	animload_skater_m_basics
	animload_skater_m_bails
	animload_skater_m_grabtricks
	animload_skater_m_flipgrab
	animload_skater_m_grind
	animload_skater_m_lip
	animload_skater_m_special
	animload_skater_m_flatland
	animload_skater_m_misc
	animload_skater_m_newtricks
ENDSCRIPT

//this script loads additional animations, found in THPS3 1.3 mod
//this is here to prevent crashing online, in case someone loads with 1.3 tricks
//note that salute checksums were added manually, there is no correct string for that, so it's in hex
SCRIPT animload_skater_m_newtricks
	LoadAnim name = "Anims\skater_m\skater_m_IndyBackFlip.ska" descChecksum = IndyBackFlip
	LoadAnim name = "Anims\skater_m\skater_m_SomiSpin.ska" descChecksum = SomiSpin
	LoadAnim name = "Anims\skater_m\skater_m_ShoveItRewind.ska" descChecksum = ShoveItRewind
	LoadAnim name = "Anims\skater_m\skater_m_TripleHeelFlipARstyle.ska" descChecksum = THARstyle
	LoadAnim name = "Anims\skater_m\skater_m_PogoAir.ska" descChecksum = PogoAir
	LoadAnim name = "Anims\skater_m\skater_m_Salute_Idle.ska" descChecksum = 0x48474645
	LoadAnim name = "Anims\skater_m\skater_m_Salute_Init.ska" descChecksum = 0x49484746
ENDSCRIPT