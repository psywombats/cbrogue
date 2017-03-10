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
    int maxMookDL = AMULET_LEVEL + 3;
    int maxAquaDL = AMULET_LEVEL + 6;
    int maxThiefDL = AMULET_LEVEL - 3;
    int fodderGroupHordes = 1;
    int kamikazeMonstersCount = 3;
    int thiefMonstersCount = 2;
    int aquaMonstersCount = 3;
    int totemsCount = 2;
    int turretsCount = 4;
    
    for (int i = 0; i < 2; i += 1) {
    int roll = rand_range(0, 100);
        if (roll < 33) {
            mookCount += 1;
        } else if (roll < 50) {
            turretsCount += 1;
        } else if (roll < 65) {
            kamikazeMonstersCount += 1;
        } else if (roll < 80) {
            thiefMonstersCount += 1;
        } else if (roll < 92) {
            totemsCount += 1;
        } else {
            aquaMonstersCount += 1;
        }
    }

    int specialistOnlyMook = rand_range(1, mookCount / 2); // index of the "dar", with no base type
    int specialistMookClasses = rand_range(3, 4);
    Body *specialistMookBody = NULL;
    std::vector<std::reference_wrapper<ChimeraMonster>> fodderMonsters;
    std::vector<std::reference_wrapper<ChimeraMonster>> mookMonsters;
    std::vector<Body *> mookBodies;
    std::vector<std::vector<std::reference_wrapper<ChimeraMonster>>> mookMinions;

    // Step 2: Generate the fodder
    for (int i = 0; i < fodderCount; i += 1) {
        Body *body = matchingBody([](const Body *body) {
            return body->dangerLevel <= 3 && !(body->genFlags & (GF_KAMIKAZE | GF_THIEVING_ONLY | GF_AQUATIC_ONLY | GF_TOTEM | GF_TURRET));
        });
        if (body == NULL) {
            continue;
        }
        ChimeraMonster &monster = this->newMonster(*body);
        monster.genFlags |= GF_MOOKISH;
        fodderMonsters.push_back(monster);
    }

    // Step 3: Generate the vanilla mooks
    for (int i = 0; i < mookCount; i += 1) {
        Body *body = matchingBody([maxMookDL, mookCount, i, specialistOnlyMook](const Body *body) {
            if (body->genFlags & (GF_KAMIKAZE | GF_THIEVING_ONLY | GF_AQUATIC_ONLY | GF_TOTEM | GF_TURRET)) {
                return false;
            }
            if (i == specialistOnlyMook && !(body->genFlags & GF_SUPPORTS_CLASS)) {
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
            monster.genFlags |= GF_MOOKISH;
            mookMonsters.push_back(monster);
            mookBodies.push_back(body);
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
    for (ChimeraMonster &monster : mookMonsters) {
        if (rand_percent(75 - (monster.dangerLevel * 2))) {
            Horde &horde = this->newHorde(monster);
            horde.addMember(monster, 2, rand_range(2, 3));
        }
    }

    // Step 8: Turn the "specialist" mook into its classes
    if (specialistMookBody != NULL) {
        std::vector<std::reference_wrapper<ChimeraMonster>> specialistMooks;
        specialistMookBody->singleUse = false;
        ChimeraMonster genericSpecialistMook = ChimeraMonster(*specialistMookBody);
        genericSpecialistMook.genFlags |= GF_MOOKISH;
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
            genericSpecialistMook.genFlags |= GF_PACK_MEMBER;
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
        std::vector<std::reference_wrapper<ChimeraMonster>> mookSet;
        mookSet.push_back(mook);
        int choice = rand_range(0, 3);
        if (choice == 0 || choice == 1 || choice == 2) {
            // a mutant...
            ChimeraMonster &monster = this->newMonster(mook);
            monster.genFlags |= GF_PACK_MEMBER;
            Ability *ability = matchingAbility([monster](const Ability *ability) {
                return ability->validForMonster(monster);
            });
            if (ability == NULL) {
                continue;
            }
            monster.applyAbility(*ability);
            mookSet.push_back(monster);
            this->newHorde(monster);

            if (choice == 1) {
                // and some followers?
                Horde &horde = this->newHorde(monster);
                horde.addMember(mook, 2, rand_range(2, 4));
            } else if (choice == 2) {
                // two mutant types and a hunting party
                ChimeraMonster &monster2 = this->newMonster(mook);
                monster2.genFlags |= GF_PACK_MEMBER;

                Ability *ability2 = matchingAbility([monster2](const Ability *ability) {
                    return ability->validForMonster(monster2);
                });
                if (ability2 == NULL) {
                    continue;
                }
                monster2.applyAbility(*ability2);
                mookSet.push_back(monster2);

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
                monster.genFlags |= GF_PACK_MEMBER;
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
                monster.genFlags |= GF_PACK_MEMBER;
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
        if (mookSet.size() > 0) {
            mookMinions.push_back(mookSet);
        }
    }
    
    // Step 10: Kamikaze fun
    std::vector<std::reference_wrapper<ChimeraMonster>> kamikazes;
    Body *kamikazeBody = matchingBody([](const Body *body) {
        return body->genFlags & GF_KAMIKAZE;
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
            return (ability->requiredFlags & GF_BURSTS) > 0 && dl >= minDL && dl <= maxDL;
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
        Body *body;
        if (rand_percent(80)) {
            body = matchingBody([](const Body *body) {
                return body->genFlags & GF_THIEVING;
            });
            if (body == NULL) {
                continue;
            }
        } else {
            body = mookBodies[rand_range(0, mookBodies.size() - 4)];
            body->singleUse = false;
            monster = new ChimeraMonster(mook.body, mook.name);
            mook.body.singleUse = true;
        }
        monster->genFlags |= GF_THIEVING;
        Ability *ability = matchingAbility([i, thiefMonstersCount, monster, maxThiefDL](const Ability *ability) {
            if (!ability->validForMonster(*monster)) {
                return false;
            }
            int minDL = i * (maxThiefDL / thiefMonstersCount) - 3;
            int maxDL = (i+1) * (maxThiefDL / thiefMonstersCount);
            int dl = (ability->dangerBoost + monster->dangerLevel);
            return (ability->requiredFlags & GF_THIEVING) > 0  && dl >= minDL && dl <= maxDL;
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
    
    // Step 12: Aqua patrols
    for (int i = 0; i < aquaMonstersCount; i += 1) {
        int minDL = i * (maxAquaDL / aquaMonstersCount) - 3;
        int maxDL = (i+1) * (maxAquaDL / aquaMonstersCount) + 1;
        if (rand_percent(80)) {
            // custom aqua
            Body *body = matchingBody([i, minDL, maxDL](const Body *body) {
                return (body->genFlags & GF_AQUATIC_ONLY)  && body->dangerLevel >= minDL && body->dangerLevel <= maxDL;
            });
            if (body == NULL) {
                continue;
            }
            ChimeraMonster &monster = this->newMonster(*body);
            monster.genFlags |= GF_MOOKISH;
            Horde &horde = this->newHorde(monster);
            horde.purpose = HordePurposeType::AQUA;
            horde.extraRange = MAX(0, (3-i));
            
            if (rand_percent(75)) {
                // let's make a group for it
                if (rand_percent(75)) {
                    // a simple group
                    Horde &horde = this->newHorde(monster);
                    horde.addMember(monster, 2, 4);
                    horde.extraRange = 1;
                } else {
                    // a fancy group
                    monster.body.singleUse = false;
                    ChimeraMonster *soloMonster = new ChimeraMonster(monster.body, monster.name);
                    ChimeraMonster *groupMonster = new ChimeraMonster(monster.body, monster.name);
                    monster.body.singleUse = true;
                    groupMonster->genFlags |= GF_PACK_MEMBER;
                    Ability *soloAbility = matchingAbility([soloMonster](const Ability *ability) {
                        if (!(ability->requiredFlags & GF_AQUATIC)) {
                            if (rand_percent(50)) return false;
                        }
                        return ability->validForMonster(*soloMonster);
                    });
                    Ability *groupAbility = matchingAbility([groupMonster](const Ability *ability) {
                        if (!(ability->requiredFlags & GF_AQUATIC)) {
                            if (rand_percent(50)) return false;
                        }
                        return ability->validForMonster(*groupMonster);
                    });
                    if (soloAbility == NULL || groupAbility == NULL) {
                        delete groupMonster;
                        delete soloMonster;
                        continue;
                    }
                    this->monsters.push_back(groupMonster);
                    this->monsters.push_back(soloMonster);
                    groupMonster->applyAbility(*groupAbility);
                    soloMonster->applyAbility(*soloAbility);
                    Horde &monoHorde = this->newHorde(*soloMonster);
                    monoHorde.addMember(*soloMonster, rand_range(1, 2), 2);
                    monoHorde.purpose = HordePurposeType::AQUA;
                    Horde &groupHorde = this->newHorde(*groupMonster);
                    groupHorde.addMember(*soloMonster, 1, 2);
                    groupHorde.addMember(monster, 1, 2);
                    groupHorde.purpose = HordePurposeType::AQUA;
                }
            } else {
                // let's make a variant of it
                ChimeraMonster &variantMonster = this->newMonster(monster);
                Ability *ability = matchingAbility([variantMonster](const Ability *ability) {
                    if (!(ability->requiredFlags & GF_AQUATIC)) {
                        if (rand_percent(50)) return false;
                    }
                    return ability->validForMonster(variantMonster);
                });
                if (ability != NULL) {
                    variantMonster.applyAbility(*ability);
                }
                Horde &groupHorde = this->newHorde(variantMonster);
                groupHorde.addMember(monster, 0, 2);
                groupHorde.extraRange = 1;
            }
        } else {
            // convert a monster
            // if possible, use a mook
            minDL -= 2;
            maxDL += 1;
            ChimeraMonster *monster = NULL;
            for (ChimeraMonster &mook : mookMonsters) {
                if (!(mook.genFlags & GF_AQUATIC)) {
                    continue;
                }
                if (mook.dangerLevel < minDL || mook.dangerLevel > maxDL) {
                    continue;
                }
                mook.body.singleUse = false;
                monster = new ChimeraMonster(mook.body, mook.name);
                mook.body.singleUse = true;
                break;
            }
            if (monster == NULL) {
                // no mook available, let's find another suitable mob
                minDL += 3;
                Body *body = matchingBody([minDL, maxDL](const Body *body) {
                    if (body->dangerLevel > minDL || body->dangerLevel < maxDL) {
                        return false;
                    }
                    return (body->genFlags & GF_AQUATIC) > 0;
                });
                if (body == NULL) {
                    i -= 1;
                    continue;
                }
                monster = new ChimeraMonster(*body);
            }
            if (monster == NULL) {
                i -= 1;
                continue;
            }
            Ability *ability = matchingAbility([monster](const Ability *ability) {
                return ability->validForMonster(*monster) && (ability->requiredFlags & GF_AQUATIC);
            });
            if (ability == NULL) {
                i -= 1;
                delete monster;
                continue;
            }
            monster->applyAbility(*ability);
            this->monsters.push_back(monster);
            Horde &horde = this->newHorde(*monster);
            horde.purpose = HordePurposeType::AQUA;
            horde.extraRange = 1;
        }
    }
    
    // Step 13: Totems
    j = 0;
    for (std::vector<std::reference_wrapper<ChimeraMonster>> &mookSet : mookMinions) {
        if (j >= totemsCount) {
            break;
        }
        ChimeraMonster &mook = mookSet[0];
        Body *body = matchingBody([mook](const Body *body) {
            return body->genFlags == ((mook.genFlags & body->genFlags) | GF_TOTEM);
        });
        if (body == NULL) {
            continue;
        }
        j += 1;
        ChimeraMonster &totem = this->newMonster(*body);
        totem.baseMonsterName = mook.name;
        totem.genFlags |= mook.genFlags;
        totem.displayColor = mook.displayColor;
        if (mook.dangerLevel > 10) {
            totem.hp += 40;
        }
        if (mook.dangerLevel > 20) {
            totem.hp += 30;
        }
        Ability *ability = matchingAbility([totem](const Ability *ability) {
            return ability->validForMonster(totem) && (ability->requiredFlags & GF_TOTEM);
        });
        totem.applyAbility(*ability);
        
        // basic group
        Horde &baseHorde = this->newHorde(totem);
        baseHorde.purpose = HordePurposeType::TOTEM;
        baseHorde.addMember(mook, 2, 4);
        
        if (rand_percent(50) && mookSet.size() > 1) {
            // colony
            Horde &colonyHorde = this->newHorde(totem);
            colonyHorde.purpose = HordePurposeType::TOTEM;
            colonyHorde.addMember(totem, 1, 2);
            colonyHorde.addMember(mook, 3, 6);
            colonyHorde.addMember(mookSet[1], 1, rand_range(1, 2));
        }
        if (mookSet.size() > 1) {
            // group
            Horde &groupHorde = this->newHorde(totem);
            groupHorde.purpose = HordePurposeType::TOTEM;
            groupHorde.addMember(mook, 2, 3);
            groupHorde.addMember(mookSet[1], 1, 2);
        }
        if (mookSet.size() > 2) {
            // big group
            Horde &groupHorde = this->newHorde(totem);
            groupHorde.purpose = HordePurposeType::TOTEM;
            groupHorde.addMember(mook, 2, 4);
            groupHorde.addMember(mookSet[1], 0, rand_range(1, 2));
            groupHorde.addMember(mookSet[2], 1, rand_range(1, 2));
        }
    }
    
    // Step 14: Turrets
    for (int i = 0; i < turretsCount; i += 1) {
        Body *body = matchingBody([](const Body *body) {
            return (body->genFlags & GF_TURRET) > 0;
        });
        if (body == NULL) {
            continue;
        }
        ChimeraMonster *turret = new ChimeraMonster(*body);
        Ability *ability = matchingAbility([turret](const Ability *ability) {
            return ability->validForMonster(*turret) && (ability->requiredFlags && GF_TURRET);
        });
        if (ability == NULL) {
            delete turret;
            continue;
        }
        this->monsters.push_back(turret);
        turret->applyAbility(*ability);
        Horde &horde = this->newHorde(*turret);
        horde.purpose = HordePurposeType::TURRET;
        horde.extraRange = -1;
    }
    
    std::string report = debugReport();
    std::cout << report;
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
    bool wasSingleUse = baseMonster.body.singleUse;
    baseMonster.body.singleUse = false;
    ChimeraMonster *monster = new ChimeraMonster(baseMonster.body, baseMonster.name);
    this->monsters.push_back(monster);
    baseMonster.body.singleUse = wasSingleUse;
    return *monster;
}

Horde &MonsterGenerator::newHorde(const ChimeraMonster &monster) {
    Horde *horde = new Horde(monster);
    this->hordes.push_back(horde);
    return *horde;
}
