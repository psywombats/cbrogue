/*
 * MonsterGenerator.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "MonsterGenerator.h"
#include "ChimeraMonster.h"
#include "Body.h"
#include "Horde.h"
#include <vector>

MonsterGenerator::MonsterGenerator() {
	this->monsters = std::list<ChimeraMonster *>();
	this->bodies = std::list<Body *>();
	this->hordes = std::list<Horde *>();
	this->fodderMonsters = std::list<std::reference_wrapper<ChimeraMonster>>();
}

MonsterGenerator::~MonsterGenerator() {
	for (Body *body : this->bodies) {
		delete body;
	}
	for (ChimeraMonster *monster : this->monsters) {
		delete monster;
	}
	for (Horde *horde : this->hordes) {
		delete horde;
	}
}

std::list<ChimeraMonster *> &MonsterGenerator::getMonsters() {
	return this->monsters;
}

std::list<Horde *> &MonsterGenerator::getHordes() {
	return this->hordes;
}

void MonsterGenerator::generate() {
	this->bodies = Body::loadBodies();

	// Step 1: Let's put together some constants
	int fodderCount = 3;
	int mookCount = rand_range(6, 7);
	int maxMookDL = AMULET_LEVEL + 5;
	int fodderGroupHordes = 1;
	int mookGroupHordes = 3;

	// Step 2: Generate the fodder
	for (int i = 0; i < fodderCount; i += 1) {
		Body *body = matchingBody([](const Body *body) {
			return body->dangerLevel <= 3;
		});
		if (body == NULL) {
			continue;
		}
		ChimeraMonster *monster = new ChimeraMonster(*body);
		this->monsters.push_back(monster);
		this->fodderMonsters.push_back(*monster);
	}

	// Step 3: Generate the vanilla mooks
	for (int i = 0; i < mookCount; i += 1) {
		Body *body = matchingBody([maxMookDL, mookCount, i](const Body *body) {
			int dl = body->dangerLevel;
			return dl >= i * (maxMookDL / mookCount) && dl <= (i + 1) * (maxMookDL / mookCount);
		});
		if (body == NULL) {
			continue;
		}
		ChimeraMonster *monster = new ChimeraMonster(*body);
		this->monsters.push_back(monster);
		this->mookMonsters.push_back(*monster);
	}

	// Step 4: Fodder hordes
	for (ChimeraMonster &monster : this->fodderMonsters) {
		Horde *horde = new Horde(monster);
		horde->purpose = HordePurposeType::FODDER;
		this->hordes.push_back(horde);
	}

	// Step 5: Fodder group hordes
	int j = 0;
	for (ChimeraMonster &monster : this->fodderMonsters) {
		if (j >= fodderGroupHordes) {
			break;
		}
		j += 1;
		Horde *horde = new Horde(monster);
		horde->addMember(monster, 1, 2);
		this->hordes.push_back(horde);
	}

	// Step 6: Mook hordes
	for (ChimeraMonster &monster : this->mookMonsters) {
		Horde *horde = new Horde(monster);
		this->hordes.push_back(horde);
	}

	// Step 7: Mook group hordes
	j = 0;
	for (ChimeraMonster &monster : this->mookMonsters) {
		if (j >= mookGroupHordes) {
			break;
		}
		j += 1;
		Horde *horde = new Horde(monster);
		horde->addMember(monster, 2, 2);
		this->hordes.push_back(horde);
	}

	std::string report = debugReport();
	printf(report.c_str());
	return;
}

Body *MonsterGenerator::matchingBody(const std::function<bool(const Body *)>& filter) {
	// TODO: this is a not very efficient solution at all
	std::vector<Body *> passing = std::vector<Body *>();
	for (Body *body : this->bodies) {
		if (filter(body) && !body->inUse) {
			passing.push_back(body);
		}
	}

	if (passing.size() == 0) {
		return NULL;
	}

	int index = rand_range(0, passing.size() - 1);
	return passing[index];
}

std::string MonsterGenerator::debugReport() const {
	std::string report = "\n==== MONSTERS ====\n\n";

	for (ChimeraMonster *monster : this->monsters) {
		report += monster->debugReport() + "\n";
	}
	report += "\n==== HORDES ====\n\n";
	for (Horde *horde : this->hordes) {
		report += horde->debugReport() + "\n";
	}

	return report;
}
