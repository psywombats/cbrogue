/*
 * Horde.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Horde.h"
#include "ChimeraMonster.h"

Horde::Horde(const ChimeraMonster &leader) :
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

hordeType *Horde::convertToStruct() {
	return NULL;
}
