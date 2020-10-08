//All new LevelMod Scripts
//Created: 2020-08-29
Script KillSafely
    IF IsAlive name = <name>
        Kill name = <name>
    ENDIF
EndScript

Script UnPauseSkaters_Script
UnPauseSkaters
printf "Finished reloading NodeArray"
EndScript

Script ReloadNodeArray_Script
  PauseSkaters
  ReloadNodeArray
EndScript

//Render customization values
uv_anim_threshold = 0.02
uv_tiling_threshold = 1.0