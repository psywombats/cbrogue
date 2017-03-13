/*
 * Ability.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef ABILITY_H_
#define ABILITY_H_

#include <list>
#include <vector>
#include "Rogue.h"
#include "ChimeraMonster.h"

class Body;

enum class ColorModFlavor {
    NONE,
    POISONOUS,
    SPELLCASTING,
    COMBAT,
    MOBILITY,
    FIRE,
    TOTEM,
    SUMMONING,
};

/**
 * An ability represents a set of modifications to a body. Usually only one should be attached to a
 * body.
 */
class Ability {

public:

    Ability();
    virtual ~Ability();

    void applyToMonster(ChimeraMonster &monster);
    bool isValidForMonster(const ChimeraMonster &monster) const;
    bool isValidForMonster(const ChimeraMonster &monster, unsigned long ignoredFlags) const;
    bool isValidFor(const Body &body) const;
    bool isValidFor(const Body &body, unsigned long ignoredFlags, const ChimeraMonster *baseMook) const;

    // rpg
    int hpBoost;
    int dangerBoost;
    int minDamageBoost;
    int maxDamageBoost;
    RegenSpeedType regenSpeed;
    MoveSpeedType moveSpeed;
    AttackSpeedType attackSpeed;
    AccuracyType accuracy;
    DefenseType defense;
    PhysiqueType physique;
    SummonType summon;
    
    // feature
    dungeonFeatureTypes featureKamikaze;
    dungeonFeatureTypes featurePeriodic;
    std::string featureMessage;
    int featurePeriodicPercent;

    // flags
    std::vector<boltType> bolts;
    unsigned long flags;            // bitset of monsterBehaviorTypes
    unsigned long abilFlags;        // bitset of monsterAbilityFlags

    // flavor
    std::string namePrefix;
    std::string nameSuffix;
    std::string flavorOverride;
    std::string flavorAddition;
    std::string summonMessage;
    std::vector<std::string> hitMessages;
    ColorModFlavor colorMod;
    const color *colorOverride;
    lightType light;

    // generation
    bool inUse;
    int rarityPercent;
    unsigned long requiredFlags;    // bitset of GenerateFlag
    unsigned long forbiddenFlags;   // bitset of GenerateFlag
    int minDL;

    static std::vector<Ability *> loadModifierAbilities();

private:

    const color *blendColor(const color *baseColor) const;

};

#endif /* ABILITY_H_ */
