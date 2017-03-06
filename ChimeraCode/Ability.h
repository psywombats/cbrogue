/*
 * Ability.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef ABILITY_H_
#define ABILITY_H_

#include <list>
#include "Rogue.h"
#include "ChimeraMonster.h"

enum class ColorModFlavor {
	NONE,
	POISONOUS,
	SPELLCASTING,
	COMBAT,
	MOBILITY,
};

/**
 * An ability represents a set of modifications to a body. Usually only one should be attached to a
 * body.
 */
class Ability {

public:

	Ability();
	virtual ~Ability();

	void applyToMonster(ChimeraMonster &monster);
	bool validForMonster(const ChimeraMonster &monster) const;

	// rpg
	int hpBoost;
	int dangerBoost;
	int minDamageBoost;
	int maxDamageBoost;
	RegenSpeedType regenSpeed;
	MoveSpeedType moveSpeed;
	AttackSpeedType attackSpeed;
	AccuracyType accuracy;
	DefenseType defense;
	bool flits;
	bool flies;

	// flavor
	std::string namePrefix;
	std::string nameSuffix;
	ColorModFlavor colorMod;

	// generation
	bool inUse;
	int requiredFlags;			// bitset of GenerateFlag

	static std::list<Ability *> loadModifierAbilities();

private:

	const color *blendColor(const color *baseColor) const;

};

#endif /* ABILITY_H_ */
