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

	static std::list<Body *> loadBodies();

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
	bool flier;

	// flavor
	std::string baseName;
	char baseChar;
	color *baseColor;

	// generation
	int flags;				// generateFlag bitset
	int dangerLevel;
	bool inUse;

	// flavor
	GenderType gender;

private:

	static std::set<std::reference_wrapper<char>> usedChars;
};

#endif /* BODY_H_ */
