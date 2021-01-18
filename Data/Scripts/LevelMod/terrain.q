
TERRAIN_DEFAULT = 0
TERRAIN_CONCSMOOTH = 1
TERRAIN_CONCROUGH = 2
TERRAIN_METALSMOOTH = 3
TERRAIN_MEATALSMOOTH = 3
TERRAIN_METALROUGH = 4
TERRAIN_METALCORRUGATED = 5
TERRAIN_METALGRATING = 6
TERRAIN_METALTIN = 7
TERRAIN_WOOD = 8
TERRAIN_WOODMASONITE = 9
TERRAIN_WOODPLYWOOD = 10
TERRAIN_WOODFLIMSY = 11
TERRAIN_WOODSHINGLE = 12
TERRAIN_WOODPIER = 13
TERRAIN_BRICK = 14
TERRAIN_TILE = 15
TERRAIN_ASPHALT = 16
TERRAIN_ROCK = 17
TERRAIN_GRAVEL = 18
TERRAIN_SIDEWALK = 19
TERRAIN_GRASS = 20
TERRAIN_GRASSDRIED = 21
TERRAIN_DIRT = 22
TERRAIN_DIRTPACKED = 23
TERRAIN_WATER = 24
TERRAIN_ICE = 25
TERRAIN_SNOW = 26
TERRAIN_SAND = 27
TERRAIN_PLEXIGLASS = 28
TERRAIN_FIBERGLASS = 29
TERRAIN_CARPET = 30
TERRAIN_CONVEYOR = 31
TERRAIN_CHAINLINK = 32
TERRAIN_METALFUTURE = 33
TERRAIN_GENERIC1 = 34
TERRAIN_GENERIC2 = 35
TERRAIN_WHEELS = 36
TERRAIN_WETCONC = 37
TERRAIN_METALFENCE = 38
TERRAIN_GRINDTRAIN = 39
TERRAIN_GRINDROPE = 40

script SetTerrainDefaults
  LoadSound "FallWater" pitch = 35 vol = 100
  LoadSound "FallPungee"
  LoadSound "BonkBush"
  SetTerrainDefault
  SetTerrainConcSmooth
  SetTerrainConcRough
  SetTerrainMetalSmooth
  SetTerrainMetalRough
  SetTerrainMetalCorrugated
  SetTerrainMetalGrating
  SetTerrainMetalTin
  SetTerrainWood
  SetTerrainWoodMasonite
  SetTerrainWoodPlywood
  SetTerrainWoodFlimsy
  SetTerrainWoodShingle
  SetTerrainWoodPier
  SetTerrainBrick
  SetTerrainTile
  SetTerrainAsphalt
  SetTerrainRock
  SetTerrainGravel
  SetTerrainSidewalk
  SetTerrainGrass
  SetTerrainGrassDried
  SetTerrainDirt
  SetTerrainDirtPacked
  SetTerrainWater
  SetTerrainIce
  SetTerrainSnow
  SetTerrainSand
  SetTerrainPlexiglass
  SetTerrainFiberglass
  SetTerrainCarpet
  SetTerrainConveyor
  SetTerrainChainlink
  SetTerrainMetalFuture
  SetTerrainGeneric1
  SetTerrainGeneric2
  SetTerrainWetConcrete
  SetTerrainMetalFence
  SetTerrainGrindTrain
  SetTerrainGrindRope
endscript
RollConcSmoothValues = { maxPitch = 120 minPitch = 30 }
RollConcRoughValues = { maxPitch = 120 minPitch = 10 }
OllieConcValues = { maxVol = 140 minVol = 70 }
LandConcValues = { maxVol = 140 minVol = 70 }
BonkConcValues = { maxVol = 140 minVol = 70 }
RollMetalSmoothValues = { maxPitch = 120 minPitch = 30 }
RollMetalRoughValues = { maxPitch = 120 minPitch = 30 maxVol = 100 minVol = 20 }
RollMetalCorrugatedValues = { maxPitch = 250 minPitch = 150 }
OllieMetalValues = { maxVol = 140 minVol = 70 }
LandMetalValues = { maxPitch = 30 minPitch = 30 maxVol = 100 minVol = 40 }
BonkMetalValues = { maxVol = 100 minVol = 30 }
RollWoodValues = { maxPitch = 120 minPitch = 30 maxVol = 100 minVol = 20 }
RollWoodShingleValues = { maxPitch = 80 minPitch = 20 maxVol = 100 minVol = 20 }
OllieWoodValues = { maxVol = 120 minVol = 60 }
LandWoodValues = { maxVol = 120 minVol = 60 }
BonkWoodValues = { maxVol = 90 minVol = 30 }
CessValues = { maxVol = 110 minVol = 25 }

