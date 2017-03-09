/*
 * Horde.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Horde.h"
#include "ChimeraMonster.h"

Horde::Horde(const ChimeraMonster &leader) :
        purpose(HordePurposeType::GENERAL),
        extraRange(0),
        extraFrequency(0),
        leader(leader) {
    this->members = std::list<HordeMember *>();
}

Horde::~Horde() {
    for (HordeMember *member : this->members) {
        delete member;
    }
}

void Horde::addMember(const ChimeraMonster &monster, short minCount, short maxCount) {
    HordeMember *member = new HordeMember(monster, minCount, maxCount);
    this->members.push_back(member);
}

std::string Horde::debugReport() const {
    std::string report = "";

    report += leader.name + " (DL " + printInt(minDL()) + "-" + printInt(maxDL());
    report += "  freq. " + printInt(calculateFrequency()) + ")\n";

    for (HordeMember *member : this->members) {
        report += "  " + member->member.name + " {" + printInt(member->minCount) + "-" + printInt(member->maxCount) + "}\n";
    }

    return report;
}

int Horde::memberCount() const {
    return this->members.size() + 1;
}

hordeType Horde::convertToStruct() {
    hordeType hordeStruct = hordeType();

    int danger = this->calculateDL();
    hordeStruct.minLevel = this->minDL();
    hordeStruct.maxLevel = this->maxDL();
    if (danger >= 28) {
        hordeStruct.maxLevel = DEEPEST_LEVEL-1;
    }
    
    hordeStruct.frequency = this->calculateFrequency();

    if (this->purpose == HordePurposeType::TOTEM) {
        hordeStruct.flags = (hordeFlags)(hordeStruct.flags | HORDE_NO_PERIODIC_SPAWN);
        if (this->leader.genFlags & GF_SHAMANISTIC) {
            if (rand_percent(60)) {
                hordeStruct.machine = MT_CAMP_AREA;
            } else {
                hordeStruct.machine = MT_DISMAL_AREA;
            }
        } else if (this->leader.genFlags & GF_SHAMANISTIC) {
            int roll = rand_range(0, 100);
            if (roll < 50) {
                hordeStruct.machine = MT_REMNANT_AREA;
            } else if (roll < 80) {
                hordeStruct.machine = MT_IDYLL_AREA;
            } else {
                hordeStruct.machine = MT_SWAMP_AREA;
            }
        }
    }
    if (this->purpose == HordePurposeType::TURRET) {
        hordeStruct.flags = (hordeFlags)(hordeStruct.flags | HORDE_NO_PERIODIC_SPAWN);
        hordeStruct.spawnsIn = WALL;
    }
    
    if (this->purpose == HordePurposeType::AQUA) {
        hordeStruct.flags = (hordeFlags)(hordeStruct.flags | HORDE_NEVER_OOD);
    }

    hordeStruct.leaderType = leader.monsterId;

    hordeStruct.numberOfMemberTypes = this->members.size();
    int i = 0;
    for (HordeMember *member : this->members) {
        hordeStruct.memberType[i] = member->member.monsterId;
        hordeStruct.memberCount[i] = {member->minCount, member->maxCount, 1};
        i += 1;
    }

    if (leader.flags & MONST_SUBMERGES) {
        applySpecialSpawn(hordeStruct, MONST_IMMUNE_TO_FIRE, LAVA);
        applySpecialSpawn(hordeStruct, MONST_IMMUNE_TO_WATER, DEEP_WATER);
    }
    
    return hordeStruct;
}

void Horde::applySpecialSpawn(hordeType &hordeStruct, monsterBehaviorFlags flag, tileType tile) {
    if ((this->leader.flags & flag) > 0) {
        bool spawnsSpecial = true;
        for (HordeMember *member : this->members) {
            if ((member->member.flags & flag) == 0) {
                spawnsSpecial = false;
                break;
            }
        }
        if (spawnsSpecial) {
            hordeStruct.spawnsIn = tile;
        }
    }
}

int Horde::calculateDL() const {
    int danger = leader.dangerLevel;
    for (HordeMember *member : this->members) {
        if (member->member.dangerLevel > danger) {
            danger = member->member.dangerLevel;
        }
    }
    if (this->purpose == HordePurposeType::TOTEM) {
        danger += this->members.size() * 2;
    } else {
        if (this->members.size() >= 1) {
            danger += CLAMP(this->members.front()->member.dangerLevel / 2, 2, 9);
        }
        if (this->members.size() >= 2) {
            danger += CLAMP(this->members.front()->member.dangerLevel / 3, 2, 7);
        }
        if (this->members.size() >= 3) {
            danger += CLAMP(this->members.front()->member.dangerLevel / 4, 2, 5);
        }
    }
    return danger;
}

int Horde::minDL() const {
    int danger = this->calculateDL();
    int dangerDelta = this->dangerDelta();
    return MIN(MAX(1, danger - dangerDelta), DEEPEST_LEVEL);
}

int Horde::maxDL() const {
    int danger = this->calculateDL();
    int dangerDelta = this->dangerDelta();
    return MIN(MIN(DEEPEST_LEVEL-1, danger + dangerDelta), DEEPEST_LEVEL);
}

int Horde::dangerDelta() const {
    int dangerDelta;
    int danger = this->calculateDL();
    if (danger <= 5) {
        dangerDelta = 2;
    } else if (danger <= 10) {
        dangerDelta = 3;
    } else if (danger <= 15) {
        dangerDelta = 4;
    } else if (danger <= 21) {
        dangerDelta = 5;
    } else {
        dangerDelta = 6;
    }
    
    dangerDelta += this->extraRange;
    return dangerDelta;
}

int Horde::calculateFrequency() const {
    int frequency;
    switch (this->purpose) {
        case HordePurposeType::FODDER:              frequency = 15;                 break;
        case HordePurposeType::KAMIKAZE:            frequency = 4;                  break;
        case HordePurposeType::THIEF:               frequency = 8;                  break;
        default:                                    frequency = 10;                 break;
    }
    
    frequency -= (this->memberCount() - 1);
    frequency += extraFrequency;
    frequency = MAX(1, frequency);
    return frequency;
}
