/*
 * ChimeraMonster.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef CHIMERAMONSTER_H_
#define CHIMERAMONSTER_H_

#include <string>
#include <list>
#include <functional>
#include <vector>
#include "MonsterGlobals.h"
#include "IncludeGlobals.h"

class Body;
class Ability;

enum GenerateFlag : int {
    AQUATIC             = 1 << 1,
    ANIMAL              = 1 << 2,
    SUPPORTS_CLASS      = 1 << 3,
    PACK_MEMBER         = 1 << 4,
    SHAMANISTIC         = 1 << 5,
    WIZARDLY            = 1 << 6,
    INSECTOID           = 1 << 7,
    ARMED               = 1 << 8,
    AMORPHOUS           = 1 << 9,
    BURSTS              = 1 << 10,
    KAMIKAZE            = 1 << 11,
    THIEVING            = 1 << 12,
};

enum class RegenSpeedType {
    EXTREMELY_FAST,
    VERY_FAST,
    FAST,
    NORMAL,
    NONE,
};

enum class MoveSpeedType {
    FAST,
    NORMAL,
    SLOW,
    VERY_SLOW,
};

enum class AttackSpeedType {
    NORMAL,
    SLOW,
    TURRET,
    TOTEM,
};

enum class AccuracyType {
    ACCURATE,
    NORMAL,
    INACCURATE,
};

enum class DefenseType {
    DEFENSELESS,
    LOW,
    NORMAL,
    HIGH,
};

enum class GenderType {
    NONE,
    BOTH,
    MALE_ONLY,
    FEMALE_ONLY,
};

struct AbsorbFlavorType {
    std::string message;
    std::string status;
};

class ChimeraMonster {

public:

    ChimeraMonster(Body &body);
    virtual ~ChimeraMonster();

    /**
     * Convert to what the rest of Brogue uses. The provided struct will have its monsterID set,
     * which needs to be respected.
     */
    creatureType convertToStruct();

    std::string debugReport() const;

    void applyAbility(Ability &ability);

    // flavor
    std::string name;
    const color *displayColor;
    uchar displayChar;
    dungeonFeatureTypes bloodType;
    lightType lightType;
    GenderType gender;

    // rpg
    short hp;
    randomRange damage;
    AccuracyType accuracy;
    DefenseType defense;
    RegenSpeedType regenSpeed;
    MoveSpeedType moveSpeed;
    AttackSpeedType attackSpeed;
    
    // feature
    dungeonFeatureTypes feature;
    std::string featureMessage;
    bool featureKamikaze;
    int featurePeriodicPercent;

    // flags
    std::vector<boltType> bolts;
    unsigned long flags;                // bitset of monsterBehaviorFlags
    unsigned long abilFlags;            // bitset of monsterAbilityFlags

    // generation
    unsigned long genFlags;             // bitset of GenerateFlag
    int monsterId;
    int dangerLevel;

private:

    static short regenSpeedToTurnsPerRegen(RegenSpeedType speed);
    static short moveSpeedToTicksPerMove(MoveSpeedType speed);
    static short attackSpeedToTicksPerAttack(AttackSpeedType speed);

    void specializeName();
    std::string generateFlavor();
    AbsorbFlavorType generateAbsorbFlavor();
    std::list<std::string> generateAttackFlavor();
    std::string generateSummonFlavor();

    // generation
    const Body &body;
    std::list<std::reference_wrapper<Ability>> abilities;

    static std::string boltToString(boltType bolt);
    static std::string dungeonFeatureToString(dungeonFeatureTypes feature);

    static int nextChimeraId;

};

#endif /* CHIMERAMONSTER_H_ */
