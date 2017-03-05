/*
 * Body.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef BODY_H_
#define BODY_H_

#include "ChimeraMonster.h"

/**
 * Bodies are the first thing applied to monsters and serve as a template for physical stats.
 * Attaching a body should only be done once.
 */
class Body {

public:
	Body();
	virtual ~Body();

	static std::list<Body *> loadBodies;

	void applyToMonster(ChimeraMonster &monster) const;

	// rpg
	short hp;
	short minDamage;
	short maxDamage;
	AccuracyType accuracy;
	DefenseType defense;
	MoveSpeedType moveSpeed;
	RegenSpeedType regenSpeed;
	AttackSpeedType attackSpeed;

	// flavor
	std::string baseName;

	// generation
	int dangerLevel;
	bool flier;
	bool intelligent;
	GenderType gender;
};

#endif /* BODY_H_ */
