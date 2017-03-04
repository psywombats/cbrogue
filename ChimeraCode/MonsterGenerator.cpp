/*
 * MonsterGenerator.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "MonsterGenerator.h"
#include "ChimeraMonster.h"
#include "Body.h"

MonsterGenerator::MonsterGenerator() {
	this->monsters = std::list<ChimeraMonster*>();
	this->bodies = std::list<Body*>();
}

MonsterGenerator::~MonsterGenerator() {
	for (Body *body : this->bodies) {
		delete body;
	}
	for (ChimeraMonster *monster : this->monsters) {
		delete monster;
	}
}

std::list<ChimeraMonster *> &MonsterGenerator::generateMonsters() {

	this->loadBodies();

	return monsters;
}

// ideally this thing would read its data from JSON
// but uhhh 7DRL YOLO
void MonsterGenerator::loadBodies() {
	Body *body;

	body = new Body();
	body->baseName = "rat";
	body->minDamage = 1;
	body->maxDamage = 3;
	body->hp = 6;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "kobold";
	body->minDamage = 1;
	body->maxDamage = 4;
	body->hp = 7;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "jackal";
	body->minDamage = 2;
	body->maxDamage = 4;
	body->hp = 8;
	body->moveSpeed = MoveSpeedType::FAST;
	bodies.push_back(body);
}

