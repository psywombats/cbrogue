/*
 * ChimeraMonster.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "ChimeraMonster.h"
#include "Body.h"
#include "IncludeGlobals.h"
#include "MonsterGlobals.h"
#include "Ability.h"
#include <stdlib.h>

int ChimeraMonster::nextChimeraId = 0;

ChimeraMonster::ChimeraMonster(Body &body) :
		hp(0),
		flags(0),
		displayChar('x'),
		accuracy(AccuracyType::NORMAL),
		defense(DefenseType::NORMAL),
		bloodType(DF_NONE),
		damage({0, 0, 0}),
		displayColor(&black),
		lightType(NO_LIGHT),
		body(body),
		dangerLevel(0),
		flies(false),
		flits(false),
		gender(GenderType::NONE),
		regenSpeed(RegenSpeedType::NORMAL),
		moveSpeed(MoveSpeedType::NORMAL),
		attackSpeed(AttackSpeedType::NORMAL) {

	body.applyToMonster(*this);

	this->abilities = std::list<std::reference_wrapper<Ability>>();

	if (ChimeraMonster::nextChimeraId == 0) {
		ChimeraMonster::nextChimeraId = getWardenMonsterId() + 1;
	}
	this->monsterId = ChimeraMonster::nextChimeraId;
	ChimeraMonster::nextChimeraId += 1;
}

ChimeraMonster::~ChimeraMonster() {

}

creatureType ChimeraMonster::convertToStruct() {
	creatureType creatureStruct = creatureType();

	creatureStruct.monsterID = this->monsterId;
	memcpy(&creatureStruct.monsterName, this->name.c_str(), this->name.length()+1);
	creatureStruct.displayChar = this->displayChar;
	creatureStruct.foreColor = this->displayColor;
    creatureStruct.maxHP = this->hp;
    memcpy(&creatureStruct.damage, &this->damage, sizeof(randomRange));
    creatureStruct.turnsBetweenRegen = regenSpeedToTurnsPerRegen(this->regenSpeed);
    creatureStruct.movementSpeed = moveSpeedToTicksPerMove(this->moveSpeed);
    creatureStruct.attackSpeed = attackSpeedToTicksPerAttack(this->attackSpeed);
    creatureStruct.bloodType = this->bloodType;
    creatureStruct.intrinsicLightType = this->lightType;

    AbsorbFlavorType absorb = this->generateAbsorbFlavor();
    std::string flavor = this->generateFlavor();
    memcpy(&creatureStruct.flavorText, flavor.c_str(), flavor.length()+1);
    memcpy(&creatureStruct.absorbStatus, absorb.status.c_str(), absorb.status.length()+1);
    memcpy(&creatureStruct.absorbing, absorb.message.c_str(), absorb.message.length()+1);

	if (this->hp < 10) {
		creatureStruct.accuracy = 70;
	} else if (this->hp < 19) {
		creatureStruct.accuracy = 85;
	} else if (this->hp < 50) {
		creatureStruct.accuracy = 100;
	} else if (this->hp < 75) {
		creatureStruct.accuracy = 125;
	} else if (this->hp < 95){
		creatureStruct.accuracy = 150;
	} else {
		creatureStruct.accuracy = 225;
	}
	if (accuracy == AccuracyType::ACCURATE) {
		creatureStruct.accuracy = (short)((float)creatureStruct.accuracy * 1.5f);
	}
	if (accuracy == AccuracyType::INACCURATE) {
		creatureStruct.accuracy -= 30;
	}

	if (this->hp < 10) {
		creatureStruct.defense = 0;
	} else if (this->hp < 50) {
		creatureStruct.defense = 20;
	} else if (this->hp < 70) {
		creatureStruct.defense = 55;
	} else if (this->hp < 95) {
		creatureStruct.defense = 70;
	} else {
		creatureStruct.defense = 90;
	}
	if (defense == DefenseType::DEFENSELESS) {
		creatureStruct.defense = 0;
	}
	if (defense == DefenseType::HIGH) {
		creatureStruct.defense += 50;
	}
	if (defense == DefenseType::LOW) {
		creatureStruct.defense /= 2;
	}

	creatureStruct.flags = 0;
	if (this->flies) {
		creatureStruct.flags |= MONST_FLIES;
	}
	if (this->flits) {
		creatureStruct.flags |= MONST_FLITS;
	}

    // TODO
    //short DFChance;                     // percent chance to spawn the dungeon feature per awake turn
    //enum dungeonFeatureTypes DFType;    // kind of dungeon feature
    //enum boltType bolts[20];
    //unsigned long abilityFlags;
    //char summonMessage[DCOLS * 2];

	return creatureStruct;
}

void ChimeraMonster::applyAbility(Ability &ability) {
	ability.applyToMonster(*this);
	this->abilities.push_back(ability);
}

std::string ChimeraMonster::debugReport() const {
	std::string report = "";

	report += name + " (id " + printInt(monsterId) + " DL " + printInt(dangerLevel) + ")\n";
	report += "  HP: " + printInt(hp) + "  dmg: " + printInt(damage.lowerBound) + "-" + printInt(damage.upperBound) + "\n";
	report += "  Specials: ";
	if (accuracy == AccuracyType::ACCURATE) report += "(accurate) ";
	if (accuracy == AccuracyType::INACCURATE) report += "(inaccurate) ";
	if (defense == DefenseType::DEFENSELESS) report += "(defenseless) ";
	if (defense == DefenseType::LOW) report += "(low def) ";
	if (defense == DefenseType::HIGH) report += "(hi def) ";
	if (moveSpeed == MoveSpeedType::FAST) report += "(fast move) ";
	if (moveSpeed == MoveSpeedType::SLOW) report += "(slow move) ";
	if (moveSpeed == MoveSpeedType::VERY_SLOW) report += "(v.slow move) ";
	if (attackSpeed == AttackSpeedType::SLOW) report += "(slow attack) ";
	if (attackSpeed == AttackSpeedType::TURRET) report += "(turret attack) ";
	if (attackSpeed == AttackSpeedType::TOTEM) report += "(totem attack) ";
	if (regenSpeed == RegenSpeedType::EXTREMELY_FAST) report += "(e.fast regen) ";
	if (regenSpeed == RegenSpeedType::FAST) report += "(fast regen) ";
	if (regenSpeed == RegenSpeedType::VERY_FAST) report += "(v.fast regen) ";
	report += "\n";

	return report;
}

short ChimeraMonster::regenSpeedToTurnsPerRegen(RegenSpeedType speed) {
	switch(speed) {
		case RegenSpeedType::EXTREMELY_FAST:		return 1;
		case RegenSpeedType::VERY_FAST:				return 5;
		case RegenSpeedType::FAST:					return 10;
		case RegenSpeedType::NORMAL:				return 20;
		case RegenSpeedType::NONE:					return 0;
	}
	return 0;
}

short ChimeraMonster::moveSpeedToTicksPerMove(MoveSpeedType speed) {
	switch(speed) {
		case MoveSpeedType::FAST:					return 50;
		case MoveSpeedType::NORMAL:					return 100;
		case MoveSpeedType::SLOW:					return 150;
		case MoveSpeedType::VERY_SLOW:				return 200;
	}
	return 0;
}

short ChimeraMonster::attackSpeedToTicksPerAttack(AttackSpeedType speed) {
	switch(speed) {
		case AttackSpeedType::NORMAL:				return 100;
		case AttackSpeedType::SLOW:					return 200;
		case AttackSpeedType::TURRET:				return 250;
		case AttackSpeedType::TOTEM:				return 350;
	}
	return 0;
}

std::string ChimeraMonster::generateFlavor() {
	return "a monster";
}

AbsorbFlavorType ChimeraMonster::generateAbsorbFlavor() {
	AbsorbFlavorType flavor;
	flavor.message = "absorbing";
	flavor.status = "Absorbing";
	return flavor;
}

std::list<std::string> ChimeraMonster::generateAttackFlavor() {
	std::list<std::string> list;
	list.push_back("hits");
	return list;
}

std::string ChimeraMonster::generateSummonFlavor() {
	return "summons allies.";
}

