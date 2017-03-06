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
#include <functional>
#include "MonsterGlobals.h"
#include "IncludeGlobals.h"

class Body;
class Ability;

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

	ChimeraMonster(Body &body);
	virtual ~ChimeraMonster();

	/**
	 * Convert to what the rest of Brogue uses. The provided struct will have its monsterID set,
	 * which needs to be respected.
	 */
	creatureType convertToStruct();

	std::string debugReport() const;

	void applyAbility(Ability &ability);

	// flavor
	std::string name;
	const color *displayColor;
	uchar displayChar;
	dungeonFeatureTypes bloodType;
	lightType lightType;
	GenderType gender;

	// rpg
	short hp;
	randomRange damage;
	AccuracyType accuracy;
	DefenseType defense;
	RegenSpeedType regenSpeed;
	MoveSpeedType moveSpeed;
	AttackSpeedType attackSpeed;

	// flags
	bool flies;
	bool flits;

	// generation
	int monsterId;
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
	std::list<std::reference_wrapper<Ability>> abilities;

	static int nextChimeraId;

};

#endif /* CHIMERAMONSTER_H_ */
