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
#include <set>
#include <map>
#include "MonsterGlobals.h"
#include "IncludeGlobals.h"

class Body;
class Ability;

enum GenerateFlag : int {
    GF_AQUATIC              = 1 << 1,
    GF_AQUATIC_ONLY         = 1 << 2,
    GF_ANIMAL               = 1 << 3,
    GF_SUPPORTS_CLASS       = 1 << 4,
    GF_PACK_MEMBER          = 1 << 5,
    GF_SHAMANISTIC          = 1 << 6,
    GF_WIZARDLY             = 1 << 7,
    GF_INSECTOID            = 1 << 8,
    GF_ARMED                = 1 << 9,
    GF_AMORPHOUS            = 1 << 10,
    GF_BURSTS               = 1 << 11,
    GF_KAMIKAZE             = 1 << 12,
    GF_THIEVING             = 1 << 13,
    GF_THIEVING_ONLY        = 1 << 14,
    GF_TOTEM                = 1 << 15,
    GF_MOOKISH              = 1 << 16,
    GF_NO_SPECIALS          = 1 << 17,
    GF_TURRET               = 1 << 18,
    GF_BOSSLIKE             = 1 << 19,
    GF_BOSS_ONLY            = 1 << 20,
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

enum class PhysiqueType {
    SPELLCASTER,
    NORMAL,
    BUFF,
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

    std::string debugReport();
    const std::string &getDisplayName() const;

    void applyAbility(Ability &ability);

    // flavor
    std::string baseName;
    const color *displayColor;
    uchar baseDisplayChar;
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
    PhysiqueType physique;
    
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
    Body &body;
    std::string mookName;
    std::string nameSuffix, namePrefix;
    unsigned long genFlags;             // bitset of GenerateFlag
    int monsterId;
    int dangerLevel;

private:

    static short regenSpeedToTurnsPerRegen(RegenSpeedType speed);
    static short moveSpeedToTicksPerMove(MoveSpeedType speed);
    static short attackSpeedToTicksPerAttack(AttackSpeedType speed);
    
    static std::string boltToString(boltType bolt);
    static std::string dungeonFeatureToString(dungeonFeatureTypes feature);
    static void replace(std::string &source, const std::string &token, const std::string &replacement);
    
    static int nextChimeraId;
    static std::map<std::reference_wrapper<uchar>, std::string> usedChars;

    void generateName();
    void generateDisplayChar();
    void generateFlavor();
    
    AbsorbFlavorType generateAbsorbFlavor() const;
    std::list<std::string> generateAttackFlavor() const;
    std::string generateSummonFlavor() const;
    
    bool otherSpeciesUsesChar();

    // generation
    std::list<std::reference_wrapper<Ability>> abilities;
    uchar displayChar;
    std::string displayName;
    std::string flavor;
};

#endif /* CHIMERAMONSTER_H_ */
