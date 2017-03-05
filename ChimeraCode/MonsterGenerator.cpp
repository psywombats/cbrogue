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
	body->dangerLevel = 1;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "kobold";
	body->minDamage = 1;
	body->maxDamage = 4;
	body->hp = 7;
	body->dangerLevel = 2;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "jackal";
	body->minDamage = 2;
	body->maxDamage = 4;
	body->hp = 8;
	body->moveSpeed = MoveSpeedType::FAST;
	body->dangerLevel = 3;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "goblin";
	body->minDamage = 2;
	body->maxDamage = 5;
	body->hp = 15;
	body->defense = DefenseType::LOW;
	body->accuracy = AccuracyType::INACCURATE;
	body->dangerLevel = 6;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "toad";
	body->minDamage = 1;
	body->maxDamage = 4;
	body->hp = 8;
	body->defense = DefenseType::DEFENSELESS;
	body->dangerLevel = 2;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "bat";
	body->minDamage = 2;
	body->maxDamage = 6;
	body->hp = 18;
	body->moveSpeed = MoveSpeedType::FAST;
	body->dangerLevel = 7;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "mound";
	body->minDamage = 1;
	body->maxDamage = 3;
	body->hp = 15;
	body->accuracy = AccuracyType::INACCURATE;
	body->dangerLevel = 2;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "millipede";
	body->minDamage = 4;
	body->maxDamage = 12;
	body->hp = 18;
	body->accuracy = AccuracyType::INACCURATE;
	body->dangerLevel = 8;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "ogre";
	body->minDamage = 9;
	body->maxDamage = 13;
	body->hp = 55;
	body->attackSpeed = AttackSpeedType::SLOW;
	body->dangerLevel = 10;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "monster";
	body->minDamage = 3;
	body->maxDamage = 4;
	body->hp = 55;
	body->regenSpeed = RegenSpeedType::FAST;
	body->dangerLevel = 12;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "arachnid";
	body->minDamage = 3;
	body->maxDamage = 4;
	body->hp = 20;
	body->accuracy = AccuracyType::ACCURATE;
	body->defense = DefenseType::HIGH;
	body->dangerLevel = 8;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "wisp";
	body->minDamage = 5;
	body->maxDamage = 8;
	body->hp = 20;
	body->accuracy = AccuracyType::ACCURATE;
	body->defense = DefenseType::HIGH;
	body->dangerLevel = 11;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "wraith";
	body->minDamage = 6;
	body->maxDamage = 13;
	body->hp = 50;
	body->moveSpeed = MoveSpeedType::FAST;
	body->dangerLevel = 13;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "giant";
	body->minDamage = 10;
	body->maxDamage = 15;
	body->hp = 65;
	body->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
	body->dangerLevel = 15;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "lizard";
	body->minDamage = 7;
	body->maxDamage = 11;
	body->hp = 75;
	body->dangerLevel = 13;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "dar";
	body->minDamage = 3;
	body->maxDamage = 5;
	body->hp = 20;
	body->dangerLevel = 7;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "horseman";
	body->minDamage = 4;
	body->maxDamage = 8;
	body->hp = 35;
	body->moveSpeed = MoveSpeedType::FAST;
	body->dangerLevel = 10;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "worm";
	body->minDamage = 18;
	body->maxDamage = 22;
	body->hp = 80;
	body->moveSpeed = MoveSpeedType::SLOW;
	body->attackSpeed = AttackSpeedType::SLOW;
	body->dangerLevel = 13;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "fury";
	body->minDamage = 6;
	body->maxDamage = 11;
	body->hp = 20;
	body->moveSpeed = MoveSpeedType::FAST;
	body->accuracy = AccuracyType::ACCURATE;
	body->defense = DefenseType::HIGH;
	body->dangerLevel = 14;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "horror";
	body->minDamage = 25;
	body->maxDamage = 35;
	body->hp = 120;
	body->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
	body->dangerLevel = 28;
	bodies.push_back(body);

	body = new Body();
	body->baseName = "dragon";
	body->minDamage = 25;
	body->maxDamage = 50;
	body->hp = 150;
	body->dangerLevel = 28;
	bodies.push_back(body);
}

