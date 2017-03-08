//
//  MonsterGlobals.h
//  cBrogue
//
//  Created by Aaron King on 9/6/16.
//
//

#ifndef MonsterGlobals_h
#define MonsterGlobals_h

#define CHIMERAS_ENABLED true

#include <stdio.h>
#include <string>
#include "Rogue.h"

// returns the array of monsters generated this game
creatureType *getMonsterCatalog();
hordeType *getHordeCatalog();
monsterClass *getMonsterClassCatalog();

void resetMonsterGeneration();

// utility
std::string printInt(int n);

int getMonsterCatalogCount();
int getMonsterClassCount();
int getHordeCatalogCount();

// individual monsters needed for machines etc
short getRatTrapMonsterId();
short getWarrenBossMonsterId();
short getVampireBossMonsterId();
short getWingedGuardianMonsterId();
short getGuardianMonsterId();
short getSentinelMonsterId();
short getWardenMonsterId();
short getMirrorMonsterId();
short getWebberMonsterId();
short getDiggerMonsterId();
short getSparkMonsterId();
short getUndeadMonsterId();
short getInvisibleMonsterId();
short getSpectralImageMonsterId();
short getConjurationMonsterId();
short getCharmSummonMonsterId();

#endif /* MonsterGlobals_h */
