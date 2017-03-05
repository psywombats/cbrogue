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
	this->monsters = std::list<ChimeraMonster *>();
	this->bodies = std::list<Body *>();
}

MonsterGenerator::~MonsterGenerator() {
	for (Body *body : this->bodies) {
		delete body;
	}
	for (ChimeraMonster *monster : this->monsters) {
		delete monster;
	}
}

std::list<ChimeraMonster *> &MonsterGenerator::getMonsters() {
	return this->monsters;
}

void MonsterGenerator::generate() {
	//this->bodies = Body::loadBodies();
}
