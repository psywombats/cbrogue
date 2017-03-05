/*
 * Horde.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef HOARD_H_
#define HOARD_H_

#include <list>
#include "Rogue.h"

class ChimeraMonster;
class HordeMember;

class Horde {

public:

	Horde(const ChimeraMonster &leader);
	virtual ~Horde();

	/**
	 * Convert to what the rest of Brogue uses. Caller is responsible for cleanup.
	 */
	hordeType *convertToStruct();

	void addMember(const ChimeraMonster &member, short min, short max);

private:

	const ChimeraMonster &leader;
	std::list<HordeMember *> members;
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
