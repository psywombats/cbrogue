//
//  MonsterGlobals.h
//  cBrogue
//
//  Created by Aaron King on 9/6/16.
//
//

#ifndef MonsterGlobals_h
#define MonsterGlobals_h

#include <stdio.h>
#include "Rogue.h"

// returns the array of monsters generated this game
creatureType *getMonsterCatalog();
hordeType *getHordeCatalog();
monsterClass *getMonsterClassCatalog();

int getMonsterCatalogCount();
int getMonsterClassCount();

// individual monsters needed for machines etc
int getRatTrapMonsterId();
int getWarrenBossMonsterId();
int getVampireBossMonsterId();
int getWingedGuardianMonsterId();
int getGuardianMonsterId();
int getSentinelMonsterId();
int getWardenMonsterId();
int getMirrorMonsterId();
int getWebberMonsterId();
int getDiggerMonsterId();
int getSparkMonsterId();
int getUndeadMonsterId();
int getInvisibleMonsterId();
int getSpectralImageMonsterId();
int getConjurationMonsterId();
int getCharmSummonMonsterId();

#endif /* MonsterGlobals_h */
