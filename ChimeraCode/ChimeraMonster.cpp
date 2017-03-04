/*
 * ChimeraMonster.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "ChimeraMonster.h"

ChimeraMonster::ChimeraMonster() {

}

creatureType *ChimeraMonster::convertToStruct() {
	creatureType *creatureStruct = (creatureType *) malloc(sizeof(creatureType));

	memcpy(&creatureStruct->absorbStatus, this->name.c_str(), this->name.length()+1);
	creatureStruct->displayChar = this->displayChar;
	memcpy(&creatureStruct->foreColor, &this->displayColor, sizeof(color));
    creatureStruct->maxHP = this->hp;
    creatureStruct->defense = this->defense;
    creatureStruct->accuracy = this->accuracy;
    memcpy(&creatureStruct->damage, &this->damage, sizeof(randomRange));
    creatureStruct->turnsBetweenRegen = regenSpeedToTurnsPerRegen(this->regenSpeed);
    creatureStruct->movementSpeed = moveSpeedToTicksPerMove(this->moveSpeed);
    creatureStruct->attackSpeed = attackSpeedToTicksPerAttack(this->attackSpeed);
    creatureStruct->bloodType = this->bloodType;
    creatureStruct->intrinsicLightType = this->lightType;

    AbsorbFlavorType absorb = this->generateAbsorbFlavor();
    std::string flavor = this->generateFlavor();
    memcpy(&creatureStruct->absorbStatus, flavor.c_str(), flavor.length()+1);
    memcpy(&creatureStruct->absorbStatus, absorb.status.c_str(), absorb.status.length()+1);
    memcpy(&creatureStruct->absorbing, absorb.message.c_str(), absorb.message.length()+1);

    // TODO
    //short DFChance;                     // percent chance to spawn the dungeon feature per awake turn
    //enum dungeonFeatureTypes DFType;    // kind of dungeon feature
    //enum boltType bolts[20];
    //unsigned long flags;
    //unsigned long abilityFlags;
    //char summonMessage[DCOLS * 2];

	return creatureStruct;
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

ChimeraMonster::~ChimeraMonster() {

}

