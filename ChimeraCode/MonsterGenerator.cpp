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
#include <algorithm>

MonsterGenerator::MonsterGenerator() :
        ratTrapMonsterId(0),
        warrenBossMonsterId(0),
        vampireBossMonsterId(0),
        wingedGuardianMonsterId(0),
        guardianMonsterId(0),
        sentinelMonsterId(0),
        wardenMonsterId(0),
        mirrorMonsterId(0),
        webberMonsterId(0),
        diggerMonsterId(0),
        sparkMonsterId(0),
        undeadMonsterId(0),
        invisibleMonsterId(0),
        spectralImageMonsterId(0),
        conjurationMonsterId(0),
        charmSummonMonsterId(0) {

}

MonsterGenerator::~MonsterGenerator() {
    for (Body *body : bodies) {
        delete body;
    }
    for (ChimeraMonster *monster : monsters) {
        delete monster;
    }
    for (Horde *horde : hordes) {
        delete horde;
    }
}

std::vector<ChimeraMonster *> &MonsterGenerator::getMonsters() {
    return monsters;
}

std::vector<Horde *> &MonsterGenerator::getHordes() {
    return hordes;
}

void MonsterGenerator::generate() {
    bodies = Body::loadBodies();
    abilities = Ability::loadModifierAbilities();

    // Step 1: Let's put together some constants
    int fodderCount = rand_range(3, 4);
    int mookCount = rand_range(8, 9);
    int maxMookDL = AMULET_LEVEL + 3;
    int maxAquaDL = AMULET_LEVEL + 6;
    int maxThiefDL = AMULET_LEVEL - 3;
    int fodderGroupHordes = 1;
    int kamikazeMonstersCount = 3;
    int thiefMonstersCount = 2;
    int aquaMonstersCount = 3;
    int totemsCount = 2;
    int turretsCount = 4;
    int bossCount = 4;
    int targetCaptives = 15;
    
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
    unsigned long weird = (GF_KAMIKAZE | GF_THIEVING_ONLY | GF_AQUATIC_ONLY | GF_TOTEM | GF_TURRET | GF_BOSS_ONLY | GF_CONJURATION);
    unsigned long guardFlag = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_FIRE |
                               MONST_IMMUNE_TO_WEAPONS | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 |
                               MONST_ALWAYS_USE_ABILITY | MONST_GETS_TURN_ON_ACTIVATION);
    Body *specialistMookBody = NULL;
    std::vector<std::reference_wrapper<ChimeraMonster>> fodderMonsters;
    std::vector<std::reference_wrapper<ChimeraMonster>> mookMonsters;
    std::vector<std::vector<std::reference_wrapper<ChimeraMonster>>> mookMinions;

    // Step 2: Generate the vanilla mooks
    for (int i = 0; i < mookCount; i += 1) {
        Body *body = matchingBody([weird, maxMookDL, mookCount, i, specialistOnlyMook](const Body *body) {
            if (body->genFlags & weird) {
                return false;
            }
            if (i == specialistOnlyMook && !((body->genFlags & GF_SUPPORTS_CLASS) || (body->genFlags & GF_NO_GROUPS))) {
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
            ChimeraMonster *monster = newMonster(*body);
            mookMonsters.push_back(*monster);
        }
    }
    
    // Step 3: Generate the fodder
    for (int i = 0; i < fodderCount; i += 1) {
        Body *body = matchingBody([weird](const Body *body) {
            return body->dangerLevel <= 3 && !(body->genFlags & weird);
        });
        if (body == NULL) {
            continue;
        }
        ChimeraMonster *monster = newMonster(*body);
        fodderMonsters.push_back(*monster);
        if (i == 0) {
            Ability *ability = matchingAbility([body](const Ability *ability) {
                return ability->isValidFor(*body) && ability->dangerBoost <= 2;
            });
            if (ability != NULL) {
                monster->dangerLevel -= 1;
                monster->applyAbility(*ability);
            }
        }
    }

    // Step 4: Fodder hordes
    for (ChimeraMonster &monster : fodderMonsters) {
        Horde &horde = newHorde(monster);
        horde.purpose = HordePurposeType::FODDER;
    }

    // Step 5: Fodder group hordes
    int j = 0;
    for (ChimeraMonster &monster : fodderMonsters) {
        if (j >= fodderGroupHordes) {
            break;
        }
        if (monster.genFlags & GF_NO_GROUPS) {
            continue;
        }
        j += 1;
        Horde &horde = newHorde(monster);
        horde.addMember(monster, 1, 2);
    }

    // Step 6: Mook hordes
    for (ChimeraMonster &monster : mookMonsters) {
        newHorde(monster);
    }

    // Step 7: Mook group hordes
    for (ChimeraMonster &monster : mookMonsters) {
        if (monster.genFlags & GF_NO_GROUPS) {
            continue;
        }
        if (rand_percent(70 - (monster.dangerLevel * 2))) {
            Horde &horde = newHorde(monster);
            horde.addMember(monster, 2, rand_range(2, 3));
        }
    }

    // Step 8: Turn the "specialist" mook into its classes
    if (specialistMookBody != NULL) {
        std::vector<std::reference_wrapper<ChimeraMonster>> specialistMooks;
        for (int i = 0; i < specialistMookClasses; i += 1) {
            Ability *ability = matchingAbility([specialistMookBody, i](const Ability *ability) {
                return ability->isValidFor(*specialistMookBody, (GF_PACK_MEMBER), NULL);
            });
            if (ability != NULL) {
                ChimeraMonster *monster = newMonster(*specialistMookBody);
                monster->baseMookName = specialistMookBody->baseName;
                monster->genFlags |= GF_PACK_MEMBER;
                monster->applyAbility(*ability);
                specialistMooks.push_back(*monster);
            }
        }
        j = 0;
        for (ChimeraMonster &monster : specialistMooks) {
            Horde &horde = newHorde(monster);
            j += 1;
            for (unsigned int k = j; k < specialistMooks.size(); k += 1) {
                ChimeraMonster &additional = specialistMooks[k];
                horde.addMember(additional, 1, rand_range(1, 2));
            }
            if (horde.memberCount() == 1) {
                horde.addMember(monster, 1, rand_range(1, 2));
            }
        }
        if (specialistMooks.size() > 0) {
            mookMinions.push_back(specialistMooks);
        }
    }

    // Step 9: variations on the mooks
    for (ChimeraMonster &mook : mookMonsters) {
        std::vector<std::reference_wrapper<ChimeraMonster>> mookSet;
        mookSet.push_back(mook);
        int choice = rand_range(0, 3);
        if (choice == 0 || choice == 1 || choice == 2) {
            // a mutant...
            ChimeraMonster *monster = newMonster(&mook);
            monster->genFlags |= GF_PACK_MEMBER;
            Ability *ability = matchingAbility([monster](const Ability *ability) {
                return ability->isValidForMonster(*monster);
            });
            if (ability == NULL) {
                continue;
            }
            monster->applyAbility(*ability);
            mookSet.push_back(*monster);
            newHorde(*monster);

            if (choice == 1 && !(monster->genFlags & GF_NO_GROUPS)) {
                // and some followers?
                Horde &horde = newHorde(*monster);
                horde.addMember(mook, 2, rand_range(2, 4));
            } else if (choice == 2 && !(monster->genFlags & GF_NO_GROUPS)) {
                // two mutant types and a hunting party
                ChimeraMonster *monster2 = newMonster(&mook);
                monster2->genFlags |= GF_PACK_MEMBER;
                
                Ability *ability2 = matchingAbility([monster2](const Ability *ability) {
                    return ability->isValidForMonster(*monster2);
                });
                if (ability2 == NULL) {
                    continue;
                }
                monster2->applyAbility(*ability2);
                mookSet.push_back(*monster2);
                
                if  (!(monster2->genFlags & GF_NO_GROUPS)) {
                    Horde &horde = newHorde(*monster2);
                    horde.addMember(*monster, 1, 1);

                    Horde &horde2 = newHorde(*monster2);
                    horde2.addMember(*monster, 1, 1);
                    horde2.addMember(mook, rand_range(1, 2), 2);
                }
            }
        } else if (choice == 3) {
            // shit out of luck, we make someone new
            int minDL, maxDL;
            std::vector<std::reference_wrapper<ChimeraMonster>> newSet;
            bool solo = rand_percent(50);
            if (solo) {
                maxDL = mook.dangerLevel + 1;
                minDL = mook.dangerLevel - 4;
            } else {
                maxDL = mook.dangerLevel - 2;
                minDL = mook.dangerLevel/2 - 1;
            }
            Body *body = matchingBody([minDL, maxDL, mook, weird, solo](const Body *body) {
                if (!solo && (body->genFlags & GF_NO_GROUPS)) {
                    return false;
                }
                return body->dangerLevel <= maxDL && body->dangerLevel >= minDL && !(body->genFlags & weird);
            });
            if (body == NULL) {
                continue;
            }
            ChimeraMonster *monster = newMonster(*body);
            Horde &horde = newHorde(*monster);
            newSet.push_back(*monster);
            if (!solo) {
                monster->genFlags |= GF_PACK_MEMBER;
            }
            Ability *ability = matchingAbility([monster](const Ability *ability) {
                return ability->isValidForMonster(*monster);
            });
            if (ability == NULL) {
                continue;
            }
            monster->applyAbility(*ability);
            monster->baseMookName = body->baseName;
            
            if (!solo) {
                ChimeraMonster *monster2 = newMonster(*body);
                monster2->genFlags |= GF_PACK_MEMBER;
                monster2->baseMookName = body->baseName;
                Ability *ability = matchingAbility([monster2](const Ability *ability) {
                    return ability->isValidForMonster(*monster2);
                });
                if (ability == NULL) {
                    continue;
                }
                monster2->applyAbility(*ability);
                horde.addMember(*monster2, 1, 1);
                newSet.push_back(*monster2);
            }
            if (newSet.size() > 0) {
                mookMinions.push_back(newSet);
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
        ChimeraMonster *monster = newMonster(*kamikazeBody);
        Ability *burst = matchingAbility([i, monster, kamikazeMonstersCount, kamikazeBody](const Ability *ability) {
            if (!ability->isValidForMonster(*monster)) {
                return false;
            }
            int minDL = i * (AMULET_LEVEL / kamikazeMonstersCount) - 1;
            int maxDL = (i+1) * (AMULET_LEVEL / kamikazeMonstersCount) + 4;
            int dl = (ability->dangerBoost + kamikazeBody->dangerLevel);
            return (ability->requiredFlags & GF_BURSTS) > 0 && dl >= minDL && dl <= maxDL;
        });
        if (burst != NULL) {
            monster->applyAbility(*burst);
            monster->baseMookName = kamikazeBody->baseName;
            kamikazes.push_back(*monster);
            Horde &horde = newHorde(*monster);
            horde.purpose = HordePurposeType::KAMIKAZE;
            horde.extraRange += (2 - i);
        }
    }
    int duplicateGroupIndex = rand_range(0, MAX(0, kamikazeMonstersCount - 1));
    if ((unsigned long)duplicateGroupIndex < kamikazes.size()) {
        ChimeraMonster &monster = kamikazes[duplicateGroupIndex];
        Horde &horde = newHorde(monster);
        horde.addMember(monster, 2, 2);
        horde.purpose = HordePurposeType::KAMIKAZE;
    }
    
    // Step 11: Thieves
    std::vector<std::reference_wrapper<ChimeraMonster>> thieves;
    int retries = 0;
    for (int i = 0; i < thiefMonstersCount; i += 1) {
        retries += 1;
        if (retries > 100) {
            retries = 0;
            break;
        }
        Body *body = NULL;
        ChimeraMonster *baseMook = NULL;
        if (rand_percent(80)) {
            body = matchingBody([](const Body *body) {
                return body->genFlags & GF_THIEVING;
            });
            if (body == NULL) {
                continue;
            }
        } else {
            baseMook = &mookMonsters[rand_range(0, mookMonsters.size() - 5)].get();
            body = &baseMook->body;
        }

        Ability *ability = matchingAbility([i, thiefMonstersCount, body, maxThiefDL, baseMook](const Ability *ability) {
            if (!ability->isValidFor(*body, GF_THIEVING, baseMook)) {
                return false;
            }
            int minDL = i * (maxThiefDL / thiefMonstersCount) - 3;
            int maxDL = (i+1) * (maxThiefDL / thiefMonstersCount) - 1;
            int dl = (ability->dangerBoost + body->dangerLevel);
            return (ability->requiredFlags & GF_THIEVING) > 0  && dl >= minDL && dl <= maxDL;
        });
        if (ability == NULL) {
            i -= 1;
            continue;
        }
        retries = 0;
        ChimeraMonster *monster = newMonster(*body);
        monster->baseMookName = body->baseName;
        monster->genFlags |= GF_THIEVING;
        monster->applyAbility(*ability);
        thieves.push_back(*monster);
        Horde &horde = newHorde(*monster);
        horde.purpose = HordePurposeType::THIEF;
        horde.extraRange = -i;
    }
    duplicateGroupIndex = rand_range(0, MAX(0, thiefMonstersCount - 1));
    if ((unsigned long)duplicateGroupIndex < thieves.size()) {
        ChimeraMonster &monster = thieves[duplicateGroupIndex];
        Horde &horde = newHorde(monster);
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
            ChimeraMonster *monster = newMonster(*body);
            Horde &horde = newHorde(*monster);
            horde.purpose = HordePurposeType::AQUA;
            horde.extraRange = MAX(0, (3-i));
            
            if (rand_percent(75) && !(body->genFlags & GF_NO_GROUPS)) {
                // let's make a group for it
                if (rand_percent(75)) {
                    // a simple group
                    Horde &horde = newHorde(*monster);
                    horde.addMember(*monster, 2, 4);
                    horde.extraRange = 1;
                } else {
                    // a fancy group
                    Ability *soloAbility = matchingAbility([monster](const Ability *ability) {
                        if (!(ability->requiredFlags & GF_AQUATIC)) {
                            if (rand_percent(50)) return false;
                        }
                        return ability->isValidForMonster(*monster);
                    });
                    Ability *groupAbility = matchingAbility([monster](const Ability *ability) {
                        if (!(ability->requiredFlags & GF_AQUATIC)) {
                            if (rand_percent(50)) return false;
                        }
                        return ability->isValidForMonster(*monster, GF_PACK_MEMBER);
                    });
                    if (soloAbility == NULL || groupAbility == NULL) {
                        continue;
                    }
                    ChimeraMonster *soloMonster = newMonster(monster);
                    ChimeraMonster *groupMonster = newMonster(monster);
                    groupMonster->genFlags |= GF_PACK_MEMBER;
                    groupMonster->applyAbility(*groupAbility);
                    soloMonster->applyAbility(*soloAbility);
                    Horde &monoHorde = newHorde(*soloMonster);
                    monoHorde.addMember(*soloMonster, rand_range(1, 2), 2);
                    monoHorde.purpose = HordePurposeType::AQUA;
                    Horde &groupHorde = newHorde(*groupMonster);
                    groupHorde.addMember(*soloMonster, 1, 2);
                    groupHorde.addMember(*monster, 1, 2);
                    groupHorde.purpose = HordePurposeType::AQUA;
                }
            } else {
                // let's make a variant of it
                ChimeraMonster *variantMonster = newMonster(monster);
                Ability *ability = matchingAbility([variantMonster](const Ability *ability) {
                    if (!(ability->requiredFlags & GF_AQUATIC)) {
                        if (rand_percent(50)) return false;
                    }
                    return ability->isValidForMonster(*variantMonster);
                });
                if (ability != NULL) {
                    variantMonster->applyAbility(*ability);
                }
                Horde &groupHorde = newHorde(*variantMonster);
                if (!(variantMonster->genFlags & GF_NO_GROUPS) && !(monster->genFlags & GF_NO_GROUPS)) {
                    groupHorde.addMember(*monster, 0, 2);
                    groupHorde.extraRange = 1;
                }
            }
        } else {
            // convert a monster
            // if possible, use a mook
            minDL -= 2;
            maxDL += 1;
            Body *body = NULL;
            ChimeraMonster *baseMook = NULL;
            for (ChimeraMonster &mook : mookMonsters) {
                if (!(mook.genFlags & GF_AQUATIC)) {
                    continue;
                }
                if (mook.dangerLevel < minDL || mook.dangerLevel > maxDL) {
                    continue;
                }
                break;
            }
            if (body == NULL) {
                // no mook available, let's find another suitable mob
                minDL += 3;
                Body *body = matchingBody([minDL, maxDL, weird](const Body *body) {
                    if (body->dangerLevel > minDL || body->dangerLevel < maxDL || (body->genFlags & weird)) {
                        return false;
                    }
                    return (body->genFlags & GF_AQUATIC) > 0;
                });
                if (body == NULL) {
                    i -= 1;
                    continue;
                }
            }
            if (body == NULL) {
                i -= 1;
                continue;
            }
            Ability *ability = matchingAbility([body, baseMook](const Ability *ability) {
                return ability->isValidFor(*body, 0, baseMook) && (ability->requiredFlags & GF_AQUATIC);
            });
            if (ability == NULL) {
                i -= 1;
                continue;
            }
            ChimeraMonster *monster = newMonster(*body);
            monster->applyAbility(*ability);
            Horde &horde = newHorde(*monster);
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
        if (mookSet[0].get().genFlags & GF_NO_GROUPS) {
            continue;
        }
        ChimeraMonster &mook = mookSet[0];
        Body *body = matchingBody([mook](const Body *body) {
            if (!(body->genFlags & GF_TOTEM)) {
                return false;
            }
            unsigned long reqMookFlags = body->genFlags & (~GF_TOTEM);
            return reqMookFlags == (mook.genFlags & reqMookFlags);
        });
        if (body == NULL) {
            continue;
        }
        j += 1;
        ChimeraMonster *totem = newMonster(*body);
        totem->baseMook = &mook;
        totem->genFlags |= mook.genFlags;
        totem->displayColor = mook.displayColor;
        if (mook.dangerLevel > 10) {
            totem->hp += 40;
        }
        if (mook.dangerLevel > 20) {
            totem->hp += 30;
        }
        Ability *ability = matchingAbility([totem, mook](const Ability *ability) {
            return ability->isValidForMonster(*totem) && (ability->requiredFlags & GF_TOTEM);
        });
        totem->applyAbility(*ability);
        
        // basic group
        Horde &baseHorde = newHorde(*totem);
        baseHorde.purpose = HordePurposeType::TOTEM;
        baseHorde.addMember(mook, 2, 4);
        
        if (rand_percent(50) && mookSet.size() > 1) {
            // colony
            Horde &colonyHorde = newHorde(*totem);
            colonyHorde.purpose = HordePurposeType::TOTEM;
            colonyHorde.addMember(*totem, 1, 2);
            colonyHorde.addMember(mook, 3, 6);
            colonyHorde.addMember(mookSet[1], 1, rand_range(1, 2));
        }
        if (mookSet.size() > 1) {
            // group
            Horde &groupHorde = newHorde(*totem);
            groupHorde.purpose = HordePurposeType::TOTEM;
            groupHorde.addMember(mook, 2, 3);
            groupHorde.addMember(mookSet[1], 1, 2);
        }
        if (mookSet.size() > 2) {
            // big group
            Horde &groupHorde = newHorde(*totem);
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
        Ability *ability = matchingAbility([body](const Ability *ability) {
            return ability->isValidFor(*body) && (ability->requiredFlags & GF_TURRET);
        });
        if (ability == NULL) {
            continue;
        }
        ChimeraMonster *turret = newMonster(*body);
        turret->applyAbility(*ability);
        Horde &horde = newHorde(*turret);
        horde.purpose = HordePurposeType::TURRET;
        horde.extraRange = -1;
    }
    
    // Step 15: Rat trap monster ID
    int lowestMonsterId = fodderMonsters[1].get().monsterId;
    int lowestDL = fodderMonsters[0].get().dangerLevel;
    for (ChimeraMonster &fodder : fodderMonsters) {
        if (fodder.dangerLevel < lowestDL) {
            lowestMonsterId = fodder.monsterId;
        }
    }
    ratTrapMonsterId = lowestMonsterId;
    
    // Step 16: "Vampire" boss
    Body *vampireBody = matchingBody([weird](const Body *body) {
        if (!(body->genFlags & GF_BOSSLIKE) && rand_percent(50)) {
            return false;
        }
        return !(body->genFlags & weird) && body->dangerLevel >= 15 && body->dangerLevel <= 22;
    });
    if (vampireBody != NULL) {
        Ability *vampireAbility = matchingAbility([vampireBody](const Ability *ability) {
            return (ability->abilFlags & MA_TRANSFERENCE) && ability->isValidFor(*vampireBody);
        });
        if (vampireBody != NULL && vampireAbility != NULL) {
            ChimeraMonster *vampire = newMonster(*vampireBody);
            vampire->applyAbility(*vampireAbility);
            vampireBossMonsterId = vampire->monsterId;
        }
    }
    
    // Step 17: Guardians
    Body *guardianBody = NULL;
    for (int i = 0; i < 100 && guardianBody == NULL; i += 1) {
        guardianBody = matchingBody([weird](const Body *body) {
            if (body->genFlags & weird) {
                return false;
            }
            if (!(body->genFlags & GF_ARMED)) {
                return false;
            }
            if (!(body->genFlags & GF_BOSSLIKE) && rand_percent(50)) {
                return false;
            }
            if (!(body->dangerLevel > 15) && rand_percent(50)) {
                return false;
            }
            return true;
        });
    }
    if (guardianBody != NULL) {
        guardianBody->abilFlags = 0;
        guardianBody->accuracy = AccuracyType::ACCURATE;
        guardianBody->baseColor = &white;
        guardianBody->baseChar = toupper(guardianBody->baseChar);
        guardianBody->defense = DefenseType::DEFENSELESS;
        guardianBody->flags = guardFlag;
        guardianBody->abilFlags = 0;
        guardianBody->flavor =  std::string("Guarding the room is a weathered stone statue of a $BASE carrying a $WEAPON, ") +
                                std::string("connected to the glowing glyphs on the floor by invisible strands of enchantment.");
        guardianBody->gender = GenderType::NONE;
        guardianBody->hp = 1000;
        guardianBody->light = NO_LIGHT;
        guardianBody->minDamage = 12;
        guardianBody->maxDamage = 17;
        guardianBody->periodicFeatureChance = 100;
        guardianBody->periodicFeature = DF_GUARDIAN_STEP;
        guardianBody->regenSpeed = RegenSpeedType::NONE;
        
        ChimeraMonster *guardian = newMonster(*guardianBody);
        guardianMonsterId = guardian->monsterId;
        guardian->baseMookName = guardianBody->baseName;
        guardian->namePrefix = "guardian";
        
        ChimeraMonster *wingedGuardian = newMonster(*guardianBody);
        wingedGuardianMonsterId = wingedGuardian->monsterId;
        wingedGuardian->baseMookName = guardianBody->baseName;
        wingedGuardian->namePrefix = "guardian";
        wingedGuardian->displayColor = &blue;
        wingedGuardian->bolts = {BOLT_BLINKING};
        wingedGuardian->feature = DF_SILENT_GLYPH_GLOW;
        
        ChimeraMonster *charmGuardian = newMonster(*guardianBody);
        charmSummonMonsterId = charmGuardian->monsterId;
        charmGuardian->flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS |
                                MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY);
        charmGuardian->baseMookName = guardianBody->baseName;
        charmGuardian->namePrefix = "guardian";
        charmGuardian->baseFlavor = "A spectral outline of a $BASE carrying a $WEAPON casts an ethereal light on its surroundings.";
        charmGuardian->damage.lowerBound = 5;
        charmGuardian->damage.upperBound = 12;
        charmGuardian->displayColor = &spectralImageColor;
        charmGuardian->feature = DF_NONE;
        charmGuardian->lightType = SPECTRAL_IMAGE_LIGHT;
    }
    
    // Step 18: Sentinels
    if (guardianBody != NULL) {
        Body *sentinelBody = NULL;
        for (int i = 0; i < 100 && sentinelBody == NULL; i += 1) {
            sentinelBody = matchingBody([weird](const Body *body) {
                if (body->genFlags & weird) {
                    return false;
                }
                if (!(body->genFlags & GF_WIZARDLY)) {
                    return false;
                }
                if (!(body->genFlags & GF_BOSSLIKE) && rand_percent(50)) {
                    return false;
                }
                if (!(body->dangerLevel > 15) && rand_percent(50)) {
                    return false;
                }
                return true;
            });
        }
        if (sentinelBody != NULL) {
            sentinelBody->abilFlags = 0;
            sentinelBody->accuracy = AccuracyType::ACCURATE;
            sentinelBody->baseColor = &sentinelColor;
            sentinelBody->baseChar = toupper(guardianBody->baseChar);
            sentinelBody->blood = DF_RUBBLE_BLOOD;
            sentinelBody->defense = DefenseType::DEFENSELESS;
            sentinelBody->flags = (MONST_TURRET | MONST_CAST_SPELLS_SLOWLY | MONST_DIES_IF_NEGATED);
            sentinelBody->flavor =  std::string("A crystal $BASE statue of indeterminable age, the $BASE sentinel holds aloft a $MAGIC that gleams with ") +
                                    std::string("ancient warding magic. Sentinels are always found in groups, and each will attempt to aid the others.");
            sentinelBody->gender = GenderType::NONE;
            sentinelBody->hp = 1000;
            sentinelBody->light = SENTINEL_LIGHT;
            sentinelBody->hp = 50;
            sentinelBody->minDamage = 0;
            sentinelBody->maxDamage = 0;
            sentinelBody->regenSpeed = RegenSpeedType::NONE;
            ChimeraMonster *sentinel = newMonster(*sentinelBody);
            sentinelMonsterId = sentinel->monsterId;
            sentinel->namePrefix = "sentinel";
            std::vector<boltType> offenseBolts = { BOLT_SPARK, BOLT_SPARK, BOLT_POISON };
            std::vector<boltType> defenseBolts = { BOLT_HEALING, BOLT_HEALING, BOLT_HASTE, BOLT_SHIELDING };
            sentinel->bolts.push_back(offenseBolts[rand_range(0, offenseBolts.size() - 1)]);
            sentinel->bolts.push_back(defenseBolts[rand_range(0, defenseBolts.size() - 1)]);
        }
    }
    
    // Step 19: Constant monsters
    wardenMonsterId = 2;
    spectralImageMonsterId = 1;
    
    // Step 20: Mirror totem
    Body *mirrorBody = NULL;
    mirrorBody = matchingBody([weird](const Body *body) {
        if (!(body->genFlags & GF_TOTEM) || (body->genFlags & GF_ANIMAL)) {
            return false;
        }
        return (body->genFlags & GF_WIZARDLY) || (body->genFlags & GF_SHAMANISTIC);
    });
    if (mirrorBody != NULL) {
        ChimeraMonster *mirrorTotem = newMonster(*mirrorBody);
        mirrorMonsterId = mirrorTotem->monsterId;
        ChimeraMonster::replace(mirrorTotem->baseName, "$BASE", "mirror");
        ChimeraMonster::replace(mirrorTotem->baseName, "$BASE", "ancient");
        mirrorTotem->flags = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS |
                              MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY | MONST_REFLECT_4 | MONST_IMMUNE_TO_WEAPONS | MONST_IMMUNE_TO_FIRE);
        mirrorTotem->bolts = {BOLT_BECKONING};
        mirrorTotem->feature = DF_MIRROR_TOTEM_STEP;
        mirrorTotem->lightType = NO_LIGHT;
        mirrorTotem->featurePeriodicPercent = 100;
    }
    
    // Step 21: Webber (if we have one)
    for (ChimeraMonster *monster : monsters) {
        for (boltType bolt : monster->bolts) {
            if (bolt == BOLT_SPIDERWEB) {
                webberMonsterId = monster->monsterId;
                break;
            }
        }
        if (webberMonsterId != 0) {
            break;
        }
    }
    
    // Step 22: Digger
    ChimeraMonster *digger = NULL;
    for (ChimeraMonster *existing : monsters) {
        if (!(existing->genFlags & GF_DIGGER) || (existing->genFlags & (GF_BOSS_ONLY | GF_AQUATIC_ONLY | GF_KAMIKAZE))) {
            continue;
        }
        digger = existing;
    }
    if (digger == NULL) {
        Body *diggerBody = matchingBody([weird](const Body *body) {
            return body->genFlags & GF_DIGGER;
        });
        if (diggerBody != NULL) {
            digger = newMonster(*diggerBody);
        }
    }
    if (digger != NULL) {
        diggerMonsterId = digger->monsterId;
        if (digger->namePrefix.length() == 0 && digger->nameSuffix.length() == 0) {
            if (digger->genFlags & GF_AMORPHOUS) {
                digger->namePrefix = "wall";
            } else if (digger->genFlags & GF_INSECTOID) {
                digger->namePrefix = "burrowing";
            } else {
                digger->namePrefix = "tunnel";
            }
        }
        digger->baseFlavor += "The " + digger->baseName + " is able to hibernate in walls when low on food, and at times even burst through them in ambush.";
        digger->flags |= MONST_NEVER_SLEEPS;
    }
    
    // Step 23: Spark turret (if we have one)
    for (ChimeraMonster *monster : monsters) {
        if (!(monster->genFlags & GF_TURRET)) {
            continue;
        }
        for (boltType bolt : monster->bolts) {
            if (bolt == BOLT_SPARK) {
                sparkMonsterId = monster->monsterId;
                break;
            }
        }
        if (sparkMonsterId != 0) {
            break;
        }
    }
    
    // Step 24: Undead (if we have one)
    for (ChimeraMonster *monster : monsters) {
        if (!(monster->genFlags & GF_UNDEAD)) {
            undeadMonsterId = monster->monsterId;
            break;
        }
    }
    
    // Step 25: Invisibility (if we have any)
    for (ChimeraMonster *monster : monsters) {
        if (!(monster->flags & MONST_INVISIBLE)) {
            invisibleMonsterId = monster->monsterId;
            break;
        }
    }
    if (invisibleMonsterId == 0) {
        // undead are close enough
        invisibleMonsterId = undeadMonsterId;
    }
    
    // Step 26: Conjuration
    ChimeraMonster *conjuration;
    Body *conjurationBody = matchingBody([](const Body *body) {
        return body->genFlags & GF_CONJURATION;
    });
    conjuration = newMonster(*conjurationBody);
    conjurationMonsterId = conjuration->monsterId;
    
    // Step 27: Bosses
    std::vector<std::reference_wrapper<ChimeraMonster>> bossMooks;
    for (int i = 0; i < bossCount; i += 1) {
        int minDL = 5 + (i + 0) * ((AMULET_LEVEL - 10) / bossCount);
        int maxDL = 5 + (i + 1) * ((AMULET_LEVEL - 10) / bossCount) + 1;
        Horde *bossHorde = NULL;
        while (bossHorde == NULL) {
            if (rand_percent(50)) {
                // a weirdass monster
                Body *body = matchingBody([weird](const Body *body) {
                    if (body->genFlags & weird) {
                        return false;
                    }
                    if ((body->genFlags & GF_BOSSLIKE) && rand_percent(50)) {
                        return false;
                    }
                    return true;
                });
                if (body == NULL) {
                    continue;
                }
                Ability *ability = matchingAbility([body, minDL, maxDL](const Ability *ability) {
                    if (!ability->isValidFor(*body, GF_BOSS_ONLY, NULL)) {
                        return false;
                    }
                    if (ability->requiredFlags & GF_PACK_MEMBER) {
                        return false;
                    }
                    if (!(ability->requiredFlags & GF_BOSS_ONLY)) {
                        return false;
                    }
                    int dl = body->dangerLevel + ability->dangerBoost;
                    return dl >= minDL && dl <= maxDL;
                });
                if (ability == NULL) {
                    continue;
                }
                
                ChimeraMonster *boss = newMonster(*body);
                boss->applyAbility(*ability);
                bossHorde = &newHorde(*boss);
            } else {
                // a commander
                std::vector<std::reference_wrapper<ChimeraMonster>> shuffledCommanders = std::vector<std::reference_wrapper<ChimeraMonster>>(mookMonsters);
                std::random_shuffle(shuffledCommanders.begin(), shuffledCommanders.end());
                for (ChimeraMonster &bossMook : shuffledCommanders) {
                    if (!(bossMook.genFlags & (GF_ANIMAL | GF_SUPPORTS_CLASS | GF_ARMED | GF_WIZARDLY | GF_SHAMANISTIC))) {
                        continue;
                    }
                    if (bossMook.genFlags & (GF_BRAINLESS | GF_NO_SPECIALS)) {
                        continue;
                    }
                    bool used = false;
                    for (ChimeraMonster &usedMook : bossMooks) {
                        if (usedMook.getBaseName() == bossMook.getBaseName()) {
                            used = true;
                            break;
                        }
                    }
                    if (used) {
                        continue;
                    }
                    std::vector<std::reference_wrapper<ChimeraMonster>> minions;
                    for (auto testSet : mookMinions) {
                        for (ChimeraMonster &testMook : testSet) {
                            if (testMook.getBaseName() == bossMook.getBaseName()) {
                                minions.push_back(testMook);
                            }
                        }
                    }
                    if (minions.size() <= 1 && !(bossMook.genFlags & GF_ANIMAL)) {
                        continue;
                    }
                    Ability *ability = matchingAbility([bossMook, minDL, maxDL](const Ability *ability) {
                        if (!ability->isValidFor(bossMook.body, (GF_BOSS_ONLY | GF_PACK_MEMBER), &bossMook)) {
                            return false;
                        }
                        unsigned long reqFlags = (GF_BOSS_ONLY | GF_PACK_MEMBER);
                        if ((ability->requiredFlags & reqFlags) != reqFlags) {
                            return false;
                        }
                        int dl = bossMook.dangerLevel + ability->dangerBoost;
                        return dl >= minDL && dl <= maxDL;
                    });
                    if (ability == NULL) {
                        continue;
                    }
                    
                    ChimeraMonster *boss = newMonster(&bossMook);
                    boss->genFlags |= GF_PACK_MEMBER;
                    boss->applyAbility(*ability);
                    bossMooks.push_back(bossMook);
                    bossHorde = &newHorde(*boss);
                    
                    for (ChimeraMonster *monster : monsters) {
                        if (monster->baseMook == boss->baseMook && (monster->genFlags & GF_TOTEM)) {
                            bossHorde->addMember(*monster, 1, 1);
                        }
                    }
                    for (ChimeraMonster &minion : minions) {
                        if (minion.baseMook == NULL) {
                            if (minions.size() == 1) {
                                bossHorde->addMember(minion, 3, 4);
                            } else {
                                bossHorde->addMember(minion, 2, 2);
                            }
                        } else {
                            bossHorde->addMember(minion, 1, 1);
                        }
                    }
                    break;
                }
            }
        }
        bossHorde->purpose = HordePurposeType::BOSS;
        bossHorde->overrideFloorMin = minDL;
        bossHorde->overrideFloorMax = (maxDL > 15) ? DEEPEST_LEVEL : maxDL;
    }
    
    // Step 29: Captives
    std::vector<std::vector<std::reference_wrapper<ChimeraMonster>>> allVanillaSets;
    std::vector<std::reference_wrapper<Horde>> captiveHordes;
    for (ChimeraMonster &thief : thieves) {
        allVanillaSets.push_back(std::vector<std::reference_wrapper<ChimeraMonster>>({ thief }));
    }
    for (ChimeraMonster &fodder : fodderMonsters) {
        allVanillaSets.push_back(std::vector<std::reference_wrapper<ChimeraMonster>>({ fodder }));
    }
    for (auto minionSet : mookMinions) {
        allVanillaSets.push_back(minionSet);
    }
    
    // find the captive
    for (unsigned long i = 0; i < allVanillaSets.size(); i += 1) {
        std::vector<std::reference_wrapper<ChimeraMonster>> mookSet = allVanillaSets[i];
        for (ChimeraMonster &captive : mookSet) {
            if (captive.genFlags & (GF_UNDEAD | GF_BRAINLESS)) {
                continue;
            }
            if (captive.flags & (MONST_IMMUNE_TO_WEAPONS)) {
                continue;
            }
            if (captive.summon != SummonType::NONE && rand_percent(66)) {
                continue;
            }
            if ((captive.abilFlags & (MA_HIT_STEAL_FLEE)) && !((captive.genFlags & GF_ANIMAL) && captive.dangerLevel < 8)) {
                continue;
            }
            if (captive.genFlags & (GF_AMORPHOUS | GF_INSECTOID | GF_ANIMAL)) {
                if (!(captive.genFlags & (GF_SHAMANISTIC | GF_WIZARDLY | GF_ARMED | GF_SUPPORTS_CLASS | GF_THIEVING))) {
                    continue;
                }
            }
            captive.generateName();
            if (captive.getBaseName() != captive.getDisplayName() && captive.physique != PhysiqueType::SPELLCASTER) {
                continue;
            }
            Horde &horde = newHorde(captive);
            captiveHordes.push_back(horde);
            horde.purpose = HordePurposeType::CAPTIVE;
            if (captive.abilFlags & MA_HIT_STEAL_FLEE) {
                horde.extraDanger = -4;
            }
            
            if (captive.bloodType == DF_RED_BLOOD) {
                Horde &bloodbag = newHorde(captive);
                bloodbag.purpose = HordePurposeType::BLOODBAG;
                bloodbag.extraDanger -= 1;
                if (captive.abilFlags & MA_HIT_STEAL_FLEE) {
                    horde.extraDanger -= -4;
                }
                if (captive.physique == PhysiqueType::SPELLCASTER) {
                    horde.extraDanger -= -2;
                    horde.extraRange = 2;
                }
            }
            
            Horde &kennel = newHorde(captive);
            kennel.purpose = HordePurposeType::KENNEL;
            kennel.extraRange = 1;
            
            Horde &dungeonCaptive = newHorde(captive);
            dungeonCaptive.purpose = HordePurposeType::DUNGEON_CAPTIVE;
            dungeonCaptive.extraRange = 1;
            
            // find the captor(s)
            for (int j = i + 1; j >= 0; j -= 1) {
                if (j >= (int)allVanillaSets.size()) {
                    continue;
                }
                std::vector<std::reference_wrapper<ChimeraMonster>> mookSet = allVanillaSets[j];
                if (mookSet[0].get().getBaseName() == captive.getBaseName()) {
                    continue;
                }
                if (mookSet[0].get().abilFlags & (MA_HIT_STEAL_FLEE)) {
                    continue;
                }
                if (mookSet[0].get().genFlags & (GF_ANIMAL | GF_INSECTOID | GF_BRAINLESS | GF_AMORPHOUS)) {
                    if (!(mookSet[0].get().genFlags & (GF_WIZARDLY | GF_SHAMANISTIC | GF_ARMED | GF_SUPPORTS_CLASS))) {
                        continue;
                    }
                }
                int dangerDelta = mookSet[0].get().dangerLevel - horde.calculateDL();
                if (dangerDelta > 4) {
                    continue;
                } else if (dangerDelta > 2) {
                    horde.addMember(mookSet[0], 1, 1);
                    horde.extraDanger = dangerDelta - 1;
                } else if (dangerDelta > 0) {
                    horde.addMember(mookSet[0], 2, 3);
                } else {
                    if (mookSet.size() > 1) {
                        for (ChimeraMonster &captor : mookSet) {
                            horde.addMember(captor, (dangerDelta > -5 ? 1 : 2), (dangerDelta > -2 ? 1 : 2));
                        }
                    } else {
                        horde.addMember(mookSet[0], (dangerDelta > -3 ? 2 : 3), (dangerDelta > -6 ? 3 : 4));
                    }
                }
                break;
            }
            if (horde.memberCount() == 1) {
                for (ChimeraMonster &captor : fodderMonsters) {
                    if (captor.genFlags & (GF_ARMED | GF_SUPPORTS_CLASS | GF_WIZARDLY | GF_SHAMANISTIC)) {
                        horde.addMember(captor, 2, 3);
                        break;
                    }
                }
                if (horde.memberCount() == 1) {
                    for (ChimeraMonster &captor : mookMonsters) {
                        if (captor.genFlags & (GF_ARMED | GF_SUPPORTS_CLASS | GF_WIZARDLY | GF_SHAMANISTIC)) {
                            horde.addMember(captor, 1, 1);
                            break;
                        }
                    }
                }
            }
        }
    }
    for (int i = captiveHordes.size(); i < targetCaptives; i += 1) {
        captiveHordes[rand_range(0, captiveHordes.size() - 1)].get().extraFrequency += 1;
    }
    
    // Step 30: Mark some new hordes for machines
    std::vector<Horde *> existingHordes = std::vector<Horde *>(hordes);
    for (Horde *horde : existingHordes) {
        if (horde->purpose == HordePurposeType::AQUA || horde->purpose == HordePurposeType::TURRET) {
            Horde *newHorde = horde->createMachineVariant();
            hordes.push_back(newHorde);
        }
        int thievesCreated = 0;
        if (horde->purpose == HordePurposeType::THIEF && horde->memberCount() == 1 && thievesCreated < 2) {
            thievesCreated += 1;
            Horde *newHorde = horde->createMachineVariant();
            hordes.push_back(newHorde);
            newHorde->overrideFloorMin = (thievesCreated+0) * AMULET_LEVEL / 2;
            newHorde->overrideFloorMax = (thievesCreated+1) * AMULET_LEVEL / 2;
        }
    }
    
    // Step 31: Statue machines
    for (ChimeraMonster &mook : mookMonsters) {
        if (!(mook.genFlags & (GF_DIGGER))) {
            if ((mook.abilFlags & MA_CLONE_SELF_ON_DEFEND) && mook.monsterId != getDiggerMonsterId()) {
                continue;
            }
            if (mook.genFlags & (GF_ANIMAL | GF_WIZARDLY | GF_ARMED)) {
                continue;
            }
        }
        Horde &horde = newHorde(mook);
        horde.purpose = HordePurposeType::STATUE_MACHINE;
        horde.extraDanger = 2;
    }
    
    // Step 32: The warren
    std::vector<std::reference_wrapper<ChimeraMonster>> warrenMinions;
    for (ChimeraMonster &mook : mookMonsters) {
        warrenMinions.clear();
        if (!(mook.genFlags & (GF_SUPPORTS_CLASS | GF_ARMED | GF_WIZARDLY | GF_SHAMANISTIC))) {
            continue;
        }
        if (mook.genFlags & (GF_BRAINLESS | GF_NO_SPECIALS | GF_NO_GROUPS)) {
            continue;
        }
        for (auto testSet : mookMinions) {
            for (ChimeraMonster &testMook : testSet) {
                if (testMook.getBaseName() == mook.getBaseName()) {
                    warrenMinions.push_back(testMook);
                }
            }
        }
        if (warrenMinions.size() > 1) {
            break;
        }
    }
    if (warrenMinions.size() > 0) {
        if (warrenMinions.size() == 2) {
            ChimeraMonster *newMinion = newMonster(&warrenMinions[0].get());
            warrenMinions.push_back(*newMinion);
            newMinion->genFlags |= GF_PACK_MEMBER;
            Ability *ability = matchingAbility([newMinion](const Ability *ability) {
                return ability->isValidForMonster(*newMinion);
            });
            newMinion->applyAbility(*ability);
        }
        ChimeraMonster *warrenBoss = NULL;
        for (ChimeraMonster &bossMook : bossMooks) {
            if (bossMook.getBaseName() == warrenMinions[0].get().getBaseName()) {
                warrenBoss = &bossMook;
            }
        }
        if (warrenBoss == NULL) {
            warrenBoss = newMonster(&warrenMinions[0].get());
            warrenBoss->genFlags |= (GF_PACK_MEMBER | GF_BOSS_ONLY | GF_BOSSLIKE);
            Ability *warrenBossAbility = matchingAbility([warrenBoss](const Ability *ability) {
                return ability->isValidForMonster(*warrenBoss) && (ability->requiredFlags & GF_BOSS_ONLY);
            });
            if (warrenBossAbility == NULL) {
                warrenBossAbility = matchingAbility([warrenBoss](const Ability *ability) {
                    return ability->isValidForMonster(*warrenBoss);
                });
            }
            warrenBoss->applyAbility(*warrenBossAbility);
            warrenBoss->flags |= MONST_CARRY_ITEM_25;
        }
        warrenBossMonsterId = warrenBoss->monsterId;
        std::vector<Horde *> warrenTotems;
        for (Horde *horde : hordes) {
            if (horde->purpose == HordePurposeType::TOTEM && horde->getBaseName() == warrenBoss->getBaseName()) {
                warrenTotems.push_back(horde->createMachineVariant());
            }
        }
        for (Horde *horde : warrenTotems) {
            hordes.push_back(horde);
        }
        for (unsigned long i = 0; i < warrenMinions.size(); i += 1) {
            ChimeraMonster &minion = warrenMinions[i];
            Horde &horde = newHorde(minion);
            horde.purpose = HordePurposeType::WARREN;
            if (i > 0) {
                horde.extraFrequency = -2 * (warrenMinions.size() - 1);
            }
        }
        for (unsigned long i = 1; i < warrenMinions.size(); i += 1) {
            ChimeraMonster &baseMinion = warrenMinions[0];
            ChimeraMonster &follower = warrenMinions[i];
            if (follower.physique == PhysiqueType::SPELLCASTER) {
                Horde &horde = newHorde(baseMinion);
                horde.addMember(follower, 1, 1);
                horde.purpose = HordePurposeType::WARREN;
            }
        }
        Horde &warrenCaptiveHorde = newHorde(warrenMinions[0].get());
        warrenCaptiveHorde.purpose = HordePurposeType::WARREN_CAPTIVE;
        warrenCaptiveHorde.addMember(warrenMinions[0], 1, 2);
    }
    
    // Step FINAL: Summoner hordes
    for (ChimeraMonster *monster : monsters) {
        switch (monster->summon) {
            case SummonType::CONJURATION: {
                Horde &horde = newHorde(*monster);
                horde.purpose = HordePurposeType::CONJURATION;
                horde.addMember(*conjuration, 3 + monster->dangerLevel / 15, 5 + monster->dangerLevel / 10);
                break;
            }
            case SummonType::SPAWN_BASE_MOOK_DISTANT:
            case SummonType::SPAWN_BASE_MOOK: {
                Horde &horde = newHorde(*monster);
                horde.purpose = (monster->summon == SummonType::SPAWN_BASE_MOOK) ? HordePurposeType::SUMMON : HordePurposeType::SUMMON_DISTANCE;
                int deltaDL = monster->dangerLevel - monster->baseMook->dangerLevel;
                horde.addMember(*(monster->baseMook), 1 + deltaDL / 10, 1 + deltaDL / 6);
                break;
            }
            case SummonType::SPAWN_UNRELATED_MOOK: {
                for (ChimeraMonster &mook : mookMonsters) {
                    if (mook.dangerLevel >= monster->dangerLevel/2 - 1 && mook.dangerLevel <= monster->dangerLevel/3 - 2) {
                        Horde &horde = newHorde(*monster);
                        horde.addMember(mook, 2, 3);
                        horde.purpose = HordePurposeType::SUMMON;
                        if (rand_percent(40)) {
                            break;
                        }
                    } else if (mook.dangerLevel >= monster->dangerLevel * 1/2 && mook.dangerLevel <= monster->dangerLevel * 2/3) {
                        Horde &horde = newHorde(*monster);
                        horde.addMember(mook, 1, 1);
                        horde.purpose = HordePurposeType::SUMMON;
                        if (rand_percent(40)) {
                            break;
                        }
                    }
                }
                break;
            }
            case SummonType::SPAWN_FODDER: {
                for (ChimeraMonster &fodder : fodderMonsters) {
                    Horde &horde = newHorde(*monster);
                    horde.purpose = HordePurposeType::SUMMON;
                    horde.addMember(fodder, 1, rand_range(1, 4));
                    if (rand_percent(30)) {
                        break;
                    }
                }
                break;
            }
            case SummonType::TRANSFORM_MOOK: {
                Horde &horde = newHorde(*monster);
                horde.purpose = HordePurposeType::SUMMON;
                horde.addMember(*(monster->baseMook), 3, 3);
                break;
            }
            case SummonType::NONE: {
                break;
            }
        }
    }
    
    std::string report = debugReport();
    std::cout << report;
    return;
}

Body *MonsterGenerator::matchingBody(const std::function<bool(const Body *)>& filter) {
    // TODO: this is a not very efficient solution at all
    std::vector<Body *> passing = std::vector<Body *>();
    for (Body *body : bodies) {
        if (filter(body) && (!body->inUse || body->reusable)) {
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
    for (Ability *ability : abilities) {
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

    for (ChimeraMonster *monster : monsters) {
        report += monster->debugReport() + "\n";
    }
    report += "\n==== HORDES ====\n\n";
    for (Horde *horde : hordes) {
        report += horde->debugReport() + "\n";
    }

    return report;
}

ChimeraMonster *MonsterGenerator::newMonster(Body &body) {
    ChimeraMonster *monster = new ChimeraMonster(body);
    monsters.push_back(monster);
    return monster;
}

ChimeraMonster *MonsterGenerator::newMonster(ChimeraMonster *baseMonster) {
    ChimeraMonster *monster = new ChimeraMonster(baseMonster->body);
    monster->baseMook = baseMonster;
    monsters.push_back(monster);
    return monster;
}

Horde &MonsterGenerator::newHorde(const ChimeraMonster &monster) {
    Horde *horde = new Horde(monster);
    hordes.push_back(horde);
    return *horde;
}
