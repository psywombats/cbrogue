/*
 * Body.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Body.h"
#include "ChimeraMonster.h"
#include "IncludeGlobals.h"

std::set<std::reference_wrapper<char>> Body::usedChars = std::set<std::reference_wrapper<char>>();

Body::Body() :
        hp(0),
        baseChar('?'),
        genFlags(0),
        flags(0),
        abilFlags(0),
        baseColor(&brown),
        inUse(false),
        gender(GenderType::NONE),
        dangerLevel(0),
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

void Body::applyToMonster(ChimeraMonster &monster) {
    monster.hp = this->hp;
    monster.damage = (randomRange) {minDamage, maxDamage, (short)((maxDamage - minDamage) / 3)};
    monster.accuracy = this->accuracy;
    monster.defense = this->defense;

    monster.moveSpeed = this->moveSpeed;
    monster.attackSpeed = this->attackSpeed;
    monster.regenSpeed = this->regenSpeed;

    monster.name = this->baseName;
    monster.gender = this->gender;
    monster.displayColor = this->baseColor;

    monster.dangerLevel = this->dangerLevel;

    if (this->baseChar == '?') {
        this->baseChar = this->baseName.at(0);
        if (Body::usedChars.find(this->baseChar) == Body::usedChars.end()) {
            this->baseChar = tolower(this->baseChar);
            Body::usedChars.insert(this->baseChar);
        } else {
            // let's just hope there aren't more than two monsters on the same letter
            this->baseChar = toupper(this->baseChar);
        }
    }
    monster.displayChar = this->baseChar;

    this->inUse = true;

    monster.genFlags |= this->genFlags;
    monster.flags |= this->flags;
    monster.abilFlags |= this->abilFlags;
}

// ideally this thing would read its data from JSON
// but uhhh 7DRL YOLO
std::vector<Body *> Body::loadBodies() {
    std::vector<Body *> bodies = std::vector<Body *>();
    Body *body;

    // for reference, these are the supported base colors:
    // brown green tanColor darkGreen gray darkYellow white darkBlue darkRed darkPurple

    body = new Body();
    body->baseName = "rat";
    body->baseColor = &brown;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 6;
    body->dangerLevel = 1;
    body->genFlags = (GenerateFlag::ANIMAL);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "kobold";
    body->baseColor = &darkGreen;
    body->minDamage = 1;
    body->maxDamage = 4;
    body->hp = 7;
    body->dangerLevel = 2;
    body->gender = GenderType::BOTH;
    body->genFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::SHAMANISTIC | GenerateFlag::WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "snake";
    body->baseColor = &darkGreen;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 5;
    body->dangerLevel = 2;
    body->genFlags = (GenerateFlag::INSECTOID | GenerateFlag::ANIMAL);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "jackal";
    body->baseColor = &tanColor;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 8;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 3;
    body->genFlags = (GenerateFlag::ANIMAL);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "hyena";
    body->baseColor = &tanColor;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 9;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 4;
    body->genFlags = (GenerateFlag::ANIMAL);
    body->flags = (MONST_FLEES_NEAR_DEATH);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "goblin";
    body->baseColor = &brown;
    body->minDamage = 2;
    body->maxDamage = 5;
    body->hp = 15;
    body->defense = DefenseType::LOW;
    body->accuracy = AccuracyType::INACCURATE;
    body->dangerLevel = 6;
    body->gender = GenderType::BOTH;
    body->genFlags = (GenerateFlag::SUPPORTS_CLASS & GenerateFlag::SHAMANISTIC);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "frog";
    body->baseColor = &darkGreen;
    body->minDamage = 1;
    body->maxDamage = 4;
    body->hp = 8;
    body->defense = DefenseType::DEFENSELESS;
    body->dangerLevel = 2;
    body->genFlags = (GenerateFlag::ANIMAL | GenerateFlag::AMORPHOUS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "bat";
    body->baseColor = &gray;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 18;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 6;
    body->genFlags = (GenerateFlag::ANIMAL);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "mound";
    body->baseColor = &green;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 15;
    body->accuracy = AccuracyType::INACCURATE;
    body->dangerLevel = 2;
    body->genFlags = (GenerateFlag::AMORPHOUS | GenerateFlag::BURSTS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "millipede";
    body->baseColor = &darkYellow;
    body->minDamage = 4;
    body->maxDamage = 12;
    body->hp = 18;
    body->accuracy = AccuracyType::INACCURATE;
    body->dangerLevel = 8;
    body->genFlags = (GenerateFlag::ANIMAL | GenerateFlag::INSECTOID);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "ogre";
    body->baseColor = &brown;
    body->minDamage = 9;
    body->maxDamage = 13;
    body->hp = 55;
    body->attackSpeed = AttackSpeedType::SLOW;
    body->dangerLevel = 10;
    body->gender = GenderType::BOTH;
    body->genFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::SHAMANISTIC);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "monster";
    body->baseColor = &darkGreen;
    body->minDamage = 3;
    body->maxDamage = 4;
    body->hp = 55;
    body->regenSpeed = RegenSpeedType::FAST;
    body->dangerLevel = 12;
    body->genFlags = (GenerateFlag::AMORPHOUS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "arachnid";
    body->baseColor = &darkYellow;
    body->minDamage = 3;
    body->maxDamage = 4;
    body->hp = 20;
    body->accuracy = AccuracyType::ACCURATE;
    body->defense = DefenseType::HIGH;
    body->dangerLevel = 8;
    body->genFlags = (GenerateFlag::ANIMAL | GenerateFlag::INSECTOID);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "wisp";
    body->baseColor = &white;
    body->minDamage = 5;
    body->maxDamage = 8;
    body->hp = 20;
    body->accuracy = AccuracyType::ACCURATE;
    body->defense = DefenseType::HIGH;
    body->dangerLevel = 11;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "wraith";
    body->baseColor = &gray;
    body->minDamage = 6;
    body->maxDamage = 13;
    body->hp = 50;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 13;
    body->genFlags = (GenerateFlag::ARMED);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "wight";
    body->baseColor = &white;
    body->minDamage = 3;
    body->maxDamage = 9;
    body->hp = 50;
    body->dangerLevel = 13;
    body->genFlags = (GenerateFlag::ARMED | GenerateFlag::WIZARDLY);
    body->abilFlags = (MA_TRANSFERENCE);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "blob";
    body->baseColor = &darkPurple;
    body->minDamage = 5;
    body->maxDamage = 10;
    body->hp = 50;
    body->defense = DefenseType::LOW;
    body->dangerLevel = 10;
    body->genFlags = (GenerateFlag::AMORPHOUS | GenerateFlag::BURSTS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "vampire";
    body->baseColor = &gray;
    body->minDamage = 5;
    body->maxDamage = 10;
    body->hp = 30;
    body->dangerLevel = 9;
    body->abilFlags = (MA_TRANSFERENCE);
    body->genFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "giant";
    body->baseColor = &darkGreen;
    body->minDamage = 10;
    body->maxDamage = 15;
    body->hp = 65;
    body->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
    body->dangerLevel = 15;
    body->gender = GenderType::BOTH;
    body->genFlags = (GenerateFlag::SUPPORTS_CLASS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "lizardman";
    body->baseColor = &green;
    body->minDamage = 7;
    body->maxDamage = 11;
    body->hp = 75;
    body->dangerLevel = 13;
    body->genFlags = (GenerateFlag::ANIMAL | GenerateFlag::SUPPORTS_CLASS | GenerateFlag::SHAMANISTIC | GenerateFlag::WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "dar";
    body->baseColor = &darkBlue;
    body->minDamage = 3;
    body->maxDamage = 5;
    body->hp = 20;
    body->dangerLevel = 7;
    body->gender = GenderType::BOTH;
    body->genFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "horseman";
    body->baseColor = &darkYellow;
    body->minDamage = 4;
    body->maxDamage = 8;
    body->hp = 35;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 10;
    body->gender = GenderType::MALE_ONLY;
    body->genFlags = (GenerateFlag::ANIMAL | GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "hyenaman";
    body->baseColor = &brown;
    body->minDamage = 4;
    body->maxDamage = 8;
    body->hp = 30;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 10;
    body->gender = GenderType::MALE_ONLY;
    body->genFlags = (GenerateFlag::ANIMAL | GenerateFlag::SUPPORTS_CLASS | GenerateFlag::SHAMANISTIC);
    body->flags = (MONST_FLEES_NEAR_DEATH);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "worm";
    body->baseColor = &gray;
    body->minDamage = 18;
    body->maxDamage = 22;
    body->hp = 80;
    body->moveSpeed = MoveSpeedType::SLOW;
    body->attackSpeed = AttackSpeedType::SLOW;
    body->dangerLevel = 13;
    body->genFlags = (GenerateFlag::ANIMAL | GenerateFlag::INSECTOID);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "fury";
    body->baseColor = &darkRed;
    body->minDamage = 6;
    body->maxDamage = 11;
    body->hp = 20;
    body->flags = (MONST_FLIES);
    body->accuracy = AccuracyType::ACCURATE;
    body->defense = DefenseType::HIGH;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 16;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "scorpion";
    body->baseColor = &tanColor;
    body->minDamage = 25;
    body->maxDamage = 30;
    body->hp = 100;
    body->dangerLevel = 23;
    body->defense = DefenseType::LOW;
    body->flags = (GenerateFlag::INSECTOID);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "horror";
    body->baseColor = &darkPurple;
    body->minDamage = 25;
    body->maxDamage = 35;
    body->hp = 120;
    body->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
    body->dangerLevel = 28;
    body->genFlags = (GenerateFlag::AMORPHOUS | GenerateFlag::BURSTS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "dragon";
    body->baseColor = &darkRed;
    body->minDamage = 25;
    body->maxDamage = 50;
    body->hp = 150;
    body->dangerLevel = 28;
    body->gender = GenderType::BOTH;
    body->genFlags = (GenerateFlag::WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "jelly";
    body->baseColor = &darkGreen;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 38;
    body->dangerLevel = 6;
    body->abilFlags = (MA_CLONE_SELF_ON_DEFEND);
    body->genFlags = (GenerateFlag::AMORPHOUS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "bloat";
    body->baseColor = &darkPurple;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 4;
    body->dangerLevel = 1;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->genFlags = (GenerateFlag::BURSTS | GenerateFlag::KAMIKAZE);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "spore crab";
    body->baseColor = &brown;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 2;
    body->dangerLevel = 3;
    body->moveSpeed = MoveSpeedType::FAST;
    body->genFlags = (GenerateFlag::BURSTS | GenerateFlag::KAMIKAZE);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "bomb goblin";
    body->baseColor = &brown;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 15;
    body->dangerLevel = 5;
    body->genFlags = (GenerateFlag::BURSTS | GenerateFlag::KAMIKAZE);
    bodies.push_back(body);

    return bodies;
}
