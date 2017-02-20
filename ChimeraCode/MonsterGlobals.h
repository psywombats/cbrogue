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
blueprint *getBlueprintCatalog();

int getMonsterCatalogCount();
int getMonsterClassCount();

// individual monsters needed for machines etc
int getSpectralImageMonsterId();
int getWardenMonsterId();
int getConjurationMonsterId();
int getCharmSummonMonsterId();

#endif /* MonsterGlobals_h */
