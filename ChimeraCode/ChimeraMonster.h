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
#include "Rogue.h"

class Body;

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

	ChimeraMonster(const Body &body);

	/**
	 * Convert to what the rest of Brogue uses. Caller is responsible for cleanup. The provided
	 * struct won't have its monsterID set -- that should be fixed when it's actually added to the
	 * monster catalog.
	 */
	creatureType *convertToStruct();

	virtual ~ChimeraMonster();

	// flavor
	std::string name;
	const color *displayColor;
	uchar displayChar;
	dungeonFeatureTypes bloodType;
	lightType lightType;
	GenderType gender;

	// rpg
	short hp;
	short defense;
	randomRange damage;
	AccuracyType accuracy;
	RegenSpeedType regenSpeed;
	MoveSpeedType moveSpeed;
	AttackSpeedType attackSpeed;

	// flags
	bool flies;
	bool flits;

	// generation
	int dangerLevel;

private:

	static short regenSpeedToTurnsPerRegen(RegenSpeedType speed);
	static short moveSpeedToTicksPerMove(MoveSpeedType speed);
	static short attackSpeedToTicksPerAttack(AttackSpeedType speed);

	std::string generateFlavor();
	AbsorbFlavorType generateAbsorbFlavor();
	std::list<std::string> generateAttackFlavor();
	std::string generateSummonFlavor();

	// generation
	const Body &body;

};

#endif /* CHIMERAMONSTER_H_ */
