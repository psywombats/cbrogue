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
#include "Ability.h"
#include <vector>

MonsterGenerator::MonsterGenerator() {
	this->monsters = std::list<ChimeraMonster *>();
	this->bodies = std::list<Body *>();
	this->abilities = std::list<Ability *>();
	this->hordes = std::list<Horde *>();
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
	this->abilities = Ability::loadModifierAbilities();

	// Step 1: Let's put together some constants
	int fodderCount = 3;
	int mookCount = rand_range(6, 7);
	int maxMookDL = AMULET_LEVEL + 5;
	int fodderGroupHordes = 1;
	int mookGroupHordes = 3;

	int specialistOnlyMook = rand_range(1, mookCount / 2); // index of the "dar", with no base type
	int specialistMookClasses = rand_range(2, 4);
	Body *specialistMookBody;
	std::list<std::reference_wrapper<ChimeraMonster>> fodderMonsters;
	std::list<std::reference_wrapper<ChimeraMonster>> mookMonsters;

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
		fodderMonsters.push_back(*monster);
	}

	// Step 3: Generate the vanilla mooks
	for (int i = 0; i < mookCount; i += 1) {
		Body *body = matchingBody([maxMookDL, mookCount, i](const Body *body) {
			if (!(body->flags & GenerateFlag::SUPPORTS_CLASS) && !rand_percent(50)) {
				return false;
			}
			int dl = body->dangerLevel;
			return dl >= i * (maxMookDL / mookCount) && dl <= (i + 1) * (maxMookDL / mookCount);
		});
		if (body == NULL) {
			continue;
		}
		if (i == specialistOnlyMook) {
			specialistMookBody = body;
		} else {
			ChimeraMonster *monster = new ChimeraMonster(*body);
			this->monsters.push_back(monster);
			mookMonsters.push_back(*monster);
		}
	}

	// Step 4: Fodder hordes
	for (ChimeraMonster &monster : fodderMonsters) {
		Horde *horde = new Horde(monster);
		horde->purpose = HordePurposeType::FODDER;
		this->hordes.push_back(horde);
	}

	// Step 5: Fodder group hordes
	int j = 0;
	for (ChimeraMonster &monster : fodderMonsters) {
		if (j >= fodderGroupHordes) {
			break;
		}
		j += 1;
		Horde *horde = new Horde(monster);
		horde->addMember(monster, 1, 2);
		this->hordes.push_back(horde);
	}

	// Step 6: Mook hordes
	for (ChimeraMonster &monster : mookMonsters) {
		Horde *horde = new Horde(monster);
		this->hordes.push_back(horde);
	}

	// Step 7: Mook group hordes
	j = 0;
	for (ChimeraMonster &monster : mookMonsters) {
		if (j >= mookGroupHordes) {
			break;
		}
		j += 1;
		Horde *horde = new Horde(monster);
		horde->addMember(monster, 2, 2);
		this->hordes.push_back(horde);
	}

	// Step 8: Turn the "specialist" mook into its classes
	std::vector<std::reference_wrapper<ChimeraMonster>> specialistMooks;
	ChimeraMonster genericSpecialistMook = ChimeraMonster(*specialistMookBody);
	for (int i = 0; i < specialistMookClasses; i += 1) {
		Ability *ability = matchingAbility([genericSpecialistMook](const Ability *ability) {
			return ability->validForMonster(genericSpecialistMook);
		});
		if (ability != NULL) {
			ChimeraMonster *monster = new ChimeraMonster(*specialistMookBody);
			this->monsters.push_back(monster);
			monster->applyAbility(*ability);
			specialistMooks.push_back(*monster);
		}
	}
	j = 0;
	for (ChimeraMonster &monster : specialistMooks) {
		Horde *horde = new Horde(monster);
		this->hordes.push_back(horde);
		j += 1;
		for (unsigned int k = j; k < specialistMooks.size(); k += 1) {
			ChimeraMonster &additional = specialistMooks[k];
			horde->addMember(additional, 1, rand_range(1, 2));
		}
		if (horde->memberCount() == 1 && rand_percent(50)) {
			horde->addMember(monster, 0, rand_range(1, 2));
		}
	}

	// Step 8: variations on the mooks
	for (ChimeraMonster &mook : mookMonsters) {
		int choice = rand_range(0, 3);
		if (choice == 0 || choice == 1 || choice == 2) {
			// a mutant...
			ChimeraMonster *monster = new ChimeraMonster(mook);
			monster->flags |= GenerateFlag::PACK_MEMBER;
			Ability *ability = matchingAbility([monster](const Ability *ability) {
				return ability->validForMonster(*monster);
			});
			if (ability == NULL) {
				continue;
			}
			monster->applyAbility(*ability);
			this->monsters.push_back(monster);
			Horde *horde = new Horde(*monster);
			this->hordes.push_back(horde);

			if (choice == 1) {
				// and some followers?
				Horde *horde = new Horde(*monster);
				this->hordes.push_back(horde);
				horde->addMember(mook, 2, rand_range(2, 4));
			} else if (choice == 2) {
				// two mutant types and a hunting party
				ChimeraMonster *monster2 = new ChimeraMonster(mook);
				monster->flags |= GenerateFlag::PACK_MEMBER;
				Ability *ability2 = matchingAbility([monster2](const Ability *ability) {
					return ability->validForMonster(*monster2);
				});
				if (ability2 == NULL) {
					continue;
				}
				monster2->applyAbility(*ability2);
				this->monsters.push_back(monster2);

				Horde *horde = new Horde(*monster2);
				this->hordes.push_back(horde);
				horde->addMember(*monster, 1, 1);

				Horde *horde2 = new Horde(*monster2);
				this->hordes.push_back(horde2);
				horde2->addMember(*monster, 1, 1);
				horde2->addMember(mook, rand_range(1, 2), 2);
			}
		} else if (choice == 3) {
			// shit out of luck
		}
	}

	std::string report = debugReport();
	printf(report.c_str());
	printf("\n");
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

Ability *MonsterGenerator::matchingAbility(const std::function<bool(const Ability *)>& filter) {
	// TODO: same garbage
	std::vector<Ability *> passing = std::vector<Ability *>();
	for (Ability *ability : this->abilities) {
		if (filter(ability) && !ability->inUse) {
			passing.push_back(ability);
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
