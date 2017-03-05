/*
 * Horde.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Horde.h"
#include "ChimeraMonster.h"

Horde::Horde(const ChimeraMonster &leader) :
		danger(0),
		purpose(HordePurposeType::GENERAL),
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

hordeType Horde::convertToStruct() {
	hordeType hordeStruct = hordeType();

	int dangerDelta;
	if (this->danger <= 5) {
		dangerDelta = 2;
	} else if (this->danger <= 10) {
		dangerDelta = 3;
	} else if (this->danger <= 15) {
		dangerDelta = 4;
	} else if (this->danger <= 21) {
		dangerDelta = 5;
	} else {
		dangerDelta = 6;
	}
	hordeStruct.minLevel = MAX(1, this->danger - dangerDelta);
	hordeStruct.maxLevel = MIN(DEEPEST_LEVEL-1, this->danger + dangerDelta);
	if (this->danger >= 28) {
		hordeStruct.maxLevel = DEEPEST_LEVEL-1;
	}

	short frequency;
	switch (this->purpose) {
		case HordePurposeType::FODDER: 			frequency = 15;				break;
		case HordePurposeType::SPECIAL: 		frequency = 6;				break;
		default: 								frequency = 10;				break;
	}
	if (this->purpose == HordePurposeType::SPECIAL && this->danger < 5) {
		frequency /= 2;
	}

//	if (this->purpose == HordePurposeType::TOTEM || this->purpose == HordePurposeType::TURRET) {
//		hordeStruct->flags &= HORDE_NO_PERIODIC_SPAWN;
//	}
	// TODO: immobile flag

	hordeStruct.leaderType = leader.monsterId;

	// TODO: machine flag
	//hordeStruct->machine = MT_CAMP_AREA;

	hordeStruct.numberOfMemberTypes = this->members.size();
	int i = 0;
	for (HordeMember *member : this->members) {
		hordeStruct.memberType[i] = member->member.monsterId;
		hordeStruct.memberCount[i] = {member->minCount, member->maxCount, 1};
		i += 1;
	}

	// TODO: spawnsIn

	return hordeStruct;
}
