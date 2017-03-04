/*
 * Body.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Body.h"
#include "ChimeraMonster.h"

Body::Body() :
	hp(0),
	accuracy(AccuracyType::NORMAL),
	defense(DefenseType::NORMAL),
	moveSpeed(MoveSpeedType::NORMAL),
	attackSpeed(AttackSpeedType::NORMAL),
	regenSpeed(RegenSpeedType::NORMAL),
	maxDamage(0),
	minDamage(0) {

}

Body::~Body() {

}

void Body::applyToMonster(ChimeraMonster &monster) const {
	monster.hp = this->hp;
	monster.damage = (randomRange) {minDamage, maxDamage, (maxDamage - minDamage) / 3};
	monster.accuracy = this->accuracy;
	monster.defense = this->defense;

	monster.moveSpeed = this->moveSpeed;
	monster.attackSpeed = this->attackSpeed;
	monster.regenSpeed = this->regenSpeed;
}
