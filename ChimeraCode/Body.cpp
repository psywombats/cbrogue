/*
 * Body.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Body.h"
#include "ChimeraMonster.h"
#include "IncludeGlobals.h"

Body::Body() :
        hp(0),
        baseName(""),
        flavor(""),
        baseChar('?'),
        rarityPercent(50),
        genFlags(0),
        flags(0),
        abilFlags(0),
        baseColor(&brown),
        singleUse(true),
        light(NO_LIGHT),
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

    monster.baseName = this->baseName;
    monster.gender = this->gender;
    monster.displayColor = this->baseColor;
    monster.baseDisplayChar = this->baseChar;
    monster.lightType = this->light;

    monster.dangerLevel = this->dangerLevel;

    Body::usedChars.insert(this->baseChar);
    monster.baseDisplayChar = this->baseChar;

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
    body->flavor = "The rat is a scavenger of the shallows, perpetually in search of decaying animal matter.";
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
    body->flavor = "The kobold is a lizardlike humanoid of the upper dungeon.";
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
    body->flavor = "The common cave snake may be small, but it packs a painful (if not venemous) bite.";
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
    body->flavor = "The jackal prowls the caverns for intruders to rend with $HISHER powerful jaws.";
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
    body->flavor = "The hyena is an opportunistic predator, stalking the halls by itself or in packs.";
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
    body->flavor = "A filthy little primate, the tribalistic goblin often travels in packs.";
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
    body->flavor = "The orc is a piglike humanoid endemic to the middle dungeon, where $HESHE is a part of a primitive society.";
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
    body->flavor = "Wreathed from head to toe in filthy robes, it is difficult to tell if the acolyte underneath is human at all.";
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
    body->flavor = "Seeking shelter from the elements, the frog is often found searching the dungeon for bodies of water.";
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
    body->flavor = "Often hunting in packs, leathery wings and keen senses guide the vampire bat unerringly to $HISHER prey.";
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
    body->flavor = "The mound squelches softly across the ground, seeking anything $HESHE can find for a meal.";
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
    body->flavor = "This giant millipede is covered in a array of ever-flailing legs.";
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
    body->flavor = "This lumbering creature carries an enormous club that $HESHE can swing with incredible force.";
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
    body->flavor = "This shambling creature is covered in mats of moldy, muddy fur, its arms outstretched in front of it.";
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
    body->flavor = "The arachnid's eight skittering legs propel its malformed, chitinous body across the dungeon floor.";
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
    body->flavor = "Little more than a gout of flame, the wisp pulses in time to an ethereal rhythm";
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
    body->flavor = "The wraith's hollow eye sockets stare hungrily at the world from $HISHER emaciated frame, $HISHER nails groping ceaselessly for a fresh victim.";
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
    body->flavor = "With $HISHER skull frozen in a rictus of death, the skeleton is a malevolent pile of bones held together by rusty wire and evil magic.";
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
    body->flavor = "The blob is a hungry, gelatinous mass that can take quite the beating, perhaps because of its lack of any discernible features.";
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
    body->flavor = "The vampire lives a solitary life deep underground, consuming any warm-blooded creature unfortunate to venture near $HISHER lair.";
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
    body->flavor = "This burly creature is almost too big to fit through the dungeon tunnels, and $HESHE seems to regenerate wounds as quickly as they occur.";
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
    body->flavor = "Half reptile and half human, the lizardmen live in the bellies of lakes or else deep underground caves. They do not take kindly to visitors.";
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
    body->flavor = "An elf of the deep, the dar defends $HISHER home with crystal-hewn weaponry and wicked magic.";
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
    body->baseName = "centaur";
    body->flavor = "Rider and steed in one, the centaur is often seen with a bow or other weapon, closing ground at frightening speed.";
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
    body->flavor = "A vicious, furry humanoid with a doglike face, the hyenaman gallops across the dungeon on all-fours with all the speed of $HISHER animal counterpart.";
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
    body->rarityPercent = 25;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "worm";
    body->flavor = "A strange and horrifying creature of the earth's deepest places, this worm stands man-high and delivers slow yet brutal blows to its prey.";
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
    body->flavor = "A creature of inchoate rage made flesh, the fury's moist wings beat loudly in the darkness.";
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
    body->flavor = "A merciless predator of the dark, this collosal arachnid crushes $HISHER prey to bits in $HISHER might incisors.";
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
    body->flavor = "This seething, towering nightmare of bone and sinew slinks through the bowels of the world. The horror's regeneration and strength are unparalleled.";
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
    body->flavor = "As collosal and brutal as $HESHE is old and wizened, the wyrm uses $HISHER mighty tail to beat down all who oppose $HIMHER.";
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
    body->flavor = "This mass of caustic pink goo slips across the ground in search of a warm meal.";
    body->baseColor = &darkGreen;
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 38;
    body->dangerLevel = 6;
    body->abilFlags = (MA_CLONE_SELF_ON_DEFEND);
    body->genFlags = (GF_AMORPHOUS | GF_NO_SPECIALS);
    body->rarityPercent = 100;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "bloat";
    body->flavor = "A bladder of gas buoys the bloat through the air, $HISHER thin veinous membrane ready to rupture at the slightest stress.";
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
    body->flavor = "A drab, scuttling creature, the spore crab would be unremarkable if not for the ominous pustules protruding from its fragile shell...";
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
    body->flavor = "Bellowing prayers in some foreign tongue, this zealot dwarf charges across the ground with a backpack full of dangerous, reactive compounds.";
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
    body->flavor = "Mischievous trickster that $HESHE is, the monkey lives to steal shiny trinkets from passing adventurers.";
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
    body->flavor = "The ferret is a mischevious weasel that makes $HISHER home in the grasses of the upper dungeon.";
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
    body->flavor = "A cloud of black feathers follow the raven. This particularly bloodthirsty strain of bird nests in the darkness and attacks much larger prey.";
    body->baseColor = &darkGray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 20;
    body->dangerLevel = 6;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->genFlags = (GF_THIEVING | GF_ANIMAL);
    body->rarityPercent = 33;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "magpie";
    body->flavor = "An unusual occupant of the dungeon, the magpie flits by in search of shiny treasure.";
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
    body->flavor = "An unusual construction of bone and magic, the quasit is the construct of some more powerful entity of the lower depths.";
    body->baseColor = &white;
    body->blood = DF_ASH_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 9;
    body->hp = 35;
    body->dangerLevel = 8;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_THIEVING);
    body->flags = MONST_DIES_IF_NEGATED;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "turtle";
    body->flavor = "The gentle turtle is more than capable of defending its home waters from intruding threats.";
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
    body->flavor = "The eel slips silently through the subterranean lake, waiting for unsuspecting prey to set foot in $HISHER dark waters.";
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
    body->flavor = "The cave pirahna lacks any eyes, but relentlessly seeks out and rends any creature unfortunate enough to stumble into $HISHER territory.";
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
    body->flavor = "By the machinations of a sadist or the trick of a cruel subterranean lake scheme, this shark is now the angry inhabitant of an underground lake.";
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
    body->flavor = "The mollusk's clublike tentacles and titanic bulk are obscured by the depths of the frigid waters $HESHE calls $HISHER home.";
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
    body->flavor = "This scaly humanoid chants warsongs and readies $HISHER trident to defend $HISHER lake.";
    body->baseColor = &darkGreen;
    body->minDamage = 3;
    body->maxDamage = 6;
    body->hp = 18;
    body->dangerLevel = 8;
    body->gender = GenderType::MALE_ONLY;
    body->abilFlags = MA_ATTACKS_PENETRATE;
    body->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLEES_NEAR_DEATH);
    body->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
    body->genFlags = (GF_AQUATIC | GF_AQUATIC_ONLY | GF_SHAMANISTIC);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "sea monk";
    body->flavor = "The sea monk is a mute, disfigured form that lurks beneath the surface of the water for a purpose known only to $HISHER ancient mind.";
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
    body->baseName = "$BASE totem";
    body->flavor = "A $BASE with some amount of magical ability has imbued this wooden totem with shamanistic power.";
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
    body->baseName = "onyx $BASE";
    body->flavor = "This mysterious $BASE is carved completely out of jet-black onyx. It seems to hold some sway over the $BASE nearby.";
    body->baseChar = STATUE_CHAR;
    body->baseColor = &black;
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
    body->baseName = "$BASE obelisk";
    body->flavor = "This sphere-topped pillar gleams menacingly in the darkness, a testament to the arcane power of the $BASE that crafted it.";
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
    body->baseName = "$BASE idol";
    body->flavor = "A priest has constructed this idol in the image of some $BASE saint, and the statue's gem-studded eyes glimmer with malice from beyond.";
    body->baseChar = 0x03C7; // χ
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
    body->baseName = "phantom";
    body->flavor = "The phantom slips through the dungeon invisibly in clear air, leaving behind glowing droplets of ectoplasm and the cries of $HISHER unsuspecting victims.";
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
    body->flavor = "Leaving no visible trace but drops of ectoplasm, the ghost stalks $HISHER living victims completely unsuspected.";
    body->baseColor = &white;
    body->blood = DF_ECTOPLASM_BLOOD;
    body->minDamage = 6;
    body->maxDamage = 9;
    body->hp = 15;
    body->dangerLevel = 11;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_NO_SPECIALS);
    body->flags = (MONST_INVISIBLE);
    body->rarityPercent = 25;
    body->periodicFeatureChance = 15;
    body->periodicFeature = DF_ECTOPLASM_DROPLET;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "golem";
    body->flavor = "A statue animated by an ancient and tireless magic, the golem does not regenerate and attacks with only moderate strength, but $HISHER stone form can withstand incredible damage.";
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
    body->flavor = "Formed of clay by sculptor well-versed in the eldritch arts, this construct can absorb heavy damage for $HISHER master.";
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
    body->flavor = "The zombie is the accursed product of an unholy ritual. Perpetually decaying flesh, hanging from $HISHER bones in shreds, releases a putrid and flammable stench.";
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
    body->flavor = "Emaciated and howling, the ghoul stalks the earth to rend the flesh from living victims. Malodorous flesh hanging from $HISHER gaunt form is enough to induce vomiting.";
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
    body->flavor = "This unholy ghast stalks the deep places of the earth without fear, impervious to conventional attacks.";
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
    body->baseName = "specter";
    body->flavor = "This indistinct, sorrowful form trails after the living from beyond, where it remains immune to conventional attack.";
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
    body->flavor = "This nefarious contraption is embedded in the wall to defend the area.";
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
    body->flavor = "The orb held aloft by this ancient statue sparkles in the dark.";
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
    body->flavor = "Embedded in the wall is a seemingly innocent sphere of mirrored metal.";
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
