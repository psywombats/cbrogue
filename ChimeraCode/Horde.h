/*
 * Horde.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef HOARD_H_
#define HOARD_H_

#include <list>
#include <string>
#include "Rogue.h"

class ChimeraMonster;
class HordeMember;

enum class HordePurposeType {
    FODDER,
    GENERAL,
    KAMIKAZE,
    THIEF,
    AQUA,
    TOTEM,
    TURRET,
    SUMMON,
    CONJURATION,
    CAPTIVE,
};

class Horde {

public:

    Horde(const ChimeraMonster &leader);
    virtual ~Horde();

    /**
     * Convert to what the rest of Brogue uses.
     */
    hordeType convertToStruct() const;

    void addMember(const ChimeraMonster &member, short min, short max);
    std::string debugReport() const;
    int memberCount() const;
    int calculateDL() const;

    HordePurposeType purpose;
    
    int extraRange;
    int extraFrequency;
    int extraDanger;

private:

    const ChimeraMonster &leader;
    std::list<HordeMember *> members;
    
    int minDL() const;
    int maxDL() const;
    int dangerDelta() const;
    int calculateFrequency() const;
    void applySpecialSpawn(hordeType &hordeStruct, monsterBehaviorFlags flag, tileType tile) const;
};

class HordeMember {
public:
    HordeMember(const ChimeraMonster &member, short minCount, short maxCount) :
        member(member),
        minCount(minCount),
        maxCount(maxCount) {

    };

    const ChimeraMonster &member;
    short minCount;
    short maxCount;
};

#endif /* HOARD_H_ */
