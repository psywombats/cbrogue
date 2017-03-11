/*
 * Horde.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Horde.h"
#include "ChimeraMonster.h"

Horde::Horde(const ChimeraMonster &newLeader) :
        purpose(HordePurposeType::GENERAL),
        extraRange(0),
        extraFrequency(0),
        leader(newLeader) {
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

    report += leader.getDisplayName() + " (DL " + printInt(minDL()) + "-" + printInt(maxDL());
    report += "  freq. " + printInt(calculateFrequency()) + ") ";
    
    hordeType tempStruct = convertToStruct();
    if (tempStruct.flags & HORDE_NEVER_OOD) report += "[never ood] ";
    if (tempStruct.flags & HORDE_IS_SUMMONED) report += "[summoned] ";
    if (tempStruct.flags & HORDE_MACHINE_MUD) report += "[mud] ";
    if (tempStruct.flags & HORDE_MACHINE_BOSS) report += "[boss] ";
    if (tempStruct.flags & HORDE_MACHINE_ONLY) report += "[machine only] ";
    if (tempStruct.flags & HORDE_MACHINE_THIEF) report += "[thief] ";
    if (tempStruct.flags & HORDE_LEADER_CAPTIVE) report += "[captive] ";
    if (tempStruct.flags & HORDE_MACHINE_KENNEL) report += "[kennel] ";
    if (tempStruct.flags & HORDE_MACHINE_STATUE) report += "[statue] ";
    if (tempStruct.flags & HORDE_MACHINE_TURRET) report += "[turret] ";
    if (tempStruct.flags & HORDE_VAMPIRE_FODDER) report += "[vampire food] ";
    if (tempStruct.flags & HORDE_MACHINE_GOBLIN_WARREN) report += "[warren] ";
    if (tempStruct.flags & HORDE_ALLIED_WITH_PLAYER) report += "[allied] ";
    if (tempStruct.flags & HORDE_DIES_ON_LEADER_DEATH) report += "[leaderbased] ";
    if (tempStruct.flags & HORDE_NO_PERIODIC_SPAWN) report += "[nospawn] ";
    if (tempStruct.flags & HORDE_SUMMONED_AT_DISTANCE) report += "[distance summon] ";
    if (tempStruct.flags & HORDE_MACHINE_WATER_MONSTER) report += "[watermon] ";
    if (tempStruct.flags & HORDE_MACHINE_LEGENDARY_ALLY) report += "[legendally] ";

    for (HordeMember *member : this->members) {
        report += "  " + member->member.getDisplayName() + " {" + printInt(member->minCount) + "-" + printInt(member->maxCount) + "}\n";
    }

    return report;
}

int Horde::memberCount() const {
    return this->members.size() + 1;
}

hordeType Horde::convertToStruct() const {
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
    if (this->purpose == HordePurposeType::SUMMON || this->purpose == HordePurposeType::CONJURATION) {
        hordeStruct.flags = (hordeFlags)(hordeStruct.flags | HORDE_IS_SUMMONED);
    }
    if (this->purpose == HordePurposeType::CONJURATION) {
        hordeStruct.flags = (hordeFlags)(hordeStruct.flags | HORDE_DIES_ON_LEADER_DEATH);
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

void Horde::applySpecialSpawn(hordeType &hordeStruct, monsterBehaviorFlags flag, tileType tile) const {
    if ((this->leader.flags & flag) > 0) {
        bool spawnsSpecial = true;
        for (HordeMember *member : this->members) {
            if (!(member->member.flags & flag)) {
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
        if (members.size() > 0) {
            if (purpose == HordePurposeType::FODDER) {
                danger += 1;
            } else {
                int totalDanger = 0;
                int count = 0;
                for (HordeMember *member : members) {
                    int avgMembers = (member->maxCount + member->minCount + 1) / 2;
                    count += avgMembers;
                    totalDanger += member->member.dangerLevel * avgMembers;
                }
                int avgDanger = totalDanger / count;
                if (avgDanger + 2 <= danger) {
                    // this is a swarm of similar DL monsters
                    danger += CLAMP(count, 2, 4);
                } else {
                    // the leader is powerful (mage?) with some trailing minions
                    danger += members.size() + CLAMP(avgDanger / 4, 0, 4);
                }
            }
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
        case HordePurposeType::FODDER:              frequency = 12;                 break;
        case HordePurposeType::KAMIKAZE:            frequency = 5;                  break;
        case HordePurposeType::THIEF:               frequency = 8;                  break;
        default:                                    frequency = 10;                 break;
    }
    
    frequency -= (this->memberCount() - 1);
    if (purpose == HordePurposeType::THIEF) {
        if (members.size() > 0) {
            frequency /= members.front()->maxCount;
        }
    }
    frequency += extraFrequency;
    frequency = MAX(1, frequency);
    return frequency;
}