script SetTerrainDefault
endscript

script SetTerrainConcSmooth
  LoadSound "Terrains\RollConcSmooth" vol = 70
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Terrains\GrindConc"
  LoadSound "Terrains\SlideConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollConcSmooth { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_BONK LandConcValues }
  SetTerrainSfxProperties GrindConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_GRIND maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_GRINDJUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_GRINDLAND maxVol = 150 minVol = 100 }
  SetTerrainSfxProperties SlideConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_SLIDE maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_SLIDEJUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_SLIDELAND maxVol = 120 minVol = 80 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_CONCSMOOTH Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainConcRough
  LoadSound "Terrains\RollConcRough" vol = 100
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollConcRough { Terrain = TERRAIN_CONCROUGH Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_CONCROUGH Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CONCROUGH Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CONCROUGH Table = SK3SFX_TABLE_BONK LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_CONCROUGH Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainMetalSmooth
  LoadSound "Terrains\RollMetalSmooth"
  LoadSound "Terrains\OllieMetal" vol = 120
  LoadSound "Terrains\LandMetal"
  LoadSound "Terrains\BonkMetal" vol = 100
  LoadSound "Terrains\GrindMetal"
  LoadSound "Terrains\GrindMetalOn"
  LoadSound "Terrains\GrindMetalOff"
  LoadSound "Terrains\SlideMetal"
  LoadSound "Skater\RevertMetal" vol = 300
  SetTerrainSfxProperties RollMetalSmooth { Terrain = TERRAIN_METALSMOOTH Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieMetal { Terrain = TERRAIN_METALSMOOTH Table = SK3SFX_TABLE_JUMP OllieMetalValues }
  SetTerrainSfxProperties LandMetal { Terrain = TERRAIN_METALSMOOTH Table = SK3SFX_TABLE_LAND LandMetalValues }
  SetTerrainSfxProperties BonkMetal { Terrain = TERRAIN_METALSMOOTH Table = SK3SFX_TABLE_BONK BonkMetalValues }
  SetTerrainSfxProperties GrindMetal { Terrain = TERRAIN_METALMOOTH Table = SK3SFX_TABLE_GRIND maxPitch = 120 minPitch = 80 maxVol = 80 minVol = 50 }
  SetTerrainSfxProperties GrindMetalOff { Terrain = TERRAIN_METALMOOTH Table = SK3SFX_TABLE_GRINDJUMP maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties GrindMetalOn { Terrain = TERRAIN_METALMOOTH Table = SK3SFX_TABLE_GRINDLAND maxVol = 100 minVol = 50 }
  SetTerrainSfxProperties SlideMetal { Terrain = TERRAIN_METALMOOTH Table = SK3SFX_TABLE_SLIDE maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties GrindMetalOff { Terrain = TERRAIN_METALMOOTH Table = SK3SFX_TABLE_SLIDEJUMP maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties GrindMetalOn { Terrain = TERRAIN_METALMOOTH Table = SK3SFX_TABLE_SLIDELAND maxVol = 100 minVol = 50 }
  SetTerrainSfxProperties RevertMetal { Terrain = TERRAIN_METALSMOOTH Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainMetalRough
  LoadSound "Terrains\RollMetalRough"
  LoadSound "Terrains\OllieMetal" vol = 120
  LoadSound "Terrains\LandMetal"
  LoadSound "Terrains\BonkMetal" vol = 100
  LoadSound "Skater\RevertMetal" vol = 300
  SetTerrainSfxProperties RollMetalRough { Terrain = TERRAIN_METALROUGH Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieMetal { Terrain = TERRAIN_METALROUGH Table = SK3SFX_TABLE_JUMP OllieMetalValues }
  SetTerrainSfxProperties LandMetal { Terrain = TERRAIN_METALROUGH Table = SK3SFX_TABLE_LAND LandMetalValues }
  SetTerrainSfxProperties BonkMetal { Terrain = TERRAIN_METALROUGH Table = SK3SFX_TABLE_BONK BonkMetalValues }
  SetTerrainSfxProperties RevertMetal { Terrain = TERRAIN_METALROUGH Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainMetalCorrugated
  LoadSound "Terrains\RollMetalCorrugated"
  LoadSound "Terrains\OllieMetal" vol = 120
  LoadSound "Terrains\LandMetal"
  LoadSound "Terrains\BonkMetal" vol = 100
  LoadSound "Skater\RevertMetal" vol = 300
  SetTerrainSfxProperties RollMetalCorrugated { Terrain = TERRAIN_METALCORRUGATED Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieMetal { Terrain = TERRAIN_METALCORRUGATED Table = SK3SFX_TABLE_JUMP OllieMetalValues }
  SetTerrainSfxProperties LandMetal { Terrain = TERRAIN_METALCORRUGATED Table = SK3SFX_TABLE_LAND LandMetalValues }
  SetTerrainSfxProperties BonkMetal { Terrain = TERRAIN_METALCORRUGATED Table = SK3SFX_TABLE_BONK BonkMetalValues }
  SetTerrainSfxProperties RevertMetal { Terrain = TERRAIN_METALCORRUGATED Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainMetalGrating
  LoadSound "Terrains\RollMetalGrating"
  LoadSound "Terrains\OllieMetal" vol = 120
  LoadSound "Terrains\LandMetal"
  LoadSound "Terrains\BonkMetal" vol = 100
  LoadSound "Skater\RevertMetal" vol = 300
  SetTerrainSfxProperties RollMetalGrating { Terrain = TERRAIN_METALGRATING Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieMetal { Terrain = TERRAIN_METALGRATING Table = SK3SFX_TABLE_JUMP OllieMetalValues }
  SetTerrainSfxProperties LandMetal { Terrain = TERRAIN_METALGRATING Table = SK3SFX_TABLE_LAND LandMetalValues }
  SetTerrainSfxProperties BonkMetal { Terrain = TERRAIN_METALGRATING Table = SK3SFX_TABLE_BONK BonkMetalValues }
  SetTerrainSfxProperties RevertMetal { Terrain = TERRAIN_METALGRATING Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainMetalTin
  LoadSound "Terrains\RollMetalTin"
  LoadSound "Terrains\OllieMetal" vol = 120
  LoadSound "Terrains\LandMetal"
  LoadSound "Terrains\BonkMetal" vol = 100
  LoadSound "Skater\RevertMetal" vol = 300
  SetTerrainSfxProperties RollMetalTin { Terrain = TERRAIN_METALTIN Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieMetal { Terrain = TERRAIN_METALTIN Table = SK3SFX_TABLE_JUMP OllieMetalValues }
  SetTerrainSfxProperties LandMetal { Terrain = TERRAIN_METALTIN Table = SK3SFX_TABLE_LAND LandMetalValues }
  SetTerrainSfxProperties BonkMetal { Terrain = TERRAIN_METALTIN Table = SK3SFX_TABLE_BONK BonkMetalValues }
  SetTerrainSfxProperties RevertMetal { Terrain = TERRAIN_METALTIN Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainWood
  LoadSound "Terrains\RollWood" vol = 50
  LoadSound "Terrains\OllieWood"
  LoadSound "Terrains\LandWood" vol = 100
  LoadSound "Terrains\BonkWood" vol = 100
  LoadSound "Terrains\GrindWood" vol = 100
  LoadSound "Terrains\SlideWood" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_JUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_LAND LandWoodValues }
  SetTerrainSfxProperties BonkWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_BONK BonkWoodValues }
  SetTerrainSfxProperties GrindWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_GRIND maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_GRINDJUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_GRINDLAND maxVol = 100 minVol = 60 }
  SetTerrainSfxProperties SlideWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_SLIDE maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_SLIDEJUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_SLIDELAND maxVol = 120 minVol = 80 }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_WOOD Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainWoodMasonite
  LoadSound "Terrains\RollWoodMasonite" pitch = 50 vol = 80
  LoadSound "Terrains\OllieWood"
  LoadSound "Terrains\LandWood" vol = 100
  LoadSound "Terrains\BonkWood" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollWoodMasonite { Terrain = TERRAIN_WOODMASONITE Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOODMASONITE Table = SK3SFX_TABLE_JUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOODMASONITE Table = SK3SFX_TABLE_LAND LandWoodValues }
  SetTerrainSfxProperties BonkWood { Terrain = TERRAIN_WOODMASONITE Table = SK3SFX_TABLE_BONK BonkWoodValues }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_WOODMASONITE Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainWoodPlywood
  LoadSound "Terrains\RollWoodPlywood" vol = 80
  LoadSound "Terrains\OllieWood"
  LoadSound "Terrains\LandWood" vol = 100
  LoadSound "Terrains\BonkWood" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollWoodPlywood { Terrain = TERRAIN_WOODPLYWOOD Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOODPLYWOOD Table = SK3SFX_TABLE_JUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOODPLYWOOD Table = SK3SFX_TABLE_LAND LandWoodValues }
  SetTerrainSfxProperties BonkWood { Terrain = TERRAIN_WOODPLYWOOD Table = SK3SFX_TABLE_BONK BonkWoodValues }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_WOODPLYWOOD Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainWoodFlimsy
  LoadSound "Terrains\RollWoodFlimsy" vol = 80
  LoadSound "Terrains\OllieWood"
  LoadSound "Terrains\LandWood" vol = 100
  LoadSound "Terrains\BonkWood" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollWoodFlimsy { Terrain = TERRAIN_WOODFLIMSY Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOODFLIMSY Table = SK3SFX_TABLE_JUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOODFLIMSY Table = SK3SFX_TABLE_LAND LandWoodValues }
  SetTerrainSfxProperties BonkWood { Terrain = TERRAIN_WOODFLIMSY Table = SK3SFX_TABLE_BONK BonkWoodValues }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_WOODFLIMSY Table = SK3SFX_TABLE_CESSCessValues }
endscript

script SetTerrainWoodShingle
  LoadSound "Terrains\RollWoodShingle" vol = 80
  LoadSound "Terrains\OllieWood"
  LoadSound "Terrains\LandWood" vol = 100
  LoadSound "Terrains\BonkWood" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollWoodShingle { Terrain = TERRAIN_WOODSHINGLE Table = SK3SFX_TABLE_WHEELROLL maxPitch = 80 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOODSHINGLE Table = SK3SFX_TABLE_JUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOODSHINGLE Table = SK3SFX_TABLE_LAND LandWoodValues }
  SetTerrainSfxProperties BonkWood { Terrain = TERRAIN_WOODSHINGLE Table = SK3SFX_TABLE_BONK BonkWoodValues }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_WOODSHINGLE Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainWoodPier
  LoadSound "Terrains\RollWoodPier" vol = 80
  LoadSound "Terrains\OllieWood"
  LoadSound "Terrains\LandWood" vol = 100
  LoadSound "Terrains\BonkWood" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollWoodPier { Terrain = TERRAIN_WOODPIER Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_WOODPIER Table = SK3SFX_TABLE_JUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_WOODPIER Table = SK3SFX_TABLE_LAND LandWoodValues }
  SetTerrainSfxProperties BonkWood { Terrain = TERRAIN_WOODPIER Table = SK3SFX_TABLE_BONK BonkWoodValues }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_WOODPIER Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainBrick
  LoadSound "Terrains\RollBrick" vol = 80
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollBrick { Terrain = TERRAIN_BRICK Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_BRICK Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_BRICK Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_BRICK Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainTile
  LoadSound "Terrains\RollTile" vol = 80
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollTile { Terrain = TERRAIN_TILE Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_TILE Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_TILE Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_TILE Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainAsphalt
  LoadSound "Terrains\RollAsphalt" vol = 100
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollAsphalt { Terrain = TERRAIN_ASPHALT Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_ASPHALT Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_ASPHALT Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_ASPHALT Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainRock
  LoadSound "Terrains\RollRock" vol = 200
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollRock { Terrain = TERRAIN_ROCK Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_ROCK Table = SK3SFX_TABLE_JUMP maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_ROCK Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_ROCK Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainGravel
  LoadSound "Terrains\RollGravel" vol = 200
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollGravel { Terrain = TERRAIN_GRAVEL Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_GRAVEL Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_GRAVEL Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_GRAVEL Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainSidewalk
  LoadSound "Terrains\RollSidewalk" vol = 100
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollSidewalk { Terrain = TERRAIN_SIDEWALK Table = SK3SFX_TABLE_WHEELROLL maxPitch = 90 minPitch = 50 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_SIDEWALK Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_SIDEWALK Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_SIDEWALK Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainGrass
  LoadSound "Terrains\RollGrass"
  LoadSound "Terrains\LandDirt"
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollGrass { Terrain = TERRAIN_GRASS Table = SK3SFX_TABLE_WHEELROLL maxPitch = 90 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_GRASS Table = SK3SFX_TABLE_JUMP maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_GRASS Table = SK3SFX_TABLE_LAND maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_GRASS Table = SK3SFX_TABLE_CESS CessValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_GRASS num = 200 start_col = -15259859 end_col = 931772970 emit_w = 2.0 emit_h = 2.1 size = 6 aspect_ratio = 1 growth = 1.0 angle = -50.0 speed = 60.0 speed_range = 60.0 life = 0.9 grav = -0.5 name = "Grass_1.png" }
endscript

script SetTerrainGrassDried
  LoadSound "Terrains\RollGrassDried"
  LoadSound "Terrains\LandDirt"
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollGrassDried { Terrain = TERRAIN_GRASSDRIED Table = SK3SFX_TABLE_WHEELROLL maxPitch = 50 minPitch = 30 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_GRASSDRIED Table = SK3SFX_TABLE_JUMP maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_GRASSDRIED Table = SK3SFX_TABLE_LAND maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_GRASSDRIED Table = SK3SFX_TABLE_CESS CessValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_GRASSDRIED num = 200 start_col = -15259859 end_col = 931772970 emit_w = 2.0 emit_h = 2.1 size = 6 aspect_ratio = 1 growth = 1.0 angle = -50.0 speed = 60.0 speed_range = 60.0 life = 0.9 grav = -0.5 name = "Grass_1.png" }
endscript

script SetTerrainDirt
  LoadSound "Terrains\RollDirt" vol = 100
  LoadSound "Terrains\LandDirt"
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollDirt { Terrain = TERRAIN_DIRT Table = SK3SFX_TABLE_WHEELROLL maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_DIRT Table = SK3SFX_TABLE_JUMP maxPitch = 120 minPitch = 20 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_DIRT Table = SK3SFX_TABLE_LAND maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_DIRT Table = SK3SFX_TABLE_CESS CessValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_DIRT num = 20 start_col = 1516740274 end_col = 6793906 emit_w = 1.0 emit_h = 1.0 size = 25 aspect_ratio = 1.0 growth = 2.0 angle = 90.0 speed = 30.0 speed_range = 50.0 life = 0.6 grav = 0.5 name = "breath.png" }
endscript

script SetTerrainDirtPacked
  LoadSound "Terrains\RollDirtPacked" vol = 75
  LoadSound "Terrains\LandDirt"
  LoadSound "Terrains\OllieWood" vol = 100
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollDirtPacked { Terrain = TERRAIN_DIRTPACKED Table = SK3SFX_TABLE_WHEELROLL maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_DIRTPACKED Table = SK3SFX_TABLE_JUMP }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_DIRTPACKED Table = SK3SFX_TABLE_LAND maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_DIRTPACKED Table = SK3SFX_TABLE_CESS CessValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_DIRTPACKED num = 20 start_col = 1516740274 end_col = 6793906 emit_w = 1.0 emit_h = 1.0 size = 25 aspect_ratio = 1.0 growth = 2.0 angle = 90.0 speed = 30.0 speed_range = 50.0 life = 0.6 grav = 0.5 name = "breath.png" }
endscript

script SetTerrainWater
  LoadSound "Terrains\RollWater" vol = 150
  LoadSound "Terrains\OllieWater" vol = 100
  LoadSound "Terrains\LandWater" vol = 100
  SetTerrainSfxProperties RollWater { Terrain = TERRAIN_WATER Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 }
  SetTerrainSfxProperties OllieWater { Terrain = TERRAIN_WATER Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandWater { Terrain = TERRAIN_WATER Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_WATER num = 200 start_col = -1008349527 end_col = 166055593 emit_w = 2.0 emit_h = 5.0 size = 3 aspect_ratio = 0.45 growth = 2.0 angle = 45.0 speed = 90.0 speed_range = 60.0 life = 0.6 grav = 0.0 name = "breath.png" }
endscript

script SetTerrainIce
  LoadSound "Terrains\RollIce" vol = 70
  LoadSound "Terrains\LandDirt"
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollIce { Terrain = TERRAIN_ICE Table = SK3SFX_TABLE_WHEELROLL maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_ICE Table = SK3SFX_TABLE_JUMP maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_ICE Table = SK3SFX_TABLE_LAND maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_ICE Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainSnow
  LoadSound "Terrains\RollSnow" vol = 70
  LoadSound "Terrains\LandDirt"
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollSnow { Terrain = TERRAIN_SNOW Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_SNOW Table = SK3SFX_TABLE_JUMP maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_SNOW Table = SK3SFX_TABLE_LAND maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_SNOW Table = SK3SFX_TABLE_CESS CessValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_SNOW num = 20 start_col = 1526327776 end_col = 217704928 emit_w = 1.0 emit_h = 1.0 size = 21 aspect_ratio = 0.7 growth = 2.0 angle = 60.0 speed = 30.0 speed_range = 50.0 life = 0.7 grav = 2 name = "breath.png" }
endscript

script SetTerrainSand
  LoadSound "Terrains\RollSand" vol = 80
  LoadSound "Terrains\LandDirt"
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollSand { Terrain = TERRAIN_SAND Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_SAND Table = SK3SFX_TABLE_JUMP maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties LandDirt { Terrain = TERRAIN_SAND Table = SK3SFX_TABLE_LAND maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_SAND Table = SK3SFX_TABLE_CESS CessValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_SAND num = 20 start_col = 1516740274 end_col = 6793906 emit_w = 1.0 emit_h = 1.0 size = 25 aspect_ratio = 1.0 growth = 2.0 angle = 90.0 speed = 30.0 speed_range = 50.0 life = 0.6 grav = 0.5 name = "breath.png" }
endscript

script SetTerrainPlexiglass
  LoadSound "Terrains\RollPlexiglass" vol = 50
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc"
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollPlexiglass { Terrain = TERRAIN_PLEXIGLASS Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_PLEXIGLASS Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_PLEXIGLASS Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_PLEXIGLASS Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainFiberglass
  LoadSound "Terrains\RollFiberglass" vol = 50
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollFiberglass { Terrain = TERRAIN_FIBERGLASS Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_FIBERGLASS Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_FIBERGLASS Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_FIBERGLASS Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainCarpet
  LoadSound "Terrains\RollCarpet" vol = 100
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertWood" vol = 300
  SetTerrainSfxProperties RollCarpet { Terrain = TERRAIN_CARPET Table = SK3SFX_TABLE_WHEELROLL RollConcSmoothValues }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_CARPET Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CARPET Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertWood { Terrain = TERRAIN_CARPET Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainConveyor
  LoadSound "Terrains\RollConveyor" vol = 70
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandConc" vol = 100
  LoadSound "Skater\RevertMetal" vol = 300
  SetTerrainSfxProperties RollConveyor { Terrain = TERRAIN_CONVEYOR Table = SK3SFX_TABLE_WHEELROLL RollConcSmoothValues }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_CONVEYOR Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_CONVEYOR Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_CONVEYOR Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainChainlink
  LoadSound "Terrains\BonkChainlink" vol = 100
  SetTerrainSfxProperties BonkChainlink { Terrain = TERRAIN_CHAINLINK Table = SK3SFX_TABLE_BONK maxPitch = 120 minPitch = 80 maxVol = 100 minVol = 20 }
endscript

script SetTerrainMetalFuture
  LoadSound "Terrains\RollMetalFuture" vol = 700
  LoadSound "Terrains\OllieMetalFuture" vol = 200
  LoadSound "Terrains\LandMetalFuture" vol = 200
  LoadSound "Skater\RevertMetal" vol = 300
  SetTerrainSfxProperties RollMetalFuture { Terrain = TERRAIN_METALFUTURE Table = SK3SFX_TABLE_WHEELROLL RollMetalSmoothValues }
  SetTerrainSfxProperties OllieMetalFuture { Terrain = TERRAIN_METALFUTURE Table = SK3SFX_TABLE_JUMP OllieMetalValues }
  SetTerrainSfxProperties LandMetalFuture { Terrain = TERRAIN_METALFUTURE Table = SK3SFX_TABLE_LAND LandMetalValues }
  SetTerrainSfxProperties RevertMetal { Terrain = TERRAIN_METALSMOOTH Table = SK3SFX_TABLE_CESS CessValues }
endscript

script SetTerrainGeneric1
  LoadSound "Terrains\BonkMetalOutdoor" vol = 100
  SetTerrainSfxProperties BonkMetalOutdoor { Terrain = TERRAIN_GENERIC1 Table = SK3SFX_TABLE_BONK BonkMetalValues }
endscript

script SetTerrainGeneric2
endscript

script SetTerrainWetConc
  LoadSound "Terrains\RollWetConc" vol = 70
  LoadSound "Terrains\OllieConc"
  LoadSound "Terrains\LandWater" vol = 100
  LoadSound "Terrains\GrindConc"
  LoadSound "Terrains\SlideConc" vol = 100
  LoadSound "Terrains\LandConc"
  LoadSound "Skater\RevertConc" vol = 300
  SetTerrainSfxProperties RollWetConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_WHEELROLL maxPitch = 120 minPitch = 50 }
  SetTerrainSfxProperties LandWater { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_JUMP OllieConcValues }
  SetTerrainSfxProperties LandWater { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_LAND LandConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_BONK LandConcValues }
  SetTerrainSfxProperties GrindConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_GRIND maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_GRINDJUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_GRINDLAND maxVol = 150 minVol = 100 }
  SetTerrainSfxProperties SlideConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_SLIDE maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_SLIDEJUMP OllieConcValues }
  SetTerrainSfxProperties LandConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_SLIDELAND maxVol = 120 minVol = 80 }
  SetTerrainSfxProperties RevertConc { Terrain = TERRAIN_WETCONC Table = SK3SFX_TABLE_CESS CessValues }
  SetTerrainParticleProperties { Terrain = TERRAIN_WATER num = 200 start_col = -1008349527 end_col = 166055593 emit_w = 2.0 emit_h = 5.0 size = 3 aspect_ratio = 0.45 growth = 2.0 angle = 45.0 speed = 90.0 speed_range = 60.0 life = 0.6 grav = 0.0 name = "breath.png" }
endscript

script SetTerrainMetalFence
  LoadSound "Terrains\BonkMetalFence" vol = 400
  SetTerrainSfxProperties BonkMetalFence { Terrain = TERRAIN_METALFENCE Table = SK3SFX_TABLE_BONK BonkMetalValues }
endscript

script SetTerrainGrindTrain
  LoadSound "Terrains\GrindTrain" vol = 100
  SetTerrainSfxProperties GrindTrain { Terrain = TERRAIN_GRINDTRAIN Table = SK3SFX_TABLE_GRIND maxPitch = 100 minPitch = 60 maxVol = 200 minVol = 80 }
endscript

script SetTerrainGrindRope
  LoadSound "Terrains\GrindRope" vol = 50 pitch = 70
  LoadSound "Terrains\OllieWood" vol = 80
  LoadSound "Terrains\LandWood" vol = 80
  LoadSound "Terrains\Bonkwood" vol = 80
  SetTerrainSfxProperties GrindRope { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_GRIND maxPitch = 100 minPitch = 60 maxVol = 200 minVol = 80 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_JUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_LAND LandWoodValues }
  SetTerrainSfxProperties BonkWood { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_BONK BonkWoodValues }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_GRINDJUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_GRINDLAND maxVol = 100 minVol = 60 }
  SetTerrainSfxProperties GrindRope { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_SLIDE maxPitch = 100 minPitch = 60 maxVol = 100 minVol = 20 }
  SetTerrainSfxProperties OllieWood { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_SLIDEJUMP OllieWoodValues }
  SetTerrainSfxProperties LandWood { Terrain = TERRAIN_GRINDROPE Table = SK3SFX_TABLE_SLIDELAND maxVol = 120 minVol = 80 }
endscript
