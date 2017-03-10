/*
 * Body.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef BODY_H_
#define BODY_H_

#include <list>
#include <set>
#include <functional>
#include "ChimeraMonster.h"
#include "Rogue.h"

/**
 * Bodies are the first thing applied to monsters and serve as a template for physical stats.
 * Attaching a body should only be done once.
 */
class Body {

public:
    Body();
    virtual ~Body();

    static std::vector<Body *> loadBodies();

    void applyToMonster(ChimeraMonster &monster);

    // rpg
    short hp;
    short minDamage;
    short maxDamage;
    AccuracyType accuracy;
    DefenseType defense;
    MoveSpeedType moveSpeed;
    RegenSpeedType regenSpeed;
    AttackSpeedType attackSpeed;
    unsigned long flags;                // monsterBehaviorTypes bitset
    unsigned long abilFlags;            // monsterAbilityTypes bitset

    // flavor
    std::string baseName;
    std::string flavor;
    uchar baseChar;
    const color *baseColor;
    lightType light;

    // generation
    unsigned long genFlags;            // generateFlag bitset
    int dangerLevel;
    int rarityPercent;
    bool reusable;
    bool inUse;

    // flavor
    GenderType gender;
    dungeonFeatureTypes blood;
    
    // special, probably leave these
    dungeonFeatureTypes periodicFeature;
    int periodicFeatureChance;
};

#endif /* BODY_H_ */
