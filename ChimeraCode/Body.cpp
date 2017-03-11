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
        reusable(false),
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
        summon(SummonType::NONE),
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
    monster.summon = this->summon;

    monster.baseName = this->baseName;
    monster.gender = this->gender;
    monster.displayColor = this->baseColor;
    monster.baseDisplayChar = this->baseChar;
    monster.lightType = this->light;

    monster.dangerLevel = this->dangerLevel;
    
    monster.baseDisplayChar = this->baseChar;
    monster.baseFlavor = this->flavor;
    
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
    body->flavor = "The rat is a scavenger of the shallows, perpetually in search of decaying animal matter.";
    body->hitMessages = { "scratches", "bites" };
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
    body->hitMessages = { "claws", "bites", "mauls" };
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
    body->hitMessages = { "bites", "nips", "strikes" };
    body->baseColor = &darkGreen;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 5;
    body->dangerLevel = 2;
    body->genFlags = (GF_INSECTOID | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "rhino beetle";
    body->flavor = "Intimidating despite its size, the rhino beetle is outfitted with a chitinous horn and dull black armor plates. $HESHE crawls slowly across the dungeon.";
    body->hitMessages = { "gores", "butts", "jabs" };
    body->baseColor = &darkGray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 5;
    body->maxDamage = 7;
    body->hp = 15;
    body->dangerLevel = 3;
    body->attackSpeed = AttackSpeedType::SLOW;
    body->moveSpeed = MoveSpeedType::SLOW;
    body->genFlags = (GF_INSECTOID | GF_ANIMAL);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "jackal";
    body->flavor = "The jackal prowls the caverns for intruders to rend with $HISHER powerful jaws.";
    body->hitMessages = { "claws", "bites", "mauls" };
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
    body->baseName = "ape";
    body->baseChar = 'A';
    body->flavor = "The ape is a simian of the upper dungeon, and more intelligent (and violent) than the common monkey.";
    body->hitMessages = { "punches", "bites", "nails", "mauls" };
    body->baseColor = &darkGray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 3;
    body->maxDamage = 6;
    body->hp = 12;
    body->dangerLevel = 4;
    body->accuracy = AccuracyType::INACCURATE;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_ANIMAL |GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "hyena";
    body->flavor = "The hyena is an opportunistic predator, stalking the halls by itself or in packs.";
    body->hitMessages = { "claws", "bites", "slashes" };
    body->baseColor = &tanColor;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 4;
    body->hp = 10;
    body->moveSpeed = MoveSpeedType::FAST;
    body->regenSpeed = RegenSpeedType::FAST;
    body->dangerLevel = 4;
    body->genFlags = (GF_ANIMAL);
    body->flags = (MONST_FLEES_NEAR_DEATH);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "goblin";
    body->flavor = "A filthy little primate, the tribalistic goblin often travels in packs.";
    body->hitMessages = { "stabs", "punches", "slashes" };
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
    body->hitMessages = { "stabs", "hits", "slashes" };
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
    body->hitMessages = { "stabs", "knifes", "slices" };
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
    body->hitMessages = { "slams", "slimes" };
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
    body->hitMessages = { "nips", "bites", "slashes" };
    body->baseColor = &gray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 18;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 5;
    body->genFlags = (GF_ANIMAL);
    body->rarityPercent = 66;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "mound";
    body->flavor = "The mound squelches softly across the ground, seeking anything $HESHE can find for a meal.";
    body->hitMessages = { "slimes", "slams", "rams" };
    body->baseColor = &green;
    body->baseChar = 'a';
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 15;
    body->accuracy = AccuracyType::INACCURATE;
    body->dangerLevel = 2;
    body->genFlags = (GF_AMORPHOUS | GF_BURSTS | GF_BRAINLESS);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "millipede";
    body->flavor = "This giant millipede is covered in a array of ever-flailing legs.";
    body->hitMessages = { "rakes", "bites", "stings" };
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
    body->hitMessages = { "batters", "pummels", "beats" };
    body->baseChar = 'O';
    body->flavor = "This lumbering creature carries a $WEAPON that $HESHE can swing with incredible force.";
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
    body->hitMessages = { "nails", "squeezes", "grasps" };
    body->baseColor = &darkGreen;
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 3;
    body->maxDamage = 4;
    body->hp = 55;
    body->regenSpeed = RegenSpeedType::FAST;
    body->dangerLevel = 14;
    body->abilFlags = MA_SEIZES;
    body->genFlags = (GF_AQUATIC | GF_AMORPHOUS | GF_UNDEAD);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "arachnid";
    body->flavor = "The arachnid's eight skittering legs propel its malformed, chitinous body across the dungeon floor.";
    body->hitMessages = { "bites", "stings", "pricks" };
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
    body->flavor = "Little more than a puff of cold fire, the wisp pulses in time to an ethereal rhythm";
    body->hitMessages = { "lashes", "chills", "sears" };
    body->baseChar = '*';
    body->baseColor = &white;
    body->blood = DF_ASH_BLOOD;
    body->minDamage = 5;
    body->maxDamage = 8;
    body->hp = 20;
    body->accuracy = AccuracyType::ACCURATE;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_BRAINLESS);
    body->dangerLevel = 11;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "wraith";
    body->flavor = "The wraith's hollow eye sockets stare hungrily at the world from $HISHER emaciated frame, $HISHER nails groping ceaselessly for a fresh victim.";
    body->hitMessages = { "clutches", "bites", "claws" };
    body->baseChar = 'W';
    body->baseColor = &gray;
    body->blood = DF_ECTOPLASM_BLOOD;
    body->minDamage = 6;
    body->maxDamage = 13;
    body->hp = 50;
    body->moveSpeed = MoveSpeedType::FAST;
    body->dangerLevel = 13;
    body->genFlags = (GF_ARMED | GF_UNDEAD);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "skeleton";
    body->flavor = "With $HISHER skull frozen in a rictus of death, the skeleton is a malevolent pile of bones held together by rusty wire and evil magic.";
    body->hitMessages = { "clubs", "knocks", "whacks" };
    body->baseChar = 'z';
    body->baseColor = &white;
    body->blood = DF_BONES;
    body->minDamage = 4;
    body->maxDamage = 9;
    body->hp = 50;
    body->dangerLevel = 13;
    body->genFlags = (GF_ARMED | GF_WIZARDLY | GF_SUPPORTS_CLASS | GF_UNDEAD);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "blob";
    body->flavor = "The blob is a hungry, gelatinous mass that can take quite the beating, perhaps because of its lack of any discernible features.";
    body->hitMessages = { "slimes", "slams", "butts" };
    body->baseColor = &darkPurple;
    body->blood = DF_PURPLE_BLOOD;
    body->minDamage = 5;
    body->maxDamage = 10;
    body->hp = 50;
    body->defense = DefenseType::LOW;
    body->dangerLevel = 10;
    body->genFlags = (GF_AQUATIC | GF_AMORPHOUS | GF_BURSTS | GF_BRAINLESS);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "vampire";
    body->flavor = "The vampire lives a solitary life deep underground, consuming any warm-blooded creature unfortunate to venture near $HISHER lair.";
    body->hitMessages = { "bites", "grazes", "rakes" };
    body->baseChar = 'V';
    body->baseColor = &gray;
    body->minDamage = 5;
    body->maxDamage = 10;
    body->hp = 30;
    body->dangerLevel = 9;
    body->abilFlags = (MA_TRANSFERENCE);
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY | GF_UNDEAD);
    body->rarityPercent = 25;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "giant";
    body->flavor = "This burly creature is almost too big to fit through the dungeon tunnels, and $HESHE seems to regenerate wounds as quickly as they occur.";
    body->hitMessages = { "bludgeons", "beats", "hammers" };
    body->baseChar = 'G';
    body->baseColor = &darkGreen;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 10;
    body->maxDamage = 15;
    body->hp = 65;
    body->regenSpeed = RegenSpeedType::VERY_FAST;
    body->dangerLevel = 15;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_SUPPORTS_CLASS | GF_BOSSLIKE);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "lizardman";
    body->flavor = "Half reptile and half human, the lizardmen live in the bellies of lakes or else deep underground caves. They do not take kindly to visitors.";
    body->hitMessages = { "bites", "scratches", "tailwhips" };
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
    body->hitMessages = { "stabs", "cuts", "jabs" };
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
    body->hitMessages = { "kicks", "charges" };
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
    body->hitMessages = { "bites", "savages", "pounces on" };
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
    body->hitMessages = { "whacks", "slams", "batters" };
    body->baseChar = 'U';
    body->baseColor = &gray;
    body->blood = DF_WORM_BLOOD;
    body->minDamage = 18;
    body->maxDamage = 22;
    body->hp = 80;
    body->moveSpeed = MoveSpeedType::SLOW;
    body->attackSpeed = AttackSpeedType::SLOW;
    body->dangerLevel = 13;
    body->genFlags = (GF_ANIMAL | GF_INSECTOID | GF_AQUATIC | GF_DIGGER | GF_BRAINLESS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "armor";
    body->flavor = "A suit of battered and pockmarked armor floats through the air, $HISHER $WEAPON swinging blindly before $HIMHER.";
    body->hitMessages = { "strikes", "hits", "chops" };
    body->baseChar = ']';
    body->baseColor = &gray;
    body->minDamage = 9;
    body->maxDamage = 13;
    body->hp = 60;
    body->dangerLevel = 18;
    body->genFlags = (GF_BOSSLIKE | GF_BRAINLESS);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "fury";
    body->flavor = "A creature of inchoate rage made flesh, the fury's moist wings beat loudly in the darkness.";
    body->hitMessages = { "fustigates", "drubs", "castigates" };
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
    body->baseName = "demon";
    body->flavor = "The demon is a fallen angel, atoning for its sins in heaven by scouring the darkest places of earth.";
    body->hitMessages = { "smacks", "slaps", "hits", "claws" };
    body->baseChar = 'D';
    body->baseColor = &darkRed;
    body->minDamage = 15;
    body->maxDamage = 20;
    body->hp = 80;
    body->dangerLevel = 21;
    body->flags = (GF_WIZARDLY | GF_BOSSLIKE | GF_ARMED);
    body->rarityPercent = 33;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "scorpion";
    body->flavor = "A merciless predator of the dark, this collosal arachnid crushes $HISHER prey to bits in $HISHER might incisors.";
    body->hitMessages = { "scissors", "chomps", "stings" };
    body->baseChar = 'S';
    body->baseColor = &tanColor;
    body->minDamage = 25;
    body->maxDamage = 30;
    body->hp = 100;
    body->dangerLevel = 23;
    body->defense = DefenseType::LOW;
    body->genFlags = (GF_INSECTOID | GF_BOSSLIKE | GF_DIGGER);
    bodies.push_back(body);

    body = new Body();
    body->baseName = "horror";
    body->flavor = "This seething, towering nightmare of bone and sinew slinks through the bowels of the world. The horror's regeneration and strength are unparalleled.";
    body->hitMessages = { "batters", "assaults", "crashes into", "steamrolls over", "rains down upon" };
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
    body->hitMessages = { "tailwhips", "thrashes", "slams" };
    body->baseChar = 'W';
    body->baseColor = &darkRed;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 25;
    body->maxDamage = 40;
    body->hp = 140;
    body->dangerLevel = 28;
    body->gender = GenderType::BOTH;
    body->flags = (MONST_CARRY_ITEM_100);
    body->genFlags = (GF_WIZARDLY | GF_AQUATIC | GF_BOSSLIKE | GF_DIGGER);
    body->abilFlags = MA_ATTACKS_ALL_ADJACENT;
    bodies.push_back(body);

    body = new Body();
    body->baseName = "jelly";
    body->flavor = "This mass of caustic pink goo slips across the ground in search of a warm meal.";
    body->hitMessages = { "dissolves", "soaks", "splashes" };
    body->baseColor = &pink;
    body->blood = DF_GREEN_BLOOD;
    body->minDamage = 1;
    body->maxDamage = 3;
    body->hp = 38;
    body->dangerLevel = 6;
    body->abilFlags = (MA_CLONE_SELF_ON_DEFEND);
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_AMORPHOUS | GF_NO_SPECIALS | GF_DIGGER | GF_BRAINLESS);
    body->rarityPercent = 100;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "pudding";
    body->flavor = "The rare cousin of the jelly, the pudding is bigger, stronger, and hungrier to boot.";
    body->hitMessages = { "dissolves", "burns", "drenches" };
    body->baseColor = &black;
    body->blood = DF_PURPLE_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 7;
    body->hp = 100;
    body->dangerLevel = 19;
    body->abilFlags = (MA_CLONE_SELF_ON_DEFEND);
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_AMORPHOUS | GF_NO_SPECIALS | GF_DIGGER | GF_BRAINLESS);
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "bloat";
    body->flavor = "A bladder of gas buoys the bloat through the air, $HISHER thin veinous membrane ready to rupture at the slightest stress.";
    body->hitMessages = { "bumps" };
    body->baseColor = &darkPurple;
    body->blood = DF_PURPLE_BLOOD;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 4;
    body->dangerLevel = 1;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->genFlags = (GF_BURSTS | GF_KAMIKAZE | GF_BRAINLESS);
    body->rarityPercent = 66;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "spore crab";
    body->flavor = "A drab, scuttling creature, the spore crab would be unremarkable if not for the ominous pustules protruding from its fragile shell...";
    body->hitMessages = { "nips at" };
    body->baseChar = 'c';
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 2;
    body->dangerLevel = 3;
    body->moveSpeed = MoveSpeedType::FAST;
    body->genFlags = (GF_BURSTS | GF_KAMIKAZE | GF_ANIMAL | GF_BRAINLESS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "kamikaze dwarf";
    body->flavor = "Bellowing prayers in some foreign tongue, this zealot dwarf charges across the ground with a backpack full of dangerous, reactive compounds.";
    body->hitMessages = { "shouts at" };
    body->baseColor = &brown;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 14;
    body->dangerLevel = 4;
    body->gender = GenderType::BOTH;
    body->genFlags = (GF_BURSTS | GF_KAMIKAZE);
    body->rarityPercent = 33;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "monkey";
    body->flavor = "Mischievous trickster that $HESHE is, the monkey lives to steal shiny trinkets from passing adventurers.";
    body->hitMessages = { "punches", "tweaks", "bites" };
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
    body->hitMessages = { "punches", "scratches", "scuffs" };
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
    body->hitMessages = { "dives at", "scratches", "pecks" };
    body->baseColor = &darkGray;
    body->blood = DF_RED_BLOOD;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 20;
    body->dangerLevel = 6;
    body->flags = (MONST_FLIES | MONST_FLITS);
    body->genFlags = (GF_THIEVING | GF_ANIMAL);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "magpie";
    body->flavor = "An unusual occupant of the dungeon, the magpie flits by in search of shiny treasure.";
    body->hitMessages = { "scratches", "pokes", "pecks" };
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
    body->hitMessages = { "whacks", "flails at", "swipes" };
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
    body->flavor = "The gentle turtle is more than capable of defending its home waters by applying its sharp beak to intruders.";
    body->hitMessages = { "bites", "rams", "pecks" };
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
    body->hitMessages = { "bites", "shocks" };
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
    body->hitMessages = { "snaps at", "bites", "rends" };
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
    body->hitMessages = { "chomps", "slams", "bites" };
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
    body->hitMessages = { "slaps", "bites", "batters" };
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
    body->hitMessages = { "impales", "stabs", "skewers" };
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
    body->hitMessages = { "bashes", "clubs", "beats" };
    body->baseChar = 0x00B1; // ±
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
    body->flavor = "Some $BASE with an amount of magical ability has imbued this totem with shamanistic power.";
    body->hitMessages = { "gleams at" };
    body->baseChar = TOTEM_CHAR;
    body->baseColor = &green;
    body->blood = DF_RUBBLE_BLOOD;
    body->hp = 30;
    body->attackSpeed = AttackSpeedType::TOTEM;
    body->flags = (MONST_TURRET);
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_TOTEM | GF_SHAMANISTIC | GF_BRAINLESS);
    body->reusable = true;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "onyx $BASE";
    body->flavor = "This mysterious $BASE is carved completely out of jet-black onyx. It seems to hold some sway over the $BASE nearby.";
    body->hitMessages = { "gleams at" };
    body->baseChar = STATUE_CHAR;
    body->baseColor = &black;
    body->blood = DF_RUBBLE_BLOOD;
    body->hp = 30;
    body->attackSpeed = AttackSpeedType::TOTEM;
    body->flags = (MONST_TURRET);
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_TOTEM | GF_SHAMANISTIC | GF_ANIMAL | GF_BRAINLESS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "$BASE obelisk";
    body->flavor = "This sphere-topped pillar gleams menacingly in the darkness, a testament to the arcane power of the $BASE that crafted it.";
    body->hitMessages = { "gleams at" };
    body->baseChar = 0x03AA; // Ϊ
    body->baseColor = &green;
    body->blood = DF_RUBBLE_BLOOD;
    body->hp = 30;
    body->attackSpeed = AttackSpeedType::TOTEM;
    body->flags = (MONST_TURRET);
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_TOTEM | GF_WIZARDLY | GF_BRAINLESS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "$BASE idol";
    body->flavor = "A priest has constructed this idol in the image of some $BASE saint, and the statue's gem-studded eyes glimmer with malice from beyond.";
    body->hitMessages = { "gleams at" };
    body->baseChar = 0x03C7; // χ
    body->baseColor = &green;
    body->blood = DF_RUBBLE_BLOOD;
    body->hp = 30;
    body->attackSpeed = AttackSpeedType::TOTEM;
    body->flags = (MONST_TURRET);
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->genFlags = (GF_TOTEM | GF_WIZARDLY | GF_BRAINLESS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "phantom";
    body->flavor = "The phantom slips through the dungeon invisibly in clear air, leaving behind glowing droplets of ectoplasm and the cries of $HISHER unsuspecting victims.";
    body->hitMessages = { "hits" };
    body->baseColor = &white;
    body->blood = DF_ECTOPLASM_BLOOD;
    body->minDamage = 12;
    body->maxDamage = 18;
    body->hp = 35;
    body->dangerLevel = 21;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_NO_SPECIALS | GF_UNDEAD);
    body->flags = MONST_INVISIBLE;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "ghost";
    body->flavor = "Leaving no visible trace but drops of ectoplasm, the ghost stalks $HISHER living victims completely unsuspected.";
    body->hitMessages = { "hits" };
    body->baseColor = &white;
    body->blood = DF_ECTOPLASM_BLOOD;
    body->minDamage = 6;
    body->maxDamage = 9;
    body->hp = 15;
    body->dangerLevel = 11;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_NO_SPECIALS | GF_DIGGER | GF_UNDEAD);
    body->flags = (MONST_INVISIBLE);
    body->rarityPercent = 25;
    body->periodicFeatureChance = 15;
    body->periodicFeature = DF_ECTOPLASM_DROPLET;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "golem";
    body->flavor = "A statue animated by an ancient and tireless magic, the golem does not regenerate and attacks with only moderate strength, but $HISHER stone form can withstand incredible damage.";
    body->hitMessages = { "backhands", "punches", "kicks" };
    body->baseChar = 'G';
    body->baseColor = &gray;
    body->blood = DF_RUBBLE_BLOOD;
    body->minDamage = 4;
    body->maxDamage = 8;
    body->hp = 350;
    body->regenSpeed = RegenSpeedType::NONE;
    body->dangerLevel = 26;
    body->flags = (MONST_DIES_IF_NEGATED);
    body->genFlags = (GF_NO_SPECIALS | GF_BOSSLIKE |GF_DIGGER);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "construct";
    body->flavor = "Formed of clay by sculptor well-versed in the eldritch arts, the construct can absorb heavy damage for $HISHER master.";
    body->hitMessages = { "knees", "punches", "knocks" };
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
    body->hitMessages = { "hits", "bites", "nails" };
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
    body->genFlags = (GF_NO_SPECIALS | GF_BOSSLIKE | GF_UNDEAD);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "ghoul";
    body->flavor = "Emaciated and howling, the ghoul stalks the earth to rend the flesh from living victims. Malodorous flesh hanging from $HISHER gaunt form is enough to induce vomiting.";
    body->hitMessages = { "rakes", "bites", "claws" };
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
    body->genFlags = (GF_NO_SPECIALS | GF_DIGGER);
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "revenant";
    body->flavor = "This unholy ghast stalks the deep places of the earth without fear, impervious to conventional attacks.";
    body->hitMessages = { "hits", "slashes" };
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
    body->hitMessages = { "touches" };
    body->baseColor = &ectoplasmColor;
    body->baseChar = 'G';
    body->minDamage = 8;
    body->maxDamage = 12;
    body->hp = 15;
    body->dangerLevel = 14;
    body->defense = DefenseType::DEFENSELESS;
    body->moveSpeed = MoveSpeedType::SLOW;
    body->genFlags = (GF_NO_SPECIALS | GF_UNDEAD);
    body->flags = MONST_IMMUNE_TO_WEAPONS;
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "sorceror";
    body->baseChar = 'S';
    body->flavor = "The dessicated form of a singularly foul summoner who holds dominion over the nether planes.";
    body->hitMessages = { "bops", "bludgeons" };
    body->baseColor = &white;
    body->minDamage = 2;
    body->maxDamage = 6;
    body->hp = 35;
    body->abilFlags = MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25;
    body->summon = SummonType::SPAWN_UNRELATED_MOOK;
    body->defense = DefenseType::HIGH;
    body->light = LICH_LIGHT;
    body->genFlags = (GF_NO_SPECIALS | GF_WIZARDLY);
    body->dangerLevel = 30;
    body->rarityPercent = 33;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "rat king";
    body->baseChar = 'K';
    body->flavor = "Many forms writhe and twist beneath the purple rags of the rat king, and bursting from the seams pour dozens of rats, snakes, worms, birds and all variety of fauna.";
    body->hitMessages = { "engulfs", "swarms" };
    body->baseColor = &brown;
    body->minDamage = 7;
    body->maxDamage = 10;
    body->hp = 35;
    body->abilFlags = MONST_MAINTAINS_DISTANCE;
    body->summon = SummonType::SPAWN_FODDER;
    body->defense = DefenseType::HIGH;
    body->genFlags = (GF_SHAMANISTIC | GF_PACK_MEMBER | GF_UNDEAD | GF_DIGGER);
    body->dangerLevel = 15;
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "turret";
    body->flavor = "This nefarious contraption is embedded in the wall to defend the area.";
    body->hitMessages = { "shoots" };
    body->baseColor = &gray;
    body->baseChar = TURRET_CHAR;
    body->minDamage = 0;
    body->maxDamage = 0;
    body->hp = 30;
    body->dangerLevel = 0;
    body->defense = DefenseType::DEFENSELESS;
    body->attackSpeed = AttackSpeedType::TURRET;
    body->genFlags = (GF_TURRET | GF_BRAINLESS);
    body->regenSpeed = RegenSpeedType::NONE;
    body->flags = MONST_TURRET;
    body->reusable = true;
    body->rarityPercent = 100;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "statue";
    body->flavor = "The orb held aloft by this ancient statue sparkles in the dark.";
    body->hitMessages = { "gleams at" };
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
    body->regenSpeed = RegenSpeedType::NONE;
    body->flags = (MONST_TURRET | MONST_DIES_IF_NEGATED | GF_BRAINLESS);
    body->reusable = true;
    body->rarityPercent = 25;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "orb";
    body->flavor = "Embedded in the wall is a seemingly innocent sphere of mirrored metal.";
    body->hitMessages = { "gleams at" };
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
    body->regenSpeed = RegenSpeedType::NONE;
    body->flags = (MONST_TURRET | MONST_DIES_IF_NEGATED | GF_BRAINLESS);
    body->reusable = true;
    body->rarityPercent = 12;
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "spectral $SWORD";
    body->flavor = "Eldritch forces have coalesced to form this flickering, ethereal weapon.";
    body->hitMessages = { "nicks" };
    body->baseChar = WEAPON_CHAR;
    body->baseColor = &spectralBladeColor;
    body->flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS |
                   MB_DOES_NOT_TRACK_LEADER | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS | MONST_NOT_LISTED_IN_SIDEBAR);
    body->minDamage = 1;
    body->maxDamage = 1;
    body->accuracy = AccuracyType::FIXED;
    body->hp = 1;
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->moveSpeed = MoveSpeedType::FAST;
    body->light = SPECTRAL_BLADE_LIGHT;
    body->rarityPercent = 66;
    body->genFlags = (GF_CONJURATION | GF_NO_SPECIALS | GF_BRAINLESS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "eldritch $FODDER";
    body->baseChar = '$';
    body->flavor = "Ethereal energies shaped into concrete form zip and flicker in the air.";
    body->hitMessages = { "hits" };
    body->baseColor = &spectralBladeColor;
    body->flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS |
                   MB_DOES_NOT_TRACK_LEADER | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS | MONST_NOT_LISTED_IN_SIDEBAR);
    body->minDamage = 1;
    body->maxDamage = 1;
    body->accuracy = AccuracyType::FIXED;
    body->hp = 1;
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->moveSpeed = MoveSpeedType::FAST;
    body->light = SPECTRAL_BLADE_LIGHT;
    body->genFlags = (GF_CONJURATION | GF_NO_SPECIALS | GF_BRAINLESS);
    bodies.push_back(body);
    
    body = new Body();
    body->baseName = "animate $WEAPON";
    body->flavor = "Eldritch forces have coalesced to animate this ethereal weapon.";
    body->hitMessages = { "cuts" };
    body->baseChar = WEAPON_CHAR;
    body->baseColor = &spectralBladeColor;
    body->flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS |
                   MB_DOES_NOT_TRACK_LEADER | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS | MONST_NOT_LISTED_IN_SIDEBAR);
    body->minDamage = 1;
    body->maxDamage = 1;
    body->accuracy = AccuracyType::ACCURATE;
    body->hp = 3;
    body->defense = DefenseType::DEFENSELESS;
    body->regenSpeed = RegenSpeedType::NONE;
    body->moveSpeed = MoveSpeedType::FAST;
    body->rarityPercent = 33;
    body->genFlags = (GF_CONJURATION | GF_NO_SPECIALS | GF_BRAINLESS);
    bodies.push_back(body);
    
    return bodies;
}
