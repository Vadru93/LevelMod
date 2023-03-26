
select_shift = 0

script UserSelectSelect
  ToggleSkaterCamMode skater = 0
endscript

script Speed_Scripts
  SetSpeed <Speed>
endscript

script SetSpeedScripts
  MakeSkaterGoto Speed_Scripts
endscript

script UserSelectSelect2
  ToggleSkaterCamMode skater = 1
endscript

script send_chat
  SendChatMessage <...>
endscript

script UserSelectTriangle
  ToggleShadowType
  StopAllScripts
  send_chat string = "lol"
endscript

script Teleps
  Obj_MoveToNode name = Object1 Orient NoReset
endscript

script UserSelectSquare
  ReplaceSkinTexture replace = "CS_NH_cargoshorts.png" with = "CS_NH_cargoshorts_HD.png" image_dir = "textures/skater_m" ScreenShot
endscript

script UserSelectCircle
  //MoveObject name = wild_Log_Loader_Base_High relpos = (0, 2000, 0)
  TestReloadQB
endscript

script UserSelectStart
  RestripWorld levels = [ "Levels\SI\SI.TDX" "Levels\SI\SI.bsp" ]
  Obj_MoveToNode name = Object1 Orient NoReset
  PausePhysics
  SwitchToMenu menu = pro_menu
endscript

script UserSelectX
  ToggleViewMode
endscript
