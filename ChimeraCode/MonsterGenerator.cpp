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
#include <iostream>

MonsterGenerator::MonsterGenerator() {

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

std::vector<ChimeraMonster *> &MonsterGenerator::getMonsters() {
    return this->monsters;
}

std::vector<Horde *> &MonsterGenerator::getHordes() {
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
    int mookGroupHordes = 2;
    int kamikazeMonstersCount = 3;
    int thiefMonstersCount = 2;
    
    int roll = rand_range(0, 100);
    if (roll < 33) {
        mookGroupHordes += 1;
    } else if (roll < 33) {
        kamikazeMonstersCount += 1;
    } else {
        thiefMonstersCount += 1;
    }

    int specialistOnlyMook = rand_range(1, mookCount / 2); // index of the "dar", with no base type
    int specialistMookClasses = rand_range(3, 4);
    Body *specialistMookBody = NULL;
    std::vector<std::reference_wrapper<ChimeraMonster>> fodderMonsters;
    std::vector<std::reference_wrapper<ChimeraMonster>> mookMonsters;

    // Step 2: Generate the fodder
    for (int i = 0; i < fodderCount; i += 1) {
        Body *body = matchingBody([](const Body *body) {
            return body->dangerLevel <= 3 && !(body->genFlags & (GenerateFlag::KAMIKAZE | GenerateFlag::THIEVING));
        });
        if (body == NULL) {
            continue;
        }
        ChimeraMonster &monster = this->newMonster(*body);
        fodderMonsters.push_back(monster);
    }

    // Step 3: Generate the vanilla mooks
    for (int i = 0; i < mookCount; i += 1) {
        Body *body = matchingBody([maxMookDL, mookCount, i](const Body *body) {
            if (!(body->flags & GenerateFlag::SUPPORTS_CLASS) && !rand_percent(50)) {
                return false;
            }
            if (body->genFlags & (GenerateFlag::KAMIKAZE | GenerateFlag::THIEVING)) {
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
            specialistMookBody->inUse = true;
        } else {
            ChimeraMonster &monster = this->newMonster(*body);
            mookMonsters.push_back(monster);
        }
    }

    // Step 4: Fodder hordes
    for (ChimeraMonster &monster : fodderMonsters) {
        Horde &horde = this->newHorde(monster);
        horde.purpose = HordePurposeType::FODDER;
    }

    // Step 5: Fodder group hordes
    int j = 0;
    for (ChimeraMonster &monster : fodderMonsters) {
        if (j >= fodderGroupHordes) {
            break;
        }
        j += 1;
        Horde &horde = this->newHorde(monster);
        horde.addMember(monster, 1, 2);
    }

    // Step 6: Mook hordes
    for (ChimeraMonster &monster : mookMonsters) {
        this->newHorde(monster);
    }

    // Step 7: Mook group hordes
    j = 0;
    for (ChimeraMonster &monster : mookMonsters) {
        if (j >= mookGroupHordes) {
            break;
        }
        j += 1;
        Horde &horde = this->newHorde(monster);
        horde.addMember(monster, 2, 2);
    }

    // Step 8: Turn the "specialist" mook into its classes
    if (specialistMookBody != NULL) {
        std::vector<std::reference_wrapper<ChimeraMonster>> specialistMooks;
        ChimeraMonster genericSpecialistMook = ChimeraMonster(*specialistMookBody);
        for (int i = 0; i < specialistMookClasses; i += 1) {
            Ability *ability = matchingAbility([genericSpecialistMook](const Ability *ability) {
                return ability->validForMonster(genericSpecialistMook);
                return true;
            });
            if (ability != NULL) {
                ChimeraMonster &monster = this->newMonster(*specialistMookBody);
                monster.applyAbility(*ability);
                specialistMooks.push_back(monster);
            }
            genericSpecialistMook.flags |= GenerateFlag::PACK_MEMBER;
        }
        j = 0;
        for (ChimeraMonster &monster : specialistMooks) {
            Horde &horde = this->newHorde(monster);
            j += 1;
            for (unsigned int k = j; k < specialistMooks.size(); k += 1) {
                ChimeraMonster &additional = specialistMooks[k];
                horde.addMember(additional, 1, rand_range(1, 2));
            }
            if (horde.memberCount() == 1 && rand_percent(50)) {
                horde.addMember(monster, 0, rand_range(1, 2));
            }
        }
    }

    // Step 9: variations on the mooks
    for (ChimeraMonster &mook : mookMonsters) {
        int choice = rand_range(0, 3);
        if (choice == 0 || choice == 1 || choice == 2) {
            // a mutant...
            ChimeraMonster &monster = this->newMonster(mook);
            monster.flags |= GenerateFlag::PACK_MEMBER;
            Ability *ability = matchingAbility([monster](const Ability *ability) {
                return ability->validForMonster(monster);
            });
            if (ability == NULL) {
                continue;
            }
            monster.applyAbility(*ability);
            this->newHorde(monster);

            if (choice == 1) {
                // and some followers?
                Horde &horde = this->newHorde(monster);
                horde.addMember(mook, 2, rand_range(2, 4));
            } else if (choice == 2) {
                // two mutant types and a hunting party
                ChimeraMonster &monster2 = this->newMonster(mook);
                monster2.flags |= GenerateFlag::PACK_MEMBER;

                Ability *ability2 = matchingAbility([monster2](const Ability *ability) {
                    return ability->validForMonster(monster2);
                });
                if (ability2 == NULL) {
                    continue;
                }
                monster2.applyAbility(*ability2);

                Horde &horde = this->newHorde(monster2);
                horde.addMember(monster, 1, 1);

                Horde &horde2 = this->newHorde(monster2);
                horde2.addMember(monster, 1, 1);
                horde2.addMember(mook, rand_range(1, 2), 2);
            }
        } else if (choice == 3) {
            // shit out of luck, we make someone new
            int minDL, maxDL;
            bool solo = rand_percent(50);
            if (solo) {
                maxDL = mook.dangerLevel + 1;
                minDL = mook.dangerLevel - 4;
            } else {
                maxDL = mook.dangerLevel - 2;
                minDL = mook.dangerLevel/2 - 1;
            }
            Body *body = matchingBody([minDL, maxDL, mook](const Body *body) {
                return body->dangerLevel <= maxDL && body->dangerLevel >= minDL;
            });
            if (body == NULL) {
                continue;
            }
            ChimeraMonster &monster = this->newMonster(*body);
            if (!solo) {
                monster.flags |= PACK_MEMBER;
            }
            Ability *ability = matchingAbility([monster](const Ability *ability) {
                return ability->validForMonster(monster);
            });
            if (ability == NULL) {
                continue;
            }
            monster.applyAbility(*ability);
            Horde &horde = this->newHorde(monster);

            if (!solo) {
                ChimeraMonster &monster2 = this->newMonster(*body);
                monster.flags |= PACK_MEMBER;
                Ability *ability = matchingAbility([monster2](const Ability *ability) {
                    return ability->validForMonster(monster2);
                });
                if (ability == NULL) {
                    continue;
                }
                monster2.applyAbility(*ability);
                horde.addMember(monster2, 1, 1);
            }
        }
    }
    
    // Step 10: Kamikaze fun
    std::vector<std::reference_wrapper<ChimeraMonster>> kamikazes;
    Body *kamikazeBody = matchingBody([](const Body *body) {
        return body->genFlags & KAMIKAZE;
    });
    for (int i = 0; i < kamikazeMonstersCount; i += 1) {
        ChimeraMonster &monster = this->newMonster(*kamikazeBody);
        Ability *burst = matchingAbility([i, monster, kamikazeMonstersCount, kamikazeBody](const Ability *ability) {
            if (!ability->validForMonster(monster)) {
                return false;
            }
            int minDL = i * (AMULET_LEVEL / kamikazeMonstersCount) - 1;
            int maxDL = (i+1) * (AMULET_LEVEL / kamikazeMonstersCount) + 4;
            int dl = (ability->dangerBoost + kamikazeBody->dangerLevel);
            return (ability->requiredFlags & GenerateFlag::BURSTS) > 0 && dl >= minDL && dl <= maxDL;
        });
        if (burst != NULL) {
            monster.applyAbility(*burst);
            kamikazes.push_back(monster);
            Horde &horde = this->newHorde(monster);
            horde.purpose = HordePurposeType::KAMIKAZE;
            horde.extraRange += (2 - i);
        }
    }
    int duplicateGroupIndex = rand_range(0, MAX(0, kamikazeMonstersCount - 1));
    if ((unsigned long)duplicateGroupIndex < kamikazes.size()) {
        ChimeraMonster &monster = kamikazes[duplicateGroupIndex];
        Horde &horde = this->newHorde(monster);
        horde.addMember(monster, 2, 2);
        horde.purpose = HordePurposeType::KAMIKAZE;
    }
    
    // Step 11: Thieves
    std::vector<std::reference_wrapper<ChimeraMonster>> thieves;
    for (int i = 0; i < thiefMonstersCount; i += 1) {
        ChimeraMonster *monster;
        if (rand_percent(50)) {
            Body *body = matchingBody([](const Body *body) {
                return body->genFlags & THIEVING;
            });
            monster = new ChimeraMonster(*body);
        } else {
            ChimeraMonster &mook = mookMonsters[rand_range(0, mookMonsters.size() - 3)];
            monster = new ChimeraMonster(mook);
        }
        monster->genFlags |= GenerateFlag::THIEVING;
        Ability *ability = matchingAbility([i, thiefMonstersCount, monster](const Ability *ability) {
            if (!ability->validForMonster(*monster)) {
                return false;
            }
            int minDL = i * (AMULET_LEVEL / thiefMonstersCount) - 3;
            int maxDL = (i+1) * (AMULET_LEVEL / thiefMonstersCount) + 1;
            int dl = (ability->dangerBoost + monster->dangerLevel);
            return (ability->requiredFlags & GenerateFlag::THIEVING) > 0  && dl >= minDL && dl <= maxDL;
        });
        if (ability == NULL) {
            delete monster;
            i -= 1;
            continue;
        }
        monster->applyAbility(*ability);
        this->monsters.push_back(monster);
        thieves.push_back(*monster);
        Horde &horde = this->newHorde(*monster);
        horde.purpose = HordePurposeType::THIEF;
        horde.extraRange = -i;
    }
    duplicateGroupIndex = rand_range(0, MAX(0, thiefMonstersCount - 1));
    if ((unsigned long)duplicateGroupIndex < thieves.size()) {
        ChimeraMonster &monster = thieves[duplicateGroupIndex];
        Horde &horde = this->newHorde(monster);
        horde.addMember(monster, 2, 3);
        horde.purpose = HordePurposeType::THIEF;
        horde.extraRange = -1;
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

    for (int i = 0; i < 100; i += 1) {
        int index = rand_range(0, passing.size() - 1);
        Body *body = passing[index];
        if (rand_percent(body->rarityPercent)) {
            return body;
        }
    }
    return NULL;
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
    
    for (int i = 0; i < 100; i += 1) {
        int index = rand_range(0, passing.size() - 1);
        Ability *ability = passing[index];
        if (rand_percent(ability->rarityPercent)) {
            return ability;
        }
    }
    return NULL;
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

ChimeraMonster &MonsterGenerator::newMonster(Body &body) {
    ChimeraMonster *monster = new ChimeraMonster(body);
    this->monsters.push_back(monster);
    return *monster;
}

ChimeraMonster &MonsterGenerator::newMonster(const ChimeraMonster &baseMonster) {
    ChimeraMonster *monster = new ChimeraMonster(baseMonster);
    this->monsters.push_back(monster);
    return *monster;
}

Horde &MonsterGenerator::newHorde(const ChimeraMonster &monster) {
    Horde *horde = new Horde(monster);
    this->hordes.push_back(horde);
    return *horde;
}
