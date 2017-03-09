/*
 * Body.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Body.h"
#include "ChimeraMonster.h"
#include "IncludeGlobals.h"

std::set<std::reference_wrapper<uchar>> Body::usedChars = std::set<std::reference_wrapper<uchar>>();

Body::Body() :
        hp(0),
        baseChar('?'),
        rarityPercent(50),
        genFlags(0),
        flags(0),
        abilFlags(0),
        baseColor(&brown),
        inUse(false),
        singleUse(true),
        gender(GenderType::NONE),
        dangerLevel(0),
        periodicFeature(DF_NONE),
        periodicFeatureChance(0),
        blood(DF_NONE),
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
    }
    if (Body::usedChars.find(this->baseChar) != Body::usedChars.end() && this->singleUse) {
        if (islower(baseChar)) {
            this->baseChar = toupper(this->baseChar);
        } else {
            this->baseChar = tolower(this->baseChar);
        }
        if (Body::usedChars.find(this->baseChar) != Body::usedChars.end()) {
            // someone else has BOTH letters? fuck
            uchar randomChars[] = {0x03D7,0x03D6,0x03B6,0x0376,0x03EA,0x03E0,0x054B,0x0556,0x07F7,0x0186,0x0518};
            this->baseChar = randomChars[rand_range(0, sizeof(randomChars) / sizeof(randomChars[0]))];
        }
    }
    Body::usedChars.insert(this->baseChar);
    monster.displayChar = this->baseChar;

    if (this->singleUse) {
        this->inUse = true;
    }

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
    body->blood = DF_RED_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 6;
    body->dangerLevel = 1;
    body->genFlags = (GF_ANIMAL);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "kobold";
    body->baseColor = &darkGreen;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 4;
    body->hp = 7;
    body->dangerLevel = 2;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC | GF_WIZARDLY);
    body->rarityPercent = 75;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "snake";
    body->baseColor = &darkGreen;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 5;
    body->dangerLevel = 2;
    body->genFlags = (GF_INSECTOID | GF_ANIMAL);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "jackal";
    body->baseColor = &tanColor;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 8;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 3;
    body->genFlags = (GF_ANIMAL);
    body->rarityPercent = 75;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "hyena";
    body->baseColor = &tanColor;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 9;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 4;
    body->genFlags = (GF_ANIMAL);
    body->flags = (MONST_FLEES_NEAR_DEATH);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "goblin";
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 5;
    body->hp = 15;
    body->defense = DefenseType::LOW;
    body->accuracy = AccuracyType::INACCURATE;
    body->dangerLevel = 6;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    body->rarityPercent = 75;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "orc";
    body->baseColor = &darkGreen;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 3;
    body->maxDamage = 5;
    body->hp = 18;
    body->dangerLevel = 8;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC | GF_ARMED | GF_AQUATIC);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "acolyte";
    body->baseChar = 'A';
    body->baseColor = &darkGray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 5;
    body->hp = 16;
    body->dangerLevel = 7;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "frog";
    body->baseColor = &darkGreen;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 8;
    body->defense = DefenseType::DEFENSELESS;
    body->dangerLevel = 2;
    body->genFlags = (GF_ANIMAL | GF_AMORPHOUS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "bat";
    body->baseColor = &gray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 18;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 6;
    body->genFlags = (GF_ANIMAL);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "mound";
    body->baseColor = &green;
    body->baseChar = 'a';
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 15;
    body->accuracy = AccuracyType::INACCURATE;
    body->dangerLevel = 2;
    body->genFlags = (GF_AMORPHOUS | GF_BURSTS);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "millipede";
    body->baseColor = &darkYellow;
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 12;
    body->hp = 18;
    body->accuracy = AccuracyType::INACCURATE;
    body->dangerLevel = 8;
    body->genFlags = (GF_ANIMAL | GF_INSECTOID);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "ogre";
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 9;
    body->maxDamage = 13;
    body->hp = 55;
    body->attackSpeed = AttackSpeedType::SLOW;
    body->dangerLevel = 10;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "monster";
    body->baseColor = &darkGreen;
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 3;
    body->maxDamage = 4;
    body->hp = 55;
    body->regenSpeed = RegenSpeedType::FAST;
    body->dangerLevel = 14;
    body->abilFlags = MA_SEIZES;
    body->genFlags = (GF_AQUATIC | GF_AMORPHOUS);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "arachnid";
    body->baseChar = 's';
    body->baseColor = &darkYellow;
    body->minDamage = 3;
    body->maxDamage = 4;
    body->hp = 20;
    body->accuracy = AccuracyType::ACCURATE;
    body->defense = DefenseType::HIGH;
    body->dangerLevel = 8;
    body->genFlags = (GF_ANIMAL | GF_INSECTOID);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "wisp";
    body->baseChar = '*';
    body->baseColor = &white;
    body->blood = DF_ASH_BLOOD;
    body->minDamage = 5;
    body->maxDamage = 8;
    body->hp = 20;
    body->accuracy = AccuracyType::ACCURATE;
    body->defense = DefenseType::HIGH;
    body->dangerLevel = 11;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "wraith";
    body->baseChar = 'W';
    body->baseColor = &gray;
    body->blood = DF_ECTOPLASM_BLOOD;
    body->minDamage = 6;
    body->maxDamage = 13;
    body->hp = 50;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 13;
    body->genFlags = (GF_ARMED);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "skeleton";
    body->baseChar = 'z';
    body->baseColor = &white;
    body->blood = DF_BONES;
    body->minDamage = 4;
    body->maxDamage = 9;
    body->hp = 50;
    body->dangerLevel = 13;
    body->genFlags = (GF_ARMED | GF_WIZARDLY | GF_SUPPORTS_CLASS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "blob";
    body->baseColor = &darkPurple;
    body->blood = DF_PURPLE_BLOOD;
    body->minDamage = 5;
    body->maxDamage = 10;
    body->hp = 50;
    body->defense = DefenseType::LOW;
    body->dangerLevel = 10;
    body->genFlags = (GF_AQUATIC | GF_AMORPHOUS | GF_BURSTS);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "vampire";
    body->baseChar = 'V';
    body->baseColor = &gray;
    body->minDamage = 5;
    body->maxDamage = 10;
    body->hp = 30;
    body->dangerLevel = 9;
    body->abilFlags = (MA_TRANSFERENCE);
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    body->rarityPercent = 25;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "giant";
    body->baseChar = 'G';
    body->baseColor = &darkGreen;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 10;
    body->maxDamage = 15;
    body->hp = 65;
    body->regenSpeed = RegenSpeedType::VERY_FAST;
    body->dangerLevel = 15;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "lizardman";
    body->baseColor = &green;
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 7;
    body->maxDamage = 11;
    body->hp = 75;
    body->dangerLevel = 13;
    body->gender = GenderType::MALE_ONLY;
    body->genFlags = (GF_ANIMAL | GF_SUPPORTS_CLASS | GF_SHAMANISTIC | GF_WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "dar";
    body->baseColor = &darkBlue;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 3;
    body->maxDamage = 5;
    body->hp = 20;
    body->dangerLevel = 6;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "horseman";
    body->baseChar = 'C';
    body->baseColor = &darkYellow;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 8;
    body->hp = 35;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 10;
    body->gender = GenderType::MALE_ONLY;
    body->genFlags = (GF_ANIMAL | GF_SUPPORTS_CLASS | GF_WIZARDLY);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "hyenaman";
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 8;
    body->hp = 30;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 10;
    body->gender = GenderType::MALE_ONLY;
    body->genFlags = (GF_ANIMAL | GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    body->flags = (MONST_FLEES_NEAR_DEATH);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "worm";
    body->baseChar = 'U';
    body->baseColor = &gray;
    body->blood = DF_WORM_BLOOD;
    body->minDamage = 18;
    body->maxDamage = 22;
    body->hp = 80;
    body->moveSpeed = MoveSpeedType::SLOW;
    body->attackSpeed = AttackSpeedType::SLOW;
    body->dangerLevel = 13;
    body->genFlags = (GF_ANIMAL | GF_INSECTOID | GF_AQUATIC);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "fury";
    body->baseColor = &darkRed;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 6;
    body->maxDamage = 11;
    body->hp = 20;
    body->flags = (MONST_FLIES);
    body->accuracy = AccuracyType::ACCURATE;
    body->defense = DefenseType::HIGH;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 16;
    body->rarityPercent = 66;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "scorpion";
    body->baseChar = 'S';
    body->baseColor = &tanColor;
    body->minDamage = 25;
    body->maxDamage = 30;
    body->hp = 100;
    body->dangerLevel = 23;
    body->defense = DefenseType::LOW;
    body->flags = (GF_INSECTOID);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "horror";
    body->baseChar = 'H';
    body->baseColor = &darkPurple;
    body->blood = DF_PURPLE_BLOOD;
    body->minDamage = 25;
    body->maxDamage = 35;
    body->hp = 120;
    body->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
    body->dangerLevel = 28;
    body->genFlags = (GF_AMORPHOUS | GF_BURSTS | GF_AQUATIC);
    body->rarityPercent = 66;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "wyrm";
    body->baseChar = 'W';
    body->baseColor = &darkRed;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 25;
    body->maxDamage = 45;
    body->hp = 140;
    body->dangerLevel = 28;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_WIZARDLY | GF_AQUATIC);
    body->abilFlags = MA_ATTACKS_ALL_ADJACENT;
    body->rarityPercent = 66;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "jelly";
    body->baseColor = &darkGreen;
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 38;
    body->dangerLevel = 6;
    body->abilFlags = (MA_CLONE_SELF_ON_DEFEND);
    body->genFlags = (GF_AMORPHOUS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "bloat";
    body->baseColor = &darkPurple;
    body->blood = DF_PURPLE_BLOOD;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 4;
    body->dangerLevel = 1;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->genFlags = (GF_BURSTS | GF_KAMIKAZE);
    body->rarityPercent = 66;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "spore crab";
    body->baseChar = 'c';
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 2;
    body->dangerLevel = 3;
    body->moveSpeed = MoveSpeedType::FAST;
    body->genFlags = (GF_BURSTS | GF_KAMIKAZE | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "kamikaze dwarf";
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 15;
    body->dangerLevel = 6;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_BURSTS | GF_KAMIKAZE);
    body->rarityPercent = 33;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "monkey";
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 12;
    body->dangerLevel = 2;
    body->genFlags = (GF_THIEVING | GF_THIEVING_ONLY | GF_ANIMAL);
    body->rarityPercent = 66;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "ferret";
    body->baseColor = &tanColor;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 10;
    body->dangerLevel = 5;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_THIEVING | GF_ANIMAL);
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "raven";
    body->baseColor = &darkGray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 20;
    body->dangerLevel = 7;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->genFlags = (GF_THIEVING | GF_ANIMAL);
    body->rarityPercent = 33;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "magpie";
    body->baseColor = &gray;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 10;
    body->dangerLevel = 4;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->genFlags = (GF_THIEVING | GF_THIEVING_ONLY | GF_ANIMAL);
    body->rarityPercent = 66;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "quasit";
    body->baseColor = &white;
    body->blood = DF_ASH_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 9;
    body->hp = 35;
    body->dangerLevel = 8;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_THIEVING);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "turtle";
    body->baseColor = &brown;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 18;
    body->dangerLevel = 5;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    body->regenSpeed = RegenSpeedType::FAST;
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "eel";
    body->baseColor = &brown;
    body->minDamage = 3;
    body->maxDamage = 7;
    body->hp = 18;
    body->dangerLevel = 9;
    body->moveSpeed = MoveSpeedType::FAST;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS);
    body->regenSpeed = RegenSpeedType::FAST;
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "pirahna";
    body->baseColor = &darkRed;
    body->minDamage = 13;
    body->maxDamage = 17;
    body->hp = 25;
    body->dangerLevel = 15;
    body->defense = DefenseType::HIGH;
    body->moveSpeed = MoveSpeedType::FAST;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS);
    body->regenSpeed = RegenSpeedType::FAST;
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "shark";
    body->baseColor = &white;
    body->minDamage = 13;
    body->maxDamage = 17;
    body->hp = 40;
    body->dangerLevel = 15;
    body->defense = DefenseType::HIGH;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "mollusk";
    body->baseChar = 'M';
    body->baseColor = &brown;
    body->minDamage = 17;
    body->maxDamage = 25;
    body->hp = 120;
    body->dangerLevel = 26;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "merman";
    body->baseColor = &darkGreen;
    body->minDamage = 3;
    body->maxDamage = 6;
    body->hp = 18;
    body->dangerLevel = 8;
    body->gender = GenderType::MALE_ONLY;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLEES_NEAR_DEATH);
    body->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_ARMED);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "sea monk";
    body->baseChar = '&';
    body->baseColor = &darkGray;
    body->minDamage = 9;
    body->maxDamage = 16;
    body->hp = 50;
    body->dangerLevel = 17;
    body->gender = GenderType::MALE_ONLY;
    body->moveSpeed = MoveSpeedType::FAST;
    body->attackSpeed = AttackSpeedType::SLOW;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    body->regenSpeed = RegenSpeedType::FAST;
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_ARMED | GF_SUPPORTS_CLASS | GF_WIZARDLY);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "totem";
    body->baseChar = TOTEM_CHAR;
    body->baseColor = &green;
    body->blood = DF_RUBBLE_BLOOD;
    body->hp = 30;
    body->attackSpeed = AttackSpeedType::TOTEM;
    body->flags = (MONST_TURRET);
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_TOTEM | GF_SHAMANISTIC);
    body->singleUse = false;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "obelisk";
    body->baseChar = 0x03AA; // Ϊ
    body->baseColor = &green;
    body->blood = DF_RUBBLE_BLOOD;
    body->hp = 30;
    body->attackSpeed = AttackSpeedType::TOTEM;
    body->flags = (MONST_TURRET);
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_TOTEM | GF_WIZARDLY);
    body->singleUse = false;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "idol";
    body->baseChar = 0x03C7; // χ
    body->baseColor = &green;
    body->blood = DF_RUBBLE_BLOOD;
    body->attackSpeed = AttackSpeedType::TOTEM;
    body->flags = (MONST_TURRET);
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_TOTEM | GF_WIZARDLY);
    body->singleUse = false;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "phantom";
    body->baseColor = &white;
    body->blood = DF_ECTOPLASM_BLOOD;
    body->minDamage = 12;
    body->maxDamage = 18;
    body->hp = 35;
    body->dangerLevel = 21;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_NO_SPECIALS);
    body->flags = MONST_INVISIBLE;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "ghost";
    body->baseColor = &white;
    body->blood = DF_ECTOPLASM_BLOOD;
    body->minDamage = 6;
    body->maxDamage = 9;
    body->hp = 15;
    body->dangerLevel = 11;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_NO_SPECIALS);
    body->flags = MONST_INVISIBLE;
    body->rarityPercent = 25;
    body->periodicFeatureChance = 15;
    body->periodicFeature = DF_ECTOPLASM_DROPLET;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "golem";
    body->baseChar = 'G';
    body->baseColor = &gray;
    body->blood = DF_RUBBLE_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 8;
    body->hp = 350;
    body->regenSpeed = RegenSpeedType::NONE;
    body->dangerLevel = 26;
    body->flags = (MONST_DIES_IF_NEGATED);
    body->genFlags = (GF_NO_SPECIALS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "construct";
    body->baseColor = &gray;
    body->baseChar = '&';
    body->blood = DF_RUBBLE_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 120;
    body->dangerLevel = 11;
    body->defense = DefenseType::LOW;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_NO_SPECIALS);
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "zombie";
    body->baseColor = &vomitColor;
    body->baseChar = 'Z';
    body->blood = DF_ROT_GAS_BLOOD;
    body->minDamage = 7;
    body->maxDamage = 12;
    body->hp = 80;
    body->dangerLevel = 15;
    body->defense = DefenseType::DEFENSELESS;
    body->periodicFeature = DF_ROT_GAS_PUFF;
    body->periodicFeatureChance = 100;
    body->genFlags = (GF_NO_SPECIALS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "ghoul";
    body->baseColor = &vomitColor;
    body->baseChar = 'z';
    body->blood = DF_ROT_GAS_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 6;
    body->hp = 40;
    body->dangerLevel = 7;
    body->defense = DefenseType::DEFENSELESS;
    body->periodicFeature = DF_ROT_GAS_PUFF;
    body->periodicFeatureChance = 20;
    body->genFlags = (GF_NO_SPECIALS);
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "revenant";
    body->baseColor = &ectoplasmColor;
    body->baseChar = 'R';
    body->minDamage = 15;
    body->maxDamage = 20;
    body->hp = 30;
    body->dangerLevel = 25;
    body->defense = DefenseType::DEFENSELESS;
    body->genFlags = (GF_NO_SPECIALS);
    body->flags = MONST_IMMUNE_TO_WEAPONS;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "spectre";
    body->baseColor = &ectoplasmColor;
    body->baseChar = 'G';
    body->minDamage = 8;
    body->maxDamage = 12;
    body->hp = 18;
    body->dangerLevel = 12;
    body->defense = DefenseType::DEFENSELESS;
    body->moveSpeed = MoveSpeedType::SLOW;
    body->genFlags = (GF_NO_SPECIALS);
    body->flags = MONST_IMMUNE_TO_WEAPONS;
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "turret";
    body->baseColor = &gray;
    body->baseChar = TURRET_CHAR;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 30;
    body->dangerLevel = 0;
    body->defense = DefenseType::DEFENSELESS;
    body->attackSpeed = AttackSpeedType::TURRET;
    body->genFlags = (GF_TURRET);
    body->flags = MONST_TURRET;
    body->singleUse = false;
    body->rarityPercent = 100;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "statue";
    body->baseColor = &white;
    body->baseChar = '&';
    body->blood = DF_RUBBLE_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 2;
    body->hp = 50;
    body->dangerLevel = 5;
    body->defense = DefenseType::DEFENSELESS;
    body->attackSpeed = AttackSpeedType::TURRET;
    body->genFlags = (GF_TURRET | GF_SHAMANISTIC);
    body->flags = (MONST_TURRET | MONST_DIES_IF_NEGATED);
    body->singleUse = false;
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "orb";
    body->baseColor = &white;
    body->baseChar = 0x25CE; // ◎
    body->blood = DF_RUBBLE_BLOOD;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 40;
    body->dangerLevel = 3;
    body->defense = DefenseType::DEFENSELESS;
    body->attackSpeed = AttackSpeedType::TURRET;
    body->genFlags = (GF_TURRET | GF_WIZARDLY);
    body->flags = (MONST_TURRET | MONST_DIES_IF_NEGATED);
    body->singleUse = false;
    body->rarityPercent = 12;
    bodies.push_back(body);
    
    return bodies;
}
