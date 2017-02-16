/*
 *  MonsterGlobals.c
 *  cBrogue
 *
 *  Created by Aaron King on 9/6/16.
 *  Copyright 2016. All rights reserved.
 */

#include "MonsterGlobals.h"
#include "IncludeGlobals.h"

enum monsterTypes {
    MK_YOU,
    MK_RAT,
    MK_KOBOLD,
    MK_JACKAL,
    MK_EEL,
    MK_MONKEY,
    MK_BLOAT,
    MK_PIT_BLOAT,
    MK_GOBLIN,
    MK_GOBLIN_CONJURER,
    MK_GOBLIN_MYSTIC,
    MK_GOBLIN_TOTEM,
    MK_PINK_JELLY,
    MK_TOAD,
    MK_VAMPIRE_BAT,
    MK_ARROW_TURRET,
    MK_ACID_MOUND,
    MK_CENTIPEDE,
    MK_OGRE,
    MK_BOG_MONSTER,
    MK_OGRE_TOTEM,
    MK_SPIDER,
    MK_SPARK_TURRET,
    MK_WILL_O_THE_WISP,
    MK_WRAITH,
    MK_ZOMBIE,
    MK_TROLL,
    MK_OGRE_SHAMAN,
    MK_NAGA,
    MK_SALAMANDER,
    MK_EXPLOSIVE_BLOAT,
    MK_DAR_BLADEMASTER,
    MK_DAR_PRIESTESS,
    MK_DAR_BATTLEMAGE,
    MK_ACID_JELLY,
    MK_CENTAUR,
    MK_UNDERWORM,
    MK_SENTINEL,
    MK_ACID_TURRET,
    MK_DART_TURRET,
    MK_KRAKEN,
    MK_LICH,
    MK_PHYLACTERY,
    MK_PIXIE,
    MK_PHANTOM,
    MK_FLAME_TURRET,
    MK_IMP,
    MK_FURY,
    MK_REVENANT,
    MK_TENTACLE_HORROR,
    MK_GOLEM,
    MK_DRAGON,
    
    MK_GOBLIN_CHIEFTAN,
    MK_BLACK_JELLY,
    MK_VAMPIRE,
    MK_FLAMEDANCER,
    
    MK_SPECTRAL_BLADE,
    MK_SPECTRAL_IMAGE,
    MK_GUARDIAN,
    MK_WINGED_GUARDIAN,
    MK_CHARM_GUARDIAN,
    MK_WARDEN_OF_YENDOR,
    MK_ELDRITCH_TOTEM,
    MK_MIRRORED_TOTEM,
    
    MK_UNICORN,
    MK_IFRIT,
    MK_PHOENIX,
    MK_PHOENIX_EGG,
    MK_ANCIENT_SPIRIT,
    
    NUMBER_MONSTER_KINDS
};

#define NUMBER_HORDES               168

#define MONSTER_CLASS_COUNT         13

creatureType *catalog;
hordeType *hordeCatalog;
monsterClass *monsterClassCatalog;
blueprint *blueprintCatalog;

void ensureCatalogsInitialized();
void initMonsterCatalog();
void initHordeCatalog();
void initMonsterClassCatalog();
void initBlueprintCatalog();

creatureType *getMonsterCatalog() {
    ensureCatalogsInitialized();
    return catalog;
}

hordeType *getHordeCatalog() {
    ensureCatalogsInitialized();
    return hordeCatalog;
}

monsterClass *getMonsterClassCatalog() {
    ensureCatalogsInitialized();
    return monsterClassCatalog;
}

blueprint *getBlueprintCatalog() {
    ensureCatalogsInitialized();
    return blueprintCatalog;
}

int getRatTrapMonsterId() {
    return MK_RAT;
}

int getWarrenBossMonsterId() {
    return MK_GOBLIN_CHIEFTAN;
}

int getVampireBossMonsterId() {
    return MK_VAMPIRE;
}

int getWingedGuardianMonsterId() {
    return MK_WINGED_GUARDIAN;
}

int getGuardianMonsterId() {
    return MK_GUARDIAN;
}

int getSentinelMonsterId() {
    return MK_SENTINEL;
}

int getWardenMonsterId() {
    return MK_WARDEN_OF_YENDOR;
}

int getMirrorMonsterId() {
    return MK_MIRRORED_TOTEM;
}

int getWebberMonsterId() {
    return MK_SPIDER;
}

int getDiggerMonsterId() {
    return MK_UNDERWORM;
}

int getSparkMonsterId() {
    return MK_SPARK_TURRET;
}

int getUndeadMonsterId() {
    return MK_ZOMBIE;
}

int getInvisibleMonsterId() {
    return MK_PHANTOM;
}

void ensureCatalogsInitialized() {
    static boolean catalogInitialized = false;
    if (catalogInitialized) {
        return;
    }
    
    initMonsterCatalog();
    initHordeCatalog();
    initMonsterClassCatalog();
    initBlueprintCatalog();
    
    catalogInitialized = true;
}

void initMonsterCatalog() {
    
    catalog = (creatureType *)malloc(sizeof(creatureType) * NUMBER_MONSTER_KINDS);
    memset(catalog, 0, sizeof(creatureType) * NUMBER_MONSTER_KINDS);
    
    unsigned int id;
    
    //   id                           name           ch      color           HP      def     acc     damage          reg move    attack  blood           light   DFChance DFType    bolts
    id = MK_YOU;
    catalog[id] = (creatureType) {0, "you",  PLAYER_CHAR,    &playerInLightColor,30, 0,      100,    {1, 2, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0};
    catalog[id].flags = (MONST_MALE | MONST_FEMALE);
    strcpy(catalog[id].flavorText, "A naked adventurer in an unforgiving place, bereft of equipment and confused about the circumstances.");
    strcpy(catalog[id].absorbing, "studying");
    strcpy(catalog[id].absorbStatus, "Studying");
    strcpy(catalog[id].attack[0], "hit");
    
    id = MK_RAT;
    catalog[id] = (creatureType) {0, "rat",          'r',    &gray,          6,      0,      80,     {1, 3, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      1,      DF_URINE};
    strcpy(catalog[id].flavorText, "The rat is a scavenger of the shallows, perpetually in search of decaying animal matter.");
    strcpy(catalog[id].absorbing, "gnawing at");
    strcpy(catalog[id].absorbStatus, "Eating");
    strcpy(catalog[id].attack[0], "scratches");
    strcpy(catalog[id].attack[1], "bites");
    
    id = MK_KOBOLD;
    catalog[id] = (creatureType) {0, "kobold",       'k',    &goblinColor,   7,      0,      80,     {1, 4, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0};
    strcpy(catalog[id].flavorText, "The kobold is a lizardlike humanoid of the upper dungeon.");
    strcpy(catalog[id].absorbing, "poking at");
    strcpy(catalog[id].absorbStatus, "Examining");
    strcpy(catalog[id].attack[0], "clubs");
    strcpy(catalog[id].attack[1], "bashes");
    
    id = MK_JACKAL;
    catalog[id] = (creatureType) {0, "jackal",       'j',    &jackalColor,   8,      0,      70,     {2, 4, 1},      20, 50,     100,    DF_RED_BLOOD,   0,      1,      DF_URINE};
    strcpy(catalog[id].flavorText, "The jackal prowls the caverns for intruders to rend with $HISHER powerful jaws.");
    strcpy(catalog[id].absorbing, "tearing at");
    strcpy(catalog[id].absorbStatus, "Eating");
    strcpy(catalog[id].attack[0], "claws");
    strcpy(catalog[id].attack[1], "bites");
    strcpy(catalog[id].attack[2], "mauls");
    
    id = MK_EEL;
    catalog[id] = (creatureType) {0, "eel",          'e',    &eelColor,      18,     27,     100,    {3, 7, 2},      5,  50,     100,    0,              0,      0,      0};
    catalog[id].flags = (MONST_RESTRICTED_TO_LIQUID | MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS | MONST_NEVER_SLEEPS);
    strcpy(catalog[id].flavorText, "The eel slips silently through the subterranean lake, waiting for unsuspecting prey to set foot in $HISHER dark waters.");
    strcpy(catalog[id].absorbing, "eating");
    strcpy(catalog[id].absorbStatus, "Eating");
    strcpy(catalog[id].attack[0], "shocks");
    strcpy(catalog[id].attack[1], "bites");
    
    id = MK_MONKEY;
    catalog[id] = (creatureType) {0, "monkey",       'm',    &ogreColor,     12,     17,     100,    {1, 3, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      1,      DF_URINE},
    catalog[id].abilityFlags = (MA_HIT_STEAL_FLEE);
    strcpy(catalog[id].flavorText, "Mischievous trickster that $HESHE is, the monkey lives to steal shiny trinkets from passing adventurers.");
    strcpy(catalog[id].absorbing, "examining");
    strcpy(catalog[id].absorbStatus, "Examining");
    strcpy(catalog[id].attack[0], "tweaks");
    strcpy(catalog[id].attack[1], "bites");
    strcpy(catalog[id].attack[2], "punches");
    
    id = MK_BLOAT;
    catalog[id] = (creatureType) {0, "bloat",        'b',    &poisonGasColor,4,      0,      100,    {0, 0, 0},      5,  100,    100,    DF_PURPLE_BLOOD,0,      0,      DF_BLOAT_DEATH},
    catalog[id].flags = (MONST_FLIES | MONST_FLITS);
    catalog[id].abilityFlags = (MA_KAMIKAZE | MA_DF_ON_DEATH);
    strcpy(catalog[id].flavorText, "A bladder of deadly gas buoys the bloat through the air, $HISHER thin veinous membrane ready to rupture at the slightest stress.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "bumps");
    strcpy(catalog[id].DFMessage, "bursts, leaving behind an expanding cloud of caustic gas!");
    
    id = MK_PIT_BLOAT;
    catalog[id] = (creatureType) {0, "pit bloat",    'b',    &lightBlue,     4,      0,      100,    {0, 0, 0},      5,  100,    100,    DF_PURPLE_BLOOD,0,      0,      DF_HOLE_POTION},
    catalog[id].flags = (MONST_FLIES | MONST_FLITS);
    catalog[id].abilityFlags = (MA_KAMIKAZE | MA_DF_ON_DEATH);
    strcpy(catalog[id].flavorText, "This rare subspecies of bloat is filled with a peculiar vapor that, if released, will cause the floor to vanish out from underneath $HIMHER.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "bumps");
    strcpy(catalog[id].DFMessage, "bursts, causing the floor underneath $HIMHER to disappear!");
    
    id = MK_GOBLIN;
    catalog[id] = (creatureType) {0, "goblin",       'g',    &goblinColor,   15,     10,     70,     {2, 5, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0},
    catalog[id].abilityFlags = (MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "A filthy little primate, the tribalistic goblin often travels in packs and carries a makeshift stone spear.");
    strcpy(catalog[id].absorbing, "chanting over");
    strcpy(catalog[id].absorbStatus, "Chanting");
    strcpy(catalog[id].attack[0], "cuts");
    strcpy(catalog[id].attack[1], "stabs");
    strcpy(catalog[id].attack[2], "skewers");
    
    id = MK_GOBLIN_CONJURER;
    catalog[id] = (creatureType) {0, "goblin conjurer",'g',  &goblinConjurerColor, 10,10,    70,     {2, 4, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0},
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CAST_SPELLS_SLOWLY | MONST_CARRY_ITEM_25);
    catalog[id].abilityFlags = (MA_CAST_SUMMON | MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "This goblin is covered with glowing sigils that pulse with power. $HESHE can call into existence phantom blades to attack $HISHER foes.");
    strcpy(catalog[id].absorbing, "performing a ritual on");
    strcpy(catalog[id].absorbStatus, "Performing ritual");
    strcpy(catalog[id].attack[0], "thumps");
    strcpy(catalog[id].attack[1], "whacks");
    strcpy(catalog[id].attack[2], "wallops");
    strcpy(catalog[id].summonMessage, "gestures ominously!");
    
    id = MK_GOBLIN_MYSTIC;
    catalog[id] = (creatureType) {0, "goblin mystic",'g',    &goblinMysticColor, 10, 10,     70,     {2, 4, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,     {BOLT_SHIELDING}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    catalog[id].abilityFlags = (MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "This goblin carries no weapon, and $HISHER eyes sparkle with golden light. $HESHE can invoke a powerful shielding magic to protect $HISHER escorts from harm.");
    strcpy(catalog[id].absorbing, "performing a ritual on");
    strcpy(catalog[id].absorbStatus, "Performing ritual");
    strcpy(catalog[id].attack[0], "slaps");
    strcpy(catalog[id].attack[1], "punches");
    strcpy(catalog[id].attack[2], "kicks");
    
    id = MK_GOBLIN_TOTEM;
    catalog[id] = (creatureType) {0, "goblin totem", TOTEM_CHAR, &orange,    30,     0,      0,      {0, 0, 0},      0,  100,    300,    DF_RUBBLE_BLOOD,IMP_LIGHT,0,    0,     {BOLT_HASTE, BOLT_SPARK}};
    catalog[id].flags = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS);
    strcpy(catalog[id].flavorText, "Goblins have created this makeshift totem and imbued $HIMHER with a shamanistic power.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "hits");
    
    id = MK_PINK_JELLY;
    catalog[id] = (creatureType) {0, "pink jelly",   'J',    &pinkJellyColor,50,     0,      85,     {1, 3, 1},      0,  100,    100,    DF_PURPLE_BLOOD,0,      0,      0};
    catalog[id].flags = (MONST_NEVER_SLEEPS);
    catalog[id].abilityFlags = (MA_CLONE_SELF_ON_DEFEND);
    strcpy(catalog[id].flavorText, "This mass of caustic pink goo slips across the ground in search of a warm meal.");
    strcpy(catalog[id].absorbing, "absorbing");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "smears");
    strcpy(catalog[id].attack[1], "slimes");
    strcpy(catalog[id].attack[2], "drenches");
    
    id = MK_TOAD;
    catalog[id] = (creatureType) {0, "toad",         't',    &toadColor,     18,     0,      90,     {1, 4, 1},      10, 100,    100,    DF_GREEN_BLOOD, 0,      0,      0};
    catalog[id].abilityFlags = (MA_HIT_HALLUCINATE);
    strcpy(catalog[id].flavorText, "The enormous, warty toad secretes a powerful hallucinogenic slime to befuddle the senses of any creatures that come in contact with $HIMHER.");
    strcpy(catalog[id].absorbing, "eating");
    strcpy(catalog[id].absorbStatus, "Eating");
    strcpy(catalog[id].attack[0], "slimes");
    strcpy(catalog[id].attack[1], "slams");
    
    id = MK_VAMPIRE_BAT;
    catalog[id] = (creatureType) {0, "vampire bat",  'v',    &gray,          18,     25,     100,    {2, 6, 1},      20, 50,     100,    DF_RED_BLOOD,   0,      0,      0};
    catalog[id].flags = (MONST_FLIES | MONST_FLITS);
    catalog[id].abilityFlags = (MA_TRANSFERENCE);
    strcpy(catalog[id].flavorText, "Often hunting in packs, leathery wings and keen senses guide the vampire bat unerringly to $HISHER prey.");
    strcpy(catalog[id].absorbing, "draining");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "nips");
    strcpy(catalog[id].attack[1], "bites");
    
    id = MK_ARROW_TURRET;
    catalog[id] = (creatureType) {0, "arrow turret", TURRET_CHAR,&black,     30,     0,      90,     {2, 6, 1},      0,  100,    250,    0,              0,      0,      0,     {BOLT_DISTANCE_ATTACK}};
    catalog[id].flags = (MONST_TURRET);
    strcpy(catalog[id].flavorText, "A mechanical contraption embedded in the wall, the spring-loaded arrow turret will fire volley after volley of arrows at intruders.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "shoots");
    
    id = MK_ACID_MOUND;
    catalog[id] = (creatureType) {0, "acid mound",   'a',    &acidBackColor, 15,     10,     70,     {1, 3, 1},      5,  100,    100,    DF_ACID_BLOOD,  0,      0,      0};
    catalog[id].flags = (MONST_DEFEND_DEGRADE_WEAPON);
    catalog[id].abilityFlags = (MA_HIT_DEGRADE_ARMOR);
    strcpy(catalog[id].flavorText, "The acid mound squelches softly across the ground, leaving a trail of acidic goo in $HISHER path.");
    strcpy(catalog[id].absorbing, "liquefying");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "slimes");
    strcpy(catalog[id].attack[1], "douses");
    strcpy(catalog[id].attack[2], "drenches");
    
    id = MK_CENTIPEDE;
    catalog[id] = (creatureType) {0, "centipede",    'c',    &centipedeColor,20,     20,     80,     {4, 12, 1},     20, 100,    100,    DF_GREEN_BLOOD, 0,      0,      0};
    catalog[id].abilityFlags = (MA_CAUSES_WEAKNESS);
    strcpy(catalog[id].flavorText, "This monstrous centipede's incisors are imbued with a horrible venom that will slowly kill $HISHER prey.");
    strcpy(catalog[id].absorbing, "eating");
    strcpy(catalog[id].absorbStatus, "Eating");
    strcpy(catalog[id].attack[0], "pricks");
    strcpy(catalog[id].attack[1], "stings");
    
    id = MK_OGRE;
    catalog[id] = (creatureType) {0, "ogre",         'O',    &ogreColor,     55,     60,     125,    {9, 13, 2},     20, 100,    200,    DF_RED_BLOOD,   0,      0,      0};
    catalog[id].flags = (MONST_MALE | MONST_FEMALE);
    catalog[id].abilityFlags = (MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "This lumbering creature carries an enormous club that $HESHE can swing with incredible force.");
    strcpy(catalog[id].absorbing, "examining");
    strcpy(catalog[id].absorbStatus, "Studying");
    strcpy(catalog[id].attack[0], "cudgels");
    strcpy(catalog[id].attack[1], "clubs");
    strcpy(catalog[id].attack[2], "batters");
    
    id = MK_BOG_MONSTER;
    catalog[id] = (creatureType) {0, "bog monster",  'B',    &krakenColor,   55,     60,     5000,   {3, 4, 1},      3,  200,    100,    0,              0,      0,      0},
    catalog[id].flags = (MONST_RESTRICTED_TO_LIQUID | MONST_SUBMERGES | MONST_FLITS | MONST_FLEES_NEAR_DEATH);
    catalog[id].abilityFlags = (MA_SEIZES);
    strcpy(catalog[id].flavorText, "The horrifying bog monster dwells beneath the surface of mud-filled swamps. When $HISHER prey ventures into the mud, the bog monster will ensnare the unsuspecting victim in $HISHER pale tentacles and squeeze its life away.");
    strcpy(catalog[id].absorbing, "draining");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "squeezes");
    strcpy(catalog[id].attack[1], "strangles");
    strcpy(catalog[id].attack[2], "crushes");
    
    id = MK_OGRE_TOTEM;
    catalog[id] = (creatureType) {0, "ogre totem",   TOTEM_CHAR, &green,     70,     0,      0,      {0, 0, 0},      0,  100,    400,    DF_RUBBLE_BLOOD,LICH_LIGHT,0,   0,     {BOLT_HEALING, BOLT_SLOW_2}};
    catalog[id].flags = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS);
    strcpy(catalog[id].flavorText, "Ancient ogres versed in the eldritch arts have assembled this totem and imbued $HIMHER with occult power.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "hits");
    
    id = MK_SPIDER;
    catalog[id] = (creatureType) {0, "spider",       's',    &white,         20,     70,     90,     {3, 4, 2},      20, 100,    200,    DF_GREEN_BLOOD, 0,      0,      0,     {BOLT_SPIDERWEB}};
    catalog[id].flags = (MONST_IMMUNE_TO_WEBS | MONST_CAST_SPELLS_SLOWLY | MONST_ALWAYS_USE_ABILITY);
    catalog[id].abilityFlags = (MA_POISONS);
    strcpy(catalog[id].flavorText, "The spider's red eyes pierce the darkness in search of enemies to ensnare with $HISHER projectile webs and dissolve with deadly poison.");
    strcpy(catalog[id].absorbing, "draining");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "bites");
    strcpy(catalog[id].attack[1], "stings");
    
    id = MK_SPARK_TURRET;
    catalog[id] = (creatureType) {0, "spark turret", TURRET_CHAR, &lightningColor,80,0,      100,    {0, 0, 0},      0,  100,    150,    0,              SPARK_TURRET_LIGHT, 0,  0, {BOLT_SPARK}};
    catalog[id].flags = (MONST_TURRET);
    strcpy(catalog[id].flavorText, "This contraption hums with electrical charge that $HISHER embedded crystals and magical sigils can direct at intruders in deadly arcs.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "shocks");
    
    id = MK_WILL_O_THE_WISP;
    catalog[id] = (creatureType) {0, "will-o-the-wisp",'w',  &wispLightColor,10,     90,     100,    {5, 8, 2},      5,  100,    100,    DF_ASH_BLOOD,   WISP_LIGHT, 0,  0};
    catalog[id].flags = (MONST_IMMUNE_TO_FIRE | MONST_FLIES | MONST_FLITS | MONST_NEVER_SLEEPS | MONST_FIERY | MONST_DIES_IF_NEGATED);
    strcpy(catalog[id].flavorText, "An ethereal blue flame dances through the air, flickering and pulsing in time to an otherworldly rhythm.");
    strcpy(catalog[id].absorbing, "consuming");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "scorches");
    strcpy(catalog[id].attack[1], "burns");
    
    id = MK_WRAITH;
    catalog[id] = (creatureType) {0, "wraith",       'W',    &wraithColor,   50,     60,     120,    {6, 13, 2},     5,  50,     100,    DF_GREEN_BLOOD, 0,      0,      0};
    catalog[id].flags = (MONST_FLEES_NEAR_DEATH);
    strcpy(catalog[id].flavorText, "The wraith's hollow eye sockets stare hungrily at the world from $HISHER emaciated frame, and $HISHER long, bloodstained nails grope ceaselessly at the air for a fresh victim.");
    strcpy(catalog[id].absorbing, "devouring");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "clutches");
    strcpy(catalog[id].attack[1], "claws");
    strcpy(catalog[id].attack[2], "bites");
    
    id = MK_ZOMBIE;
    catalog[id] = (creatureType) {0, "zombie",       'Z',    &vomitColor,    80,     0,      120,    {7, 12, 1},     0,  100,    100,    DF_ROT_GAS_BLOOD,0,     100,    DF_ROT_GAS_PUFF};
    strcpy(catalog[id].flavorText, "The zombie is the accursed product of a long-forgotten ritual. Perpetually decaying flesh, hanging from $HISHER bones in shreds, releases a putrid and flammable stench that will induce violent nausea with one whiff.");
    strcpy(catalog[id].absorbing, "rending");
    strcpy(catalog[id].absorbStatus, "Eating");
    strcpy(catalog[id].attack[0], "hits");
    strcpy(catalog[id].attack[1], "bites");
    
    id = MK_TROLL;
    catalog[id] = (creatureType) {0, "troll",        'T',    &trollColor,    65,     70,     125,    {10, 15, 3},    1,  100,    100,    DF_RED_BLOOD,   0,      0,      0};
    catalog[id].flags = (MONST_MALE | MONST_FEMALE);
    strcpy(catalog[id].flavorText, "An enormous, disfigured creature covered in phlegm and warts, the troll regenerates very quickly and attacks with astonishing strength. Many adventures have ended at $HISHER misshapen hands.");
    strcpy(catalog[id].absorbing, "eating");
    strcpy(catalog[id].absorbStatus, "Eating");
    strcpy(catalog[id].attack[0], "cudgels");
    strcpy(catalog[id].attack[1], "clubs");
    strcpy(catalog[id].attack[2], "bludgeons");
    strcpy(catalog[id].attack[3], "pummels");
    strcpy(catalog[id].attack[4], "batters");
    
    id = MK_OGRE_SHAMAN;
    catalog[id] = (creatureType) {0, "ogre shaman",  'O',    &green,         45,     40,     100,    {5, 9, 1},      20, 100,    200,    DF_RED_BLOOD,   0,      0,      0,     {BOLT_HASTE, BOLT_SPARK}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CAST_SPELLS_SLOWLY | MONST_MALE | MONST_FEMALE);
    catalog[id].abilityFlags = (MA_CAST_SUMMON | MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "This ogre is bent with age, but what $HESHE has lost in physical strength, $HESHE has more than gained in occult power.");
    strcpy(catalog[id].absorbing, "performing a ritual on");
    strcpy(catalog[id].absorbStatus, "Performing ritual");
    strcpy(catalog[id].attack[0], "cudgels");
    strcpy(catalog[id].attack[1], "clubs");
    strcpy(catalog[id].summonMessage, "chants in a harsh, guttural tongue!");
    
    id = MK_NAGA;
    catalog[id] = (creatureType) {0, "naga",         'N',    &trollColor,    75,     70,     150,    {7, 11, 4},     10, 100,    100,    DF_GREEN_BLOOD, 0,      100,    DF_PUDDLE};
    catalog[id].flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_NEVER_SLEEPS | MONST_FEMALE);
    catalog[id].abilityFlags = (MA_ATTACKS_ALL_ADJACENT);
    strcpy(catalog[id].flavorText, "The serpentine naga live beneath the subterranean waters and emerge to attack unsuspecting adventurers.");
    strcpy(catalog[id].absorbing, "studying");
    strcpy(catalog[id].absorbStatus, "Studying");
    strcpy(catalog[id].attack[0], "claws");
    strcpy(catalog[id].attack[1], "bites");
    strcpy(catalog[id].attack[1], "tail-whips");
    
    id = MK_SALAMANDER;
    catalog[id] = (creatureType) {0, "salamander",   'S',    &salamanderColor,60,    70,     150,    {5, 11, 3},     10, 100,    100,    DF_ASH_BLOOD,   SALAMANDER_LIGHT, 100, DF_SALAMANDER_FLAME};
    catalog[id].flags = (MONST_IMMUNE_TO_FIRE | MONST_SUBMERGES | MONST_NEVER_SLEEPS | MONST_FIERY | MONST_MALE);
    catalog[id].abilityFlags = (MA_ATTACKS_EXTEND);
    strcpy(catalog[id].flavorText, "A serpent wreathed in flames and carrying a burning lash, salamanders dwell in lakes of fire and emerge when they sense a nearby victim, leaving behind a trail of glowing embers.");
    strcpy(catalog[id].absorbing, "studying");
    strcpy(catalog[id].absorbStatus, "Studying");
    strcpy(catalog[id].attack[0], "whips");
    strcpy(catalog[id].attack[1], "lashes");
    
    id = MK_EXPLOSIVE_BLOAT;
    catalog[id] = (creatureType) {0, "explosive bloat",'b',  &orange,        10,     0,      100,    {0, 0, 0},      5,  100,    100,    DF_RED_BLOOD,   EXPLOSIVE_BLOAT_LIGHT,0, DF_BLOAT_EXPLOSION};
    catalog[id].flags = (MONST_FLIES | MONST_FLITS);
    catalog[id].abilityFlags = (MA_KAMIKAZE | MA_DF_ON_DEATH);
    strcpy(catalog[id].flavorText, "This rare subspecies of bloat is little more than a thin membrane surrounding a bladder of highly explosive gases. The slightest stress will cause $HIMHER to rupture in spectacular and deadly fashion.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "bumps");
    strcpy(catalog[id].DFMessage, "detonates with terrifying force!");
    
    id = MK_DAR_BLADEMASTER;
    catalog[id] = (creatureType) {0, "dar blademaster",'d',  &purple,        35,     70,     160,    {5, 9, 2},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,     {BOLT_BLINKING}};
    catalog[id].flags = (MONST_CARRY_ITEM_25 | MONST_MALE | MONST_FEMALE);
    catalog[id].abilityFlags = (MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "An elf of the deep, the dar blademaster leaps toward $HISHER enemies with frightening speed to engage in deadly swordplay.");
    strcpy(catalog[id].absorbing, "studying");
    strcpy(catalog[id].absorbStatus, "Studying");
    strcpy(catalog[id].attack[0], "grazes");
    strcpy(catalog[id].attack[1], "cuts");
    strcpy(catalog[id].attack[2], "slices");
    strcpy(catalog[id].attack[3], "slashes");
    strcpy(catalog[id].attack[4], "stabs");
    
    id = MK_DAR_PRIESTESS;
    catalog[id] = (creatureType) {0, "dar priestess", 'd',   &darPriestessColor,20,  60,     100,    {2, 5, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,     {BOLT_NEGATION, BOLT_HEALING, BOLT_HASTE, BOLT_SPARK}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_FEMALE);
    catalog[id].abilityFlags = (MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "The dar priestess carries a host of religious relics that jangle as $HESHE walks.");
    strcpy(catalog[id].absorbing, "praying over");
    strcpy(catalog[id].absorbStatus, "Praying");
    strcpy(catalog[id].attack[0], "cuts");
    strcpy(catalog[id].attack[1], "slices");
    
    id = MK_DAR_BATTLEMAGE;
    catalog[id] = (creatureType) {0, "dar battlemage",'d',   &darMageColor,  20,     60,     100,    {1, 3, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,     {BOLT_FIRE, BOLT_SLOW_2, BOLT_DISCORD}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_MALE | MONST_FEMALE);
    catalog[id].abilityFlags = (MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "The dar battlemage's eyes glow like embers and $HISHER hands radiate an occult heat.");
    strcpy(catalog[id].absorbing, "transmuting");
    strcpy(catalog[id].absorbStatus, "Transmuting");
    strcpy(catalog[id].attack[0], "cuts");
    
    id = MK_ACID_JELLY;
    catalog[id] = (creatureType) {0, "acidic jelly", 'J',    &acidBackColor, 60,     0,      115,    {2, 6, 1},      0,  100,    100,    DF_ACID_BLOOD,  0,      0,      0};
    catalog[id].flags = (MONST_DEFEND_DEGRADE_WEAPON);
    catalog[id].abilityFlags = (MA_HIT_DEGRADE_ARMOR | MA_CLONE_SELF_ON_DEFEND);
    strcpy(catalog[id].flavorText, "A jelly subsisting on a diet of acid mounds will eventually express the characteristics of $HISHER prey, corroding any weapons or armor that come in contact with $HIMHER.");
    strcpy(catalog[id].absorbing, "transmuting");
    strcpy(catalog[id].absorbStatus, "Transmuting");
    strcpy(catalog[id].attack[0], "burns");
    
    id = MK_CENTAUR;
    catalog[id] = (creatureType) {0, "centaur",      'C',    &tanColor,      35,     50,     175,    {4, 8, 2},      20, 50,     100,    DF_RED_BLOOD,   0,      0,      0,     {BOLT_DISTANCE_ATTACK}};
    catalog[id].abilityFlags = (MONST_MAINTAINS_DISTANCE | MONST_MALE);
    strcpy(catalog[id].flavorText, "Half man and half horse, the centaur is an expert with the bow and arrow -- hunter and steed fused into a single creature.");
    strcpy(catalog[id].absorbing, "studying");
    strcpy(catalog[id].absorbStatus, "Studying");
    strcpy(catalog[id].attack[0], "shoots");
    
    id = MK_UNDERWORM;
    catalog[id] = (creatureType) {0, "underworm",    'U',    &wormColor,     80,     40,     160,    {18, 22, 2},    3,  150,    200,    DF_WORM_BLOOD,  0,      0,      0};
    catalog[id].flags = (MONST_NEVER_SLEEPS);
    strcpy(catalog[id].flavorText, "A strange and horrifying creature of the earth's deepest places, larger than an ogre but capable of squeezing through tiny openings. When hungry, the underworm will burrow behind the walls of a cavern and lurk dormant and motionless -- often for months -- until $HESHE can feel the telltale vibrations of nearby prey.");
    strcpy(catalog[id].absorbing, "consuming");
    strcpy(catalog[id].absorbStatus, "Consuming");
    strcpy(catalog[id].attack[0], "slams");
    strcpy(catalog[id].attack[1], "bites");
    strcpy(catalog[id].attack[2], "tail-whips");
    
    id = MK_SENTINEL;
    catalog[id] = (creatureType) {0, "sentinel",     STATUE_CHAR, &sentinelColor, 50,0,      0,      {0, 0, 0},      0,  100,    175,    DF_RUBBLE_BLOOD,SENTINEL_LIGHT,0,0,    {BOLT_HEALING, BOLT_SPARK}};
    catalog[id].flags = (MONST_TURRET | MONST_CAST_SPELLS_SLOWLY | MONST_DIES_IF_NEGATED);
    strcpy(catalog[id].flavorText, "An ancient statue of an unrecognizable humanoid figure, the sentinel holds aloft a crystal that gleams with ancient warding magic. Sentinels are always found in groups, and each will attempt to repair any damage done to the others.");
    strcpy(catalog[id].absorbing, "focusing on");
    strcpy(catalog[id].absorbStatus, "Focusing");
    strcpy(catalog[id].attack[0], "hits");
    
    id = MK_ACID_TURRET;
    catalog[id] = (creatureType) {0, "acid turret", TURRET_CHAR, &acidBackColor,35,  0,      250,    {1, 2, 1},      0,  100,    250,    0,              0,      0,      0,     {BOLT_ACID_TURRET_ATTACK}};
    catalog[id].flags = (MONST_TURRET);
    catalog[id].abilityFlags = (MA_HIT_DEGRADE_ARMOR);
    strcpy(catalog[id].flavorText, "A green-flecked nozzle is embedded in the wall, ready to spew a stream of corrosive acid at intruders.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "douses");
    strcpy(catalog[id].attack[1], "drenches");
    
    id = MK_DART_TURRET;
    catalog[id] = (creatureType) {0, "dart turret", TURRET_CHAR, &centipedeColor,20, 0,      140,    {1, 2, 1},      0,  100,    250,    0,              0,      0,      0,     {BOLT_POISON_DART}};
    catalog[id].flags = (MONST_TURRET);
    catalog[id].abilityFlags = (MA_CAUSES_WEAKNESS);
    strcpy(catalog[id].flavorText, "This spring-loaded contraption fires darts that are imbued with a strength-sapping poison.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "pricks");
    
    id = MK_KRAKEN;
    catalog[id] = (creatureType) {0, "kraken",       'K',    &krakenColor,   120,    0,      150,    {15, 20, 3},    1,  50,     100,    0,              0,      0,      0};
    catalog[id].flags = (MONST_RESTRICTED_TO_LIQUID | MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS | MONST_NEVER_SLEEPS | MONST_FLEES_NEAR_DEATH);
    catalog[id].abilityFlags = (MA_SEIZES);
    strcpy(catalog[id].flavorText, "This tentacled nightmare will emerge from the subterranean waters to ensnare and devour any creature foolish enough to set foot into $HISHER lake.");
    strcpy(catalog[id].absorbing, "devouring");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "slaps");
    strcpy(catalog[id].attack[1], "smites");
    strcpy(catalog[id].attack[2], "batters");
    
    id = MK_LICH;
    catalog[id] = (creatureType) {0, "lich",         'L',    &white,         35,     80,     175,    {2, 6, 1},      0,  100,    100,    DF_ASH_BLOOD,   LICH_LIGHT, 0,  0,     {BOLT_FIRE}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_NO_POLYMORPH);
    catalog[id].abilityFlags = (MA_CAST_SUMMON);
    strcpy(catalog[id].flavorText, "The desiccated form of an ancient sorcerer, animated by dark arts and lust for power, commands the obedience of the infernal planes. $HISHER essence is anchored to reality by a phylactery that is always in $HISHER possession, and the lich cannot die unless $HISHER phylactery is destroyed.");
    strcpy(catalog[id].absorbing, "enchanting");
    strcpy(catalog[id].absorbStatus, "Enchanting");
    strcpy(catalog[id].attack[0], "touches");
    strcpy(catalog[id].summonMessage, "rasps a terrifying incantation!");
    
    id = MK_PHYLACTERY;
    catalog[id] = (creatureType) {0, "phylactery",   GEM_CHAR,&lichLightColor,30,    0,      0,      {0, 0, 0},      0,  100,    150,    DF_RUBBLE_BLOOD,LICH_LIGHT, 0,  0};
    catalog[id].flags = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED);
    catalog[id].abilityFlags = (MA_CAST_SUMMON | MA_ENTER_SUMMONS);
    strcpy(catalog[id].flavorText, "This gem was the fulcrum of a dark rite, performed centuries ago, that bound the soul of an ancient and terrible sorcerer. Hurry and destroy the gem, before the lich can gather its power and regenerate its corporeal form!");
    strcpy(catalog[id].absorbing, "enchanting");
    strcpy(catalog[id].absorbStatus, "Enchanting");
    strcpy(catalog[id].attack[0], "touches");
    strcpy(catalog[id].summonMessage, "swirls with dark sorcery as the lich regenerates its form!");
    
    id = MK_PIXIE;
    catalog[id] = (creatureType) {0, "pixie",        'p',    &pixieColor,    10,     90,     100,    {1, 3, 1},      20, 50,     100,    DF_GREEN_BLOOD, PIXIE_LIGHT, 0, 0,     {BOLT_NEGATION, BOLT_SLOW_2, BOLT_DISCORD, BOLT_SPARK}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_FLIES | MONST_FLITS | MONST_MALE | MONST_FEMALE);
    strcpy(catalog[id].flavorText, "A tiny humanoid sparkles in the gloom, the hum of $HISHER beating wings punctuated by intermittent peals of high-pitched laughter. What $HESHE lacks in physical endurance, $HESHE makes up for with $HISHER wealth of mischievous magical abilities.");
    strcpy(catalog[id].absorbing, "sprinkling dust on");
    strcpy(catalog[id].absorbStatus, "Dusting");
    strcpy(catalog[id].attack[0], "pokes");
    
    id = MK_PHANTOM;
    catalog[id] = (creatureType) {0, "phantom",      'P',    &ectoplasmColor,35,     70,     160,    {12, 18, 4},    0,  50,     200,    DF_ECTOPLASM_BLOOD, 0,  2,      DF_ECTOPLASM_DROPLET};
    catalog[id].flags = (MONST_INVISIBLE | MONST_FLITS | MONST_FLIES | MONST_IMMUNE_TO_WEBS);
    strcpy(catalog[id].flavorText, "A silhouette of mournful rage against an empty backdrop, the phantom slips through the dungeon invisibly in clear air, leaving behind glowing droplets of ectoplasm and the cries of $HISHER unsuspecting victims.");
    strcpy(catalog[id].absorbing, "permeating");
    strcpy(catalog[id].absorbStatus, "Permeating");
    strcpy(catalog[id].attack[0], "hits");
    
    id = MK_FLAME_TURRET;
    catalog[id] = (creatureType) {0, "flame turret", TURRET_CHAR, &lavaForeColor,40, 0,      150,    {1, 2, 1},      0,  100,    250,    0,              LAVA_LIGHT, 0,  0,     {BOLT_FIRE}};
    catalog[id].flags = (MONST_TURRET);
    strcpy(catalog[id].flavorText, "This infernal contraption spits blasts of flame at intruders.");
    strcpy(catalog[id].absorbing, "incinerating");
    strcpy(catalog[id].absorbStatus, "Incinerating");
    strcpy(catalog[id].attack[0], "pricks");
    
    id = MK_IMP;
    catalog[id] = (creatureType) {0, "imp",          'i',    &pink,          35,     90,     225,    {4, 9, 2},      10, 100,    100,    DF_GREEN_BLOOD, IMP_LIGHT,  0,  0,     {BOLT_BLINKING}};
    catalog[id].abilityFlags = (MA_HIT_STEAL_FLEE);
    strcpy(catalog[id].flavorText, "This trickster demon moves with astonishing speed and delights in stealing from $HISHER enemies and blinking away.");
    strcpy(catalog[id].absorbing, "dissecting");
    strcpy(catalog[id].absorbStatus, "Dissecting");
    strcpy(catalog[id].attack[0], "slices");
    strcpy(catalog[id].attack[1], "cuts");
    
    id = MK_FURY;
    catalog[id] = (creatureType) {0, "fury",         'f',    &darkRed,       19,     90,     200,    {6, 11, 4},     20, 50,     100,    DF_RED_BLOOD,   0,      0,      0};
    catalog[id].flags = (MONST_NEVER_SLEEPS | MONST_FLIES);
    strcpy(catalog[id].flavorText, "A creature of inchoate rage made flesh, the fury's moist wings beat loudly in the darkness.");
    strcpy(catalog[id].absorbing, "flagellating");
    strcpy(catalog[id].absorbStatus, "Flagellating");
    strcpy(catalog[id].attack[0], "drubs");
    strcpy(catalog[id].attack[1], "fustigates");
    strcpy(catalog[id].attack[2], "castigates");
    
    id = MK_REVENANT;
    catalog[id] = (creatureType) {0, "revenant",     'R',    &ectoplasmColor,30,     0,      200,    {15, 20, 5},    0,  100,    100,    DF_ECTOPLASM_BLOOD, 0,  0,      0};
    catalog[id].flags = (MONST_IMMUNE_TO_WEAPONS);
    strcpy(catalog[id].flavorText, "This unholy specter stalks the deep places of the earth without fear, impervious to conventional attacks.");
    strcpy(catalog[id].absorbing, "desecrating");
    strcpy(catalog[id].absorbStatus, "Desecrating");
    strcpy(catalog[id].attack[0], "hits");
    
    id = MK_TENTACLE_HORROR;
    catalog[id] = (creatureType) {0, "tentacle horror",'H',  &centipedeColor,120,    95,     225,    {25, 35, 3},    1,  100,    100,    DF_PURPLE_BLOOD,0,      0,      0};
    strcpy(catalog[id].flavorText, "This seething, towering nightmare of fleshy tentacles slinks through the bowels of the world. The tentacle horror's incredible strength and regeneration make $HIMHER one of the most fearsome creatures of the dungeon.");
    strcpy(catalog[id].absorbing, "sucking on");
    strcpy(catalog[id].absorbStatus, "Consuming");
    strcpy(catalog[id].attack[0], "slaps");
    strcpy(catalog[id].attack[1], "batters");
    strcpy(catalog[id].attack[2], "crushes");
    
    id = MK_GOLEM;
    catalog[id] = (creatureType) {0, "golem",        'G',    &gray,          400,    70,     225,    {4, 8, 1},      0,  100,    100,    DF_RUBBLE_BLOOD,0,      0,      0};
    catalog[id].flags = (MONST_REFLECT_4 | MONST_DIES_IF_NEGATED);
    strcpy(catalog[id].flavorText, "A statue animated by an ancient and tireless magic, the golem does not regenerate and attacks with only moderate strength, but $HISHER stone form can withstand incredible damage before collapsing into rubble.");
    strcpy(catalog[id].absorbing, "cradling");
    strcpy(catalog[id].absorbStatus, "Cradling");
    strcpy(catalog[id].attack[0], "backhands");
    strcpy(catalog[id].attack[1], "punches");
    strcpy(catalog[id].attack[2], "kicks");
    
    id = MK_DRAGON;
    catalog[id] = (creatureType) {0, "dragon",       'D',    &dragonColor,   150,    90,     250,    {25, 50, 4},    20, 50,     200,    DF_GREEN_BLOOD, 0,      0,      0,     {BOLT_DRAGONFIRE}};
    catalog[id].flags = (MONST_IMMUNE_TO_FIRE | MONST_CARRY_ITEM_100);
    catalog[id].abilityFlags = (MA_ATTACKS_ALL_ADJACENT);
    strcpy(catalog[id].flavorText, "An ancient serpent of the world's deepest places, the dragon's immense form belies its lightning-quick speed and testifies to $HISHER breathtaking strength. An undying furnace of white-hot flames burns within $HISHER scaly hide, and few could withstand a single moment under $HISHER infernal lash.");
    strcpy(catalog[id].absorbing, "consuming");
    strcpy(catalog[id].absorbStatus, "Consuming");
    strcpy(catalog[id].attack[0], "claws");
    strcpy(catalog[id].attack[1], "tail-whips");
    strcpy(catalog[id].attack[2], "bites");
    
    // bosses
    id = MK_GOBLIN_CHIEFTAN;
    catalog[id] = (creatureType) {0, "goblin warlord",'g',   &blue,          30,     17,     100,    {3, 6, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    catalog[id].abilityFlags = (MA_CAST_SUMMON | MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS);
    strcpy(catalog[id].flavorText, "Taller, stronger and smarter than other goblins, the warlord commands the loyalty of $HISHER kind and can summon them into battle.");
    strcpy(catalog[id].absorbing, "chanting over");
    strcpy(catalog[id].absorbStatus, "Chanting");
    strcpy(catalog[id].attack[0], "slashes");
    strcpy(catalog[id].attack[1], "cuts");
    strcpy(catalog[id].attack[2], "stabs");
    strcpy(catalog[id].attack[3], "skewers");
    strcpy(catalog[id].summonMessage, "lets loose a deafening war cry!");
    
    id = MK_BLACK_JELLY;
    catalog[id] = (creatureType) {0, "black jelly",  'J',    &black,         120,    0,      130,    {3, 8, 1},      0,  100,    100,    DF_PURPLE_BLOOD,0,      0,      0};
    catalog[id].abilityFlags = (MA_CLONE_SELF_ON_DEFEND);
    id = MK_BLACK_JELLY;
    strcpy(catalog[id].flavorText, "This blob of jet-black goo is as rare as $HESHE is deadly. Few creatures of the dungeon can withstand $HISHER caustic assault. Beware.");
    strcpy(catalog[id].absorbing, "absorbing");
    strcpy(catalog[id].absorbStatus, "Feeding");
    strcpy(catalog[id].attack[0], "smears");
    strcpy(catalog[id].attack[1], "slimes");
    strcpy(catalog[id].attack[2], "drenches");
    strcpy(catalog[id].summonMessage, "lets loose a deafening war cry!");
    
    id = MK_VAMPIRE;
    catalog[id] = (creatureType) {0, "vampire",      'V',    &white,         75,     60,     120,    {4, 15, 2},     6,  50,     100,    DF_RED_BLOOD,   0,      0,      DF_BLOOD_EXPLOSION, {BOLT_BLINKING, BOLT_DISCORD}};
    catalog[id].flags = (MONST_FLEES_NEAR_DEATH | MONST_MALE);
    catalog[id].abilityFlags = (MA_TRANSFERENCE | MA_DF_ON_DEATH | MA_CAST_SUMMON | MA_ENTER_SUMMONS);
    strcpy(catalog[id].flavorText, "This vampire lives a solitary life deep underground, consuming any warm-blooded creature unfortunate to venture near $HISHER lair.");
    strcpy(catalog[id].absorbing, "draining");
    strcpy(catalog[id].absorbStatus, "Drinking");
    strcpy(catalog[id].attack[0], "grazes");
    strcpy(catalog[id].attack[1], "bites");
    strcpy(catalog[id].attack[2], "buries $HISHER fangs in");
    strcpy(catalog[id].summonMessage, "spreads his cloak and bursts into a cloud of bats!");
    
    id = MK_FLAMEDANCER;
    catalog[id] = (creatureType) {0, "flamedancer",  'F',    &white,         65,     80,     120,    {3, 8, 2},      0,  100,    100,    DF_EMBER_BLOOD, FLAMEDANCER_LIGHT,100,DF_FLAMEDANCER_CORONA, {BOLT_FIRE}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_IMMUNE_TO_FIRE | MONST_FIERY);
    strcpy(catalog[id].flavorText, "An elemental creature from another plane of existence, the infernal flamedancer burns with such intensity that $HESHE is painful to behold.");
    strcpy(catalog[id].absorbing, "immolating");
    strcpy(catalog[id].absorbStatus, "Consuming");
    strcpy(catalog[id].attack[0], "singes");
    strcpy(catalog[id].attack[1], "burns");
    strcpy(catalog[id].attack[2], "immolates");
    
    // special effect monsters
    
    id = MK_SPECTRAL_BLADE;
    catalog[id] = (creatureType) {0, "spectral blade",WEAPON_CHAR, &spectralBladeColor,1, 0, 150,    {1, 1, 1},      0,  50,     100,    0,              SPECTRAL_BLADE_LIGHT,0,0};
    catalog[id].flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS | MB_DOES_NOT_TRACK_LEADER | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS | MONST_NOT_LISTED_IN_SIDEBAR);
    strcpy(catalog[id].flavorText, "Eldritch forces have coalesced to form this flickering, ethereal weapon.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "nicks");
    
    id = MK_SPECTRAL_IMAGE;
    catalog[id] = (creatureType) {0, "spectral sword",WEAPON_CHAR, &spectralImageColor, 1,0, 150,    {1, 1, 1},      0,  50,     100,    0,              SPECTRAL_IMAGE_LIGHT,0,0};
    catalog[id].flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS | MB_DOES_NOT_TRACK_LEADER | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS);
    strcpy(catalog[id].flavorText, "Eldritch energies bound up in your armor have leapt forth to project this spectral image.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "hits");
    
    id = MK_GUARDIAN;
    catalog[id] = (creatureType) {0, "stone guardian",STATUE_CHAR, &white,   1000,   0,      200,    {12, 17, 2},    0,  100,    100,    DF_RUBBLE,      0,      100,      DF_GUARDIAN_STEP};
    catalog[id].flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY | MONST_GETS_TURN_ON_ACTIVATION);
    strcpy(catalog[id].flavorText, "Guarding the room is a weathered stone statue of a knight carrying a battleaxe, connected to the glowing glyphs on the floor by invisible strands of enchantment.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "strikes");
    
    id = MK_WINGED_GUARDIAN;
    catalog[id] = (creatureType) {0, "winged guardian",STATUE_CHAR, &blue,   1000,   0,      200,    {12, 17, 2},    0,  100,    100,    DF_RUBBLE,      0,      100,      DF_SILENT_GLYPH_GLOW, {BOLT_BLINKING}};
    catalog[id].flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY);
    strcpy(catalog[id].flavorText, "A statue of a sword-wielding angel surveys the room, connected to the glowing glyphs on the floor by invisible strands of enchantment.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "strikes");
    
    id = MK_CHARM_GUARDIAN;
    catalog[id] = (creatureType) {0, "guardian spirit",STATUE_CHAR, &spectralImageColor,1000,0,200,  {5, 12, 2},     0,  100,    100,    0,              SPECTRAL_IMAGE_LIGHT,100,0};
    catalog[id].flags = (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY);
    strcpy(catalog[id].flavorText, "A spectral outline of a knight carrying a battleaxe casts an ethereal light on its surroundings.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "strikes");
    
    id = MK_WARDEN_OF_YENDOR;
    catalog[id] = (creatureType) {0, "Warden of Yendor",'Y', &yendorLightColor,1000,   0,    300,    {12, 17, 2},    0,  200,    200,    DF_RUBBLE,      YENDOR_LIGHT, 100, 0};
    catalog[id].flags = (MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_INVULNERABLE | MONST_NO_POLYMORPH);
    strcpy(catalog[id].flavorText, "An immortal presence stalks through the dungeon, implacably hunting that which was taken and the one who took it.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "strikes");
    
    id = MK_ELDRITCH_TOTEM;
    catalog[id] = (creatureType) {0, "eldritch totem",TOTEM_CHAR, &glyphColor,80,    0,      0,      {0, 0, 0},      0,  100,    100,    DF_RUBBLE_BLOOD,0,      0,      0};
    catalog[id].flags = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY);
    catalog[id].abilityFlags = (MA_CAST_SUMMON);
    strcpy(catalog[id].flavorText, "This totem sits at the center of a summoning circle that radiates a strange energy.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "strikes");
    strcpy(catalog[id].summonMessage, "crackles with energy as you touch the glyph!");
    
    id = MK_MIRRORED_TOTEM;
    catalog[id] = (creatureType) {0, "mirrored totem",TOTEM_CHAR, &beckonColor,80,   0,      0,      {0, 0, 0},      0,  100,    100,    DF_RUBBLE_BLOOD,0,      100,    DF_MIRROR_TOTEM_STEP, {BOLT_BECKONING}};
    catalog[id].flags = (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY | MONST_REFLECT_4 | MONST_IMMUNE_TO_WEAPONS | MONST_IMMUNE_TO_FIRE);
    strcpy(catalog[id].flavorText, "A prism of shoulder-high mirrored surfaces gleams in the darkness.");
    strcpy(catalog[id].absorbing, "gazing at");
    strcpy(catalog[id].absorbStatus, "Gazing");
    strcpy(catalog[id].attack[0], "strikes");
    
    // legendary allies
    
    id = MK_UNICORN;
    catalog[id] = (creatureType) {0, "unicorn",      UNICORN_CHAR, &white,   40,     60,     175,    {2, 10, 2},     20, 50,     100,    DF_RED_BLOOD,   UNICORN_LIGHT,1,DF_UNICORN_POOP, {BOLT_HEALING, BOLT_SHIELDING}};
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_MALE | MONST_FEMALE);
    strcpy(catalog[id].flavorText, "The unicorn's flowing mane and tail shine with rainbow light, $HISHER horn glows with healing and protective magic, and $HISHER eyes implore you to always chase your dreams. Unicorns are rumored to be attracted to virgins -- is there a hint of accusation in $HISHER gaze?");
    strcpy(catalog[id].absorbing, "consecrating");
    strcpy(catalog[id].absorbStatus, "Consecrating");
    strcpy(catalog[id].attack[0], "pokes");
    strcpy(catalog[id].attack[1], "stabs");
    strcpy(catalog[id].attack[2], "gores");
    
    id = MK_IFRIT;
    catalog[id] = (creatureType) {0, "ifrit",        'I',    &ifritColor,    40,     75,     175,    {5, 13, 2},     1,  50,     100,    DF_ASH_BLOOD,   IFRIT_LIGHT,0,  0,     {BOLT_DISCORD}};
    catalog[id].flags = (MONST_IMMUNE_TO_FIRE | MONST_FLIES | MONST_MALE);
    strcpy(catalog[id].flavorText, "A whirling desert storm given human shape, the ifrit's twin scimitars flicker in the darkness and $HISHER eyes burn with otherworldly zeal.");
    strcpy(catalog[id].absorbing, "absorbing");
    strcpy(catalog[id].absorbStatus, "Absorbing");
    strcpy(catalog[id].attack[0], "cuts");
    strcpy(catalog[id].attack[1], "slashes");
    strcpy(catalog[id].attack[2], "lacerates");
    
    id = MK_PHOENIX;
    catalog[id] = (creatureType) {0, "phoenix",      'P',    &phoenixColor,  30,     70,     175,    {4, 10, 2},     0,  50,     100,    DF_ASH_BLOOD,   PHOENIX_LIGHT,0,0};
    catalog[id].flags = (MONST_IMMUNE_TO_FIRE| MONST_FLIES);
    strcpy(catalog[id].flavorText, "This legendary bird shines with a brilliant light, and $HISHER wings crackle and pop like embers as they beat the air. When $HESHE dies, legend has it that an egg will form and a newborn phoenix will rise from its ashes.");
    strcpy(catalog[id].absorbing, "cremating");
    strcpy(catalog[id].absorbStatus, "Cremating");
    strcpy(catalog[id].attack[0], "pecks");
    strcpy(catalog[id].attack[1], "scratches");
    strcpy(catalog[id].attack[2], "claws");
    
    id = MK_PHOENIX_EGG;
    catalog[id] = (creatureType) {0, "phoenix egg",  GEM_CHAR,&phoenixColor, 150,    0,      0,      {0, 0, 0},      0,  100,    150,    DF_ASH_BLOOD,   PHOENIX_EGG_LIGHT,  0,  0};
    catalog[id].flags = (MONST_IMMUNE_TO_FIRE| MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS | MONST_NO_POLYMORPH | MONST_ALWAYS_HUNTING);
    catalog[id].abilityFlags = (MA_CAST_SUMMON | MA_ENTER_SUMMONS);
    strcpy(catalog[id].flavorText, "Cradled in a nest of cooling ashes, the translucent membrane of the phoenix egg reveals a yolk that glows ever brighter by the second.");
    strcpy(catalog[id].absorbing, "cremating");
    strcpy(catalog[id].absorbStatus, "Cremating");
    strcpy(catalog[id].attack[0], "touches");
    strcpy(catalog[id].summonMessage, "bursts as a newborn phoenix rises from the ashes!");
    
    id = MK_ANCIENT_SPIRIT;
    catalog[id] = (creatureType) {0, "mangrove dryad",'M',   &tanColor,      70,     60,     175,    {2, 8, 2},      6,  100,    100,    DF_ASH_BLOOD,   0,      0,      0,     {BOLT_ANCIENT_SPIRIT_VINES}};
    catalog[id].flags = (MONST_IMMUNE_TO_WEBS | MONST_ALWAYS_USE_ABILITY | MONST_MAINTAINS_DISTANCE | MONST_MALE | MONST_FEMALE);
    strcpy(catalog[id].flavorText, "This mangrove dryad is as old as the earth, and $HISHER gnarled figure houses an ancient power. When angered, $HESHE can call upon the forces of nature to bind $HISHER foes and tear them to shreds.");
    strcpy(catalog[id].absorbing, "absorbing");
    strcpy(catalog[id].absorbStatus, "Absorbing");
    strcpy(catalog[id].attack[0], "whips");
    strcpy(catalog[id].attack[1], "lashes");
    strcpy(catalog[id].attack[2], "thrashes");
    strcpy(catalog[id].attack[3], "lacerates");
}

void initHordeCatalog() {
    hordeCatalog = (hordeType *)malloc(sizeof(hordeType) * NUMBER_HORDES);
    memset(hordeCatalog, 0, sizeof(hordeType) * NUMBER_HORDES);
    
    unsigned int id = 0;
    
    //                                 leader       #members    member list                             member numbers                  minL    maxL    freq    spawnsIn        machine         flags
    hordeCatalog[id++] = (hordeType) {MK_RAT,            0,      {0},                                    {{0}},                          1,      5,      15};
    hordeCatalog[id++] = (hordeType) {MK_KOBOLD,         0,      {0},                                    {{0}},                          1,      6,      15};
    hordeCatalog[id++] = (hordeType) {MK_JACKAL,         0,      {0},                                    {{0}},                          1,      3,      10};
    hordeCatalog[id++] = (hordeType) {MK_JACKAL,         1,      {MK_JACKAL},                            {{1, 3, 1}},                    3,      7,      5};
    hordeCatalog[id++] = (hordeType) {MK_EEL,            0,      {0},                                    {{0}},                          2,      17,     10,     DEEP_WATER};
    hordeCatalog[id++] = (hordeType) {MK_MONKEY,         0,      {0},                                    {{0}},                          2,      9,      5};
    hordeCatalog[id++] = (hordeType) {MK_BLOAT,          0,      {0},                                    {{0}},                          2,      13,     3};
    hordeCatalog[id++] = (hordeType) {MK_PIT_BLOAT,      0,      {0},                                    {{0}},                          2,      13,     1};
    hordeCatalog[id++] = (hordeType) {MK_BLOAT,          1,      {MK_BLOAT},                             {{0, 2, 1}},                    14,     26,     3};
    hordeCatalog[id++] = (hordeType) {MK_PIT_BLOAT,      1,      {MK_PIT_BLOAT},                         {{0, 2, 1}},                    14,     26,     1};
    hordeCatalog[id++] = (hordeType) {MK_EXPLOSIVE_BLOAT,0,      {0},                                    {{0}},                          10,     26,     1};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         0,      {0},                                    {{0}},                          3,      10,     10};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CONJURER,0,      {0},                                    {{0}},                          3,      10,     6};
    hordeCatalog[id++] = (hordeType) {MK_TOAD,           0,      {0},                                    {{0}},                          4,      11,     10};
    hordeCatalog[id++] = (hordeType) {MK_PINK_JELLY,     0,      {0},                                    {{0}},                          4,      13,     10};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_TOTEM,   1,      {MK_GOBLIN},                            {{2,4,1}},                      5,      13,     10,     0,              MT_CAMP_AREA,   HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_ARROW_TURRET,   0,      {0},                                    {{0}},                          5,      13,     10,     WALL,   0,                      HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_MONKEY,         1,      {MK_MONKEY},                            {{2,4,1}},                      5,      13,     2};
    hordeCatalog[id++] = (hordeType) {MK_VAMPIRE_BAT,    0,      {0},                                    {{0}},                          6,      13,     3};
    hordeCatalog[id++] = (hordeType) {MK_VAMPIRE_BAT,    1,      {MK_VAMPIRE_BAT},                       {{1,2,1}},                      6,      13,     7,      0,              0,              HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_ACID_MOUND,     0,      {0},                                    {{0}},                          6,      13,     10};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         3,      {MK_GOBLIN, MK_GOBLIN_MYSTIC, MK_JACKAL},{{2, 3, 1}, {1,2,1}, {1,2,1}}, 6,      12,     4};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CONJURER,2,      {MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC}, {{0,1,1}, {1,1,1}},             7,      15,     4};
    hordeCatalog[id++] = (hordeType) {MK_CENTIPEDE,      0,      {0},                                    {{0}},                          7,      14,     10};
    hordeCatalog[id++] = (hordeType) {MK_BOG_MONSTER,    0,      {0},                                    {{0}},                          7,      14,     8,      MUD,            0,              HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_OGRE,           0,      {0},                                    {{0}},                          7,      13,     10};
    hordeCatalog[id++] = (hordeType) {MK_EEL,            1,      {MK_EEL},                               {{2, 4, 1}},                    8,      22,     7,      DEEP_WATER};
    hordeCatalog[id++] = (hordeType) {MK_ACID_MOUND,     1,      {MK_ACID_MOUND},                        {{2, 4, 1}},                    9,      13,     3};
    hordeCatalog[id++] = (hordeType) {MK_SPIDER,         0,      {0},                                    {{0}},                          9,      16,     10};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,1,      {MK_DAR_BLADEMASTER},                   {{0, 1, 1}},                    10,     14,     10};
    hordeCatalog[id++] = (hordeType) {MK_WILL_O_THE_WISP,0,      {0},                                    {{0}},                          10,     17,     10};
    hordeCatalog[id++] = (hordeType) {MK_WRAITH,         0,      {0},                                    {{0}},                          10,     17,     10};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_TOTEM,   4,      {MK_GOBLIN_TOTEM, MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC, MK_GOBLIN}, {{1,2,1},{1,2,1},{1,2,1},{3,5,1}},10,17,8,0,MT_CAMP_AREA,   HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_SPARK_TURRET,   0,      {0},                                    {{0}},                          11,     18,     10,     WALL,   0,                      HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_ZOMBIE,         0,      {0},                                    {{0}},                          11,     18,     10};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          0,      {0},                                    {{0}},                          12,     19,     10};
    hordeCatalog[id++] = (hordeType) {MK_OGRE_TOTEM,     1,      {MK_OGRE},                              {{2,4,1}},                      12,     19,     6,      0,          0,                  HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_BOG_MONSTER,    1,      {MK_BOG_MONSTER},                       {{2,4,1}},                      12,     26,     10,     MUD};
    hordeCatalog[id++] = (hordeType) {MK_NAGA,           0,      {0},                                    {{0}},                          13,     20,     10,     DEEP_WATER};
    hordeCatalog[id++] = (hordeType) {MK_SALAMANDER,     0,      {0},                                    {{0}},                          13,     20,     10,     LAVA};
    hordeCatalog[id++] = (hordeType) {MK_OGRE_SHAMAN,    1,      {MK_OGRE},                              {{1, 3, 1}},                    14,     20,     10};
    hordeCatalog[id++] = (hordeType) {MK_CENTAUR,        1,      {MK_CENTAUR},                           {{1, 1, 1}},                    14,     21,     10};
    hordeCatalog[id++] = (hordeType) {MK_ACID_JELLY,     0,      {0},                                    {{0}},                          14,     21,     10};
    hordeCatalog[id++] = (hordeType) {MK_ACID_TURRET,    0,      {0},                                    {{0}},                          15,     22,     10,     WALL,   0,                      HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_DART_TURRET,    0,      {0},                                    {{0}},                          15,     22,     10,     WALL,   0,                      HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_PIXIE,          0,      {0},                                    {{0}},                          14,     21,     8};
    hordeCatalog[id++] = (hordeType) {MK_FLAME_TURRET,   0,      {0},                                    {{0}},                          14,     24,     10,     WALL,   0,                      HORDE_NO_PERIODIC_SPAWN};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,2,      {MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS}, {{0, 1, 1}, {0, 1, 1}},         15,     17,     10};
    hordeCatalog[id++] = (hordeType) {MK_PINK_JELLY,     2,      {MK_PINK_JELLY, MK_DAR_PRIESTESS},      {{0, 1, 1}, {1, 2, 1}},         17,     23,     7};
    hordeCatalog[id++] = (hordeType) {MK_KRAKEN,         0,      {0},                                    {{0}},                          15,     30,     10,     DEEP_WATER};
    hordeCatalog[id++] = (hordeType) {MK_PHANTOM,        0,      {0},                                    {{0}},                          16,     23,     10};
    hordeCatalog[id++] = (hordeType) {MK_WRAITH,         1,      {MK_WRAITH},                            {{1, 4, 1}},                    16,     23,     8};
    hordeCatalog[id++] = (hordeType) {MK_IMP,            0,      {0},                                    {{0}},                          17,     24,     10};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,3,      {MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE},{{1,2,1},{1,1,1},{1,1,1}},18,25,10};
    hordeCatalog[id++] = (hordeType) {MK_FURY,           1,      {MK_FURY},                              {{2, 4, 1}},                    18,     26,     8};
    hordeCatalog[id++] = (hordeType) {MK_REVENANT,       0,      {0},                                    {{0}},                          19,     27,     10};
    hordeCatalog[id++] = (hordeType) {MK_GOLEM,          0,      {0},                                    {{0}},                          21,     30,     10};
    hordeCatalog[id++] = (hordeType) {MK_TENTACLE_HORROR,0,      {0},                                    {{0}},                          22,     DEEPEST_LEVEL-1,        10};
    hordeCatalog[id++] = (hordeType) {MK_PHYLACTERY,     0,      {0},                                    {{0}},                          22,     DEEPEST_LEVEL-1,        10};
    hordeCatalog[id++] = (hordeType) {MK_DRAGON,         0,      {0},                                    {{0}},                          24,     DEEPEST_LEVEL-1,        7};
    hordeCatalog[id++] = (hordeType) {MK_DRAGON,         1,      {MK_DRAGON},                            {{1,1,1}},                      27,     DEEPEST_LEVEL-1,        3};
    hordeCatalog[id++] = (hordeType) {MK_GOLEM,          3,      {MK_GOLEM, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE}, {{1, 2, 1}, {0,1,1},{0,1,1}},27,DEEPEST_LEVEL-1,   8};
    hordeCatalog[id++] = (hordeType) {MK_GOLEM,          1,      {MK_GOLEM},                             {{5, 10, 2}},                   30,     DEEPEST_LEVEL-1,    2};
    hordeCatalog[id++] = (hordeType) {MK_KRAKEN,         1,      {MK_KRAKEN},                            {{5, 10, 2}},                   30,     DEEPEST_LEVEL-1,    10,     DEEP_WATER};
    hordeCatalog[id++] = (hordeType) {MK_TENTACLE_HORROR,2,      {MK_TENTACLE_HORROR, MK_REVENANT},      {{1, 3, 1}, {2, 4, 1}},         32,     DEEPEST_LEVEL-1,    2};
    hordeCatalog[id++] = (hordeType) {MK_DRAGON,         1,      {MK_DRAGON},                            {{3, 5, 1}},                    34,     DEEPEST_LEVEL-1,    2};
    
    // summons
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CONJURER,1,      {MK_SPECTRAL_BLADE},                    {{3, 5, 1}},                    0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED | HORDE_DIES_ON_LEADER_DEATH};
    hordeCatalog[id++] = (hordeType) {MK_OGRE_SHAMAN,    1,      {MK_OGRE},                              {{1, 1, 1}},                    0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED};
    hordeCatalog[id++] = (hordeType) {MK_VAMPIRE,        1,      {MK_VAMPIRE_BAT},                       {{3, 3, 1}},                    0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED};
    hordeCatalog[id++] = (hordeType) {MK_LICH,           1,      {MK_PHANTOM},                           {{2, 3, 1}},                    0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED};
    hordeCatalog[id++] = (hordeType) {MK_LICH,           1,      {MK_FURY},                              {{2, 3, 1}},                    0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED};
    hordeCatalog[id++] = (hordeType) {MK_PHYLACTERY,     1,      {MK_LICH},                              {{1,1,1}},                      0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CHIEFTAN,2,      {MK_GOBLIN_CONJURER, MK_GOBLIN},        {{1,1,1}, {3,4,1}},             0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED | HORDE_SUMMONED_AT_DISTANCE};
    hordeCatalog[id++] = (hordeType) {MK_PHOENIX_EGG,    1,      {MK_PHOENIX},                           {{1,1,1}},                      0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED};
    hordeCatalog[id++] = (hordeType) {MK_ELDRITCH_TOTEM, 1,      {MK_SPECTRAL_BLADE},                    {{4, 7, 1}},                    0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED | HORDE_DIES_ON_LEADER_DEATH};
    hordeCatalog[id++] = (hordeType) {MK_ELDRITCH_TOTEM, 1,      {MK_FURY},                              {{2, 3, 1}},                    0,      0,      10,     0,          0,                  HORDE_IS_SUMMONED | HORDE_DIES_ON_LEADER_DEATH};
    
    // captives
    hordeCatalog[id++] = (hordeType) {MK_MONKEY,         1,      {MK_KOBOLD},                            {{1, 2, 1}},                    1,      5,      1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         1,      {MK_GOBLIN},                            {{1, 2, 1}},                    3,      7,      1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_OGRE,           1,      {MK_GOBLIN},                            {{3, 5, 1}},                    4,      10,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_MYSTIC,  1,      {MK_KOBOLD},                            {{3, 7, 1}},                    5,      11,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_OGRE,           1,      {MK_OGRE},                              {{1, 2, 1}},                    8,      15,     2,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          1,      {MK_TROLL},                             {{1, 2, 1}},                    12,     19,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_CENTAUR,        1,      {MK_TROLL},                             {{1, 2, 1}},                    12,     19,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          2,      {MK_OGRE, MK_OGRE_SHAMAN},              {{2, 3, 1}, {0, 1, 1}},         14,     19,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,1,      {MK_TROLL},                             {{1, 2, 1}},                    12,     19,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_NAGA,           1,      {MK_SALAMANDER},                        {{1, 2, 1}},                    13,     20,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_SALAMANDER,     1,      {MK_NAGA},                              {{1, 2, 1}},                    13,     20,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          1,      {MK_SALAMANDER},                        {{1, 2, 1}},                    13,     19,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_IMP,            1,      {MK_FURY},                              {{2, 4, 1}},                    18,     26,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_PIXIE,          1,      {MK_IMP, MK_PHANTOM},                   {{1, 2, 1}, {1, 2, 1}},         14,     21,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,1,      {MK_FURY},                              {{2, 4, 1}},                    18,     26,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,1,      {MK_IMP},                               {{2, 3, 1}},                    18,     26,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_DAR_PRIESTESS,  1,      {MK_FURY},                              {{2, 4, 1}},                    18,     26,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BATTLEMAGE, 1,      {MK_IMP},                               {{2, 3, 1}},                    18,     26,     1,      0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_TENTACLE_HORROR,3,      {MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE},{{1,2,1},{1,1,1},{1,1,1}},20,26,1,    0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    hordeCatalog[id++] = (hordeType) {MK_GOLEM,          3,      {MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE},{{1,2,1},{1,1,1},{1,1,1}},18,25,1,    0,          0,                  HORDE_LEADER_CAPTIVE | HORDE_NEVER_OOD};
    
    // bosses
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CHIEFTAN,2,      {MK_GOBLIN_MYSTIC, MK_GOBLIN, MK_GOBLIN_TOTEM}, {{1,1,1}, {2,3,1}, {2,2,1}},2,  10,     5,      0,          0,                  HORDE_MACHINE_BOSS};
    hordeCatalog[id++] = (hordeType) {MK_BLACK_JELLY,    0,      {0},                                    {{0}},                          5,      15,     5,      0,          0,                  HORDE_MACHINE_BOSS};
    hordeCatalog[id++] = (hordeType) {MK_VAMPIRE,        0,      {0},                                    {{0}},                          10,     DEEPEST_LEVEL,  5,  0,      0,                  HORDE_MACHINE_BOSS};
    hordeCatalog[id++] = (hordeType) {MK_FLAMEDANCER,    0,      {0},                                    {{0}},                          10,     DEEPEST_LEVEL,  5,  0,      0,                  HORDE_MACHINE_BOSS};
    
    // machine water monsters
    hordeCatalog[id++] = (hordeType) {MK_EEL,            0,      {0},                                    {{0}},                          2,      7,      10,     DEEP_WATER, 0,                  HORDE_MACHINE_WATER_MONSTER};
    hordeCatalog[id++] = (hordeType) {MK_EEL,            1,      {MK_EEL},                               {{2, 4, 1}},                    5,      15,     10,     DEEP_WATER, 0,                  HORDE_MACHINE_WATER_MONSTER};
    hordeCatalog[id++] = (hordeType) {MK_KRAKEN,         0,      {0},                                    {{0}},                          12,     DEEPEST_LEVEL,  10, DEEP_WATER, 0,              HORDE_MACHINE_WATER_MONSTER};
    hordeCatalog[id++] = (hordeType) {MK_KRAKEN,         1,      {MK_EEL},                               {{1, 2, 1}},                    12,     DEEPEST_LEVEL,  8,  DEEP_WATER, 0,              HORDE_MACHINE_WATER_MONSTER};
    
    // dungeon captives -- no captors
    hordeCatalog[id++] = (hordeType) {MK_OGRE,           0,      {0},                                    {{0}},                          1,      5,      10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_NAGA,           0,      {0},                                    {{0}},                          2,      8,      10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_MYSTIC,  0,      {0},                                    {{0}},                          2,      8,      10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          0,      {0},                                    {{0}},                          10,     20,     10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,0,      {0},                                    {{0}},                          8,      14,     10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_PRIESTESS,  0,      {0},                                    {{0}},                          8,      14,     10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_WRAITH,         0,      {0},                                    {{0}},                          11,     17,     10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOLEM,          0,      {0},                                    {{0}},                          17,     23,     10,     0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_TENTACLE_HORROR,0,      {0},                                    {{0}},                          20,     AMULET_LEVEL,10,0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DRAGON,         0,      {0},                                    {{0}},                          23,     AMULET_LEVEL,10,0,          0,                  HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE};
    
    // machine statue monsters
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         0,      {0},                                    {{0}},                          1,      6,      10,     STATUE_DORMANT, 0,              HORDE_MACHINE_STATUE};
    hordeCatalog[id++] = (hordeType) {MK_OGRE,           0,      {0},                                    {{0}},                          6,      12,     10,     STATUE_DORMANT, 0,              HORDE_MACHINE_STATUE};
    hordeCatalog[id++] = (hordeType) {MK_WRAITH,         0,      {0},                                    {{0}},                          10,     17,     10,     STATUE_DORMANT, 0,              HORDE_MACHINE_STATUE};
    hordeCatalog[id++] = (hordeType) {MK_NAGA,           0,      {0},                                    {{0}},                          12,     19,     10,     STATUE_DORMANT, 0,              HORDE_MACHINE_STATUE};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          0,      {0},                                    {{0}},                          14,     21,     10,     STATUE_DORMANT, 0,              HORDE_MACHINE_STATUE};
    hordeCatalog[id++] = (hordeType) {MK_GOLEM,          0,      {0},                                    {{0}},                          21,     30,     10,     STATUE_DORMANT, 0,              HORDE_MACHINE_STATUE};
    hordeCatalog[id++] = (hordeType) {MK_DRAGON,         0,      {0},                                    {{0}},                          29,     DEEPEST_LEVEL,  10, STATUE_DORMANT, 0,          HORDE_MACHINE_STATUE};
    hordeCatalog[id++] = (hordeType) {MK_TENTACLE_HORROR,0,      {0},                                    {{0}},                          29,     DEEPEST_LEVEL,  10, STATUE_DORMANT, 0,          HORDE_MACHINE_STATUE};
    
    // machine turrets
    hordeCatalog[id++] = (hordeType) {MK_ARROW_TURRET,   0,      {0},                                    {{0}},                          5,      13,     10,     TURRET_DORMANT, 0,              HORDE_MACHINE_TURRET};
    hordeCatalog[id++] = (hordeType) {MK_SPARK_TURRET,   0,      {0},                                    {{0}},                          11,     18,     10,     TURRET_DORMANT, 0,              HORDE_MACHINE_TURRET};
    hordeCatalog[id++] = (hordeType) {MK_ACID_TURRET,    0,      {0},                                    {{0}},                          15,     22,     10,     TURRET_DORMANT, 0,              HORDE_MACHINE_TURRET};
    hordeCatalog[id++] = (hordeType) {MK_DART_TURRET,    0,      {0},                                    {{0}},                          15,     22,     10,     TURRET_DORMANT, 0,              HORDE_MACHINE_TURRET};
    hordeCatalog[id++] = (hordeType) {MK_FLAME_TURRET,   0,      {0},                                    {{0}},                          17,     24,     10,     TURRET_DORMANT, 0,              HORDE_MACHINE_TURRET};
    
    // machine mud monsters
    hordeCatalog[id++] = (hordeType) {MK_BOG_MONSTER,    0,      {0},                                    {{0}},                          12,     26,     10,     MACHINE_MUD_DORMANT, 0,         HORDE_MACHINE_MUD};
    hordeCatalog[id++] = (hordeType) {MK_KRAKEN,         0,      {0},                                    {{0}},                          17,     26,     3,      MACHINE_MUD_DORMANT, 0,         HORDE_MACHINE_MUD};
    
    // kennel monsters
    hordeCatalog[id++] = (hordeType) {MK_MONKEY,         0,      {0},                                    {{0}},                          1,      5,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         0,      {0},                                    {{0}},                          1,      8,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CONJURER,0,      {0},                                    {{0}},                          2,      9,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_MYSTIC,  0,      {0},                                    {{0}},                          2,      9,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_OGRE,           0,      {0},                                    {{0}},                          5,      15,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          0,      {0},                                    {{0}},                          10,     19,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_NAGA,           0,      {0},                                    {{0}},                          9,      20,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_SALAMANDER,     0,      {0},                                    {{0}},                          9,      20,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_IMP,            0,      {0},                                    {{0}},                          15,     26,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_PIXIE,          0,      {0},                                    {{0}},                          11,     21,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,0,      {0},                                    {{0}},                          9,      AMULET_LEVEL, 10, MONSTER_CAGE_CLOSED, 0,       HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_PRIESTESS,  0,      {0},                                    {{0}},                          12,     AMULET_LEVEL, 10, MONSTER_CAGE_CLOSED, 0,       HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BATTLEMAGE, 0,      {0},                                    {{0}},                          13,     AMULET_LEVEL, 10, MONSTER_CAGE_CLOSED, 0,       HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE};
    
    // vampire bloodbags
    hordeCatalog[id++] = (hordeType) {MK_MONKEY,         0,      {0},                                    {{0}},                          1,      5,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         0,      {0},                                    {{0}},                          1,      8,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CONJURER,0,      {0},                                    {{0}},                          2,      9,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_MYSTIC,  0,      {0},                                    {{0}},                          2,      9,      10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_OGRE,           0,      {0},                                    {{0}},                          5,      15,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_TROLL,          0,      {0},                                    {{0}},                          10,     19,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_NAGA,           0,      {0},                                    {{0}},                          9,      20,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_IMP,            0,      {0},                                    {{0}},                          15,     AMULET_LEVEL,10,MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_PIXIE,          0,      {0},                                    {{0}},                          11,     21,     10,     MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BLADEMASTER,0,      {0},                                    {{0}},                          9,      AMULET_LEVEL,10,MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_PRIESTESS,  0,      {0},                                    {{0}},                          12,     AMULET_LEVEL,10,MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    hordeCatalog[id++] = (hordeType) {MK_DAR_BATTLEMAGE, 0,      {0},                                    {{0}},                          13,     AMULET_LEVEL,10,MONSTER_CAGE_CLOSED, 0,         HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE};
    
    // key thieves
    hordeCatalog[id++] = (hordeType) {MK_MONKEY,         0,      {0},                                    {{0}},                          1,      14,     10,     0,          0,                  HORDE_MACHINE_THIEF};
    hordeCatalog[id++] = (hordeType) {MK_IMP,            0,      {0},                                    {{0}},                          15,     DEEPEST_LEVEL,  10, 0,      0,                  HORDE_MACHINE_THIEF};
    
    // legendary allies
    hordeCatalog[id++] = (hordeType) {MK_UNICORN,        0,      {0},                                    {{0}},                          1,      DEEPEST_LEVEL,  10, 0,      0,                  HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER};
    hordeCatalog[id++] = (hordeType) {MK_IFRIT,          0,      {0},                                    {{0}},                          1,      DEEPEST_LEVEL,  10, 0,      0,                  HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER};
    hordeCatalog[id++] = (hordeType) {MK_PHOENIX_EGG,    0,      {0},                                    {{0}},                          1,      DEEPEST_LEVEL,  10, 0,      0,                  HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER};
    hordeCatalog[id++] = (hordeType) {MK_ANCIENT_SPIRIT, 0,      {0},                                    {{0}},                          1,      DEEPEST_LEVEL,  10, 0,      0,                  HORDE_MACHINE_LEGENDARY_ALLY | HORDE_ALLIED_WITH_PLAYER};
    
    // goblin warren
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         0,      {0},                                    {{0}},                          1,      10,     10,     0,              0,              HORDE_MACHINE_GOBLIN_WARREN};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CONJURER,0,      {0},                                    {{0}},                          1,      10,     6,      0,              0,              HORDE_MACHINE_GOBLIN_WARREN};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_TOTEM,   1,      {MK_GOBLIN},                            {{2,4,1}},                      5,      13,     10,     0,              MT_CAMP_AREA,   HORDE_MACHINE_GOBLIN_WARREN};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         3,      {MK_GOBLIN, MK_GOBLIN_MYSTIC, MK_JACKAL},{{2, 3, 1}, {1,2,1}, {1,2,1}}, 6,      12,     4,      0,              0,              HORDE_MACHINE_GOBLIN_WARREN};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_CONJURER,2,      {MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC}, {{0,1,1}, {1,1,1}},             7,      15,     4,      0,              0,              HORDE_MACHINE_GOBLIN_WARREN};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN_TOTEM,   4,      {MK_GOBLIN_TOTEM, MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC, MK_GOBLIN}, {{1,2,1},{1,2,1},{1,2,1},{3,5,1}},10,17,8,0,MT_CAMP_AREA,   HORDE_MACHINE_GOBLIN_WARREN};
    hordeCatalog[id++] = (hordeType) {MK_GOBLIN,         1,      {MK_GOBLIN},                            {{1, 2, 1}},                    3,      7,      1,      0,              0,              HORDE_MACHINE_GOBLIN_WARREN | HORDE_LEADER_CAPTIVE};
}

void initMonsterClassCatalog() {
    monsterClassCatalog = (monsterClass *)malloc(sizeof(monsterClass) * MONSTER_CLASS_COUNT);
    memset(monsterClassCatalog, 0, sizeof(monsterClass) * MONSTER_CLASS_COUNT);
    
    unsigned int id = 0;
    
    //                                          name               frequency   maxDepth    memberList
    monsterClassCatalog[id++] = (monsterClass) {"abomination",     10,         -1,         {MK_BOG_MONSTER, MK_UNDERWORM, MK_KRAKEN, MK_TENTACLE_HORROR}};
    monsterClassCatalog[id++] = (monsterClass) {"dar",             10,         22,         {MK_DAR_BLADEMASTER, MK_DAR_PRIESTESS, MK_DAR_BATTLEMAGE}};
    monsterClassCatalog[id++] = (monsterClass) {"animal",          10,         10,         {MK_RAT, MK_MONKEY, MK_JACKAL, MK_EEL, MK_TOAD, MK_CENTIPEDE, MK_SPIDER}};
    monsterClassCatalog[id++] = (monsterClass) {"goblin",          10,         10,         {MK_GOBLIN, MK_GOBLIN_CONJURER, MK_GOBLIN_MYSTIC, MK_GOBLIN_TOTEM, MK_GOBLIN_CHIEFTAN}};
    monsterClassCatalog[id++] = (monsterClass) {"ogre",            10,         16,         {MK_OGRE, MK_OGRE_SHAMAN, MK_OGRE_TOTEM}};
    monsterClassCatalog[id++] = (monsterClass) {"dragon",          10,         -1,         {MK_DRAGON}};
    monsterClassCatalog[id++] = (monsterClass) {"undead",          10,         -1,         {MK_ZOMBIE, MK_WRAITH, MK_VAMPIRE, MK_PHANTOM, MK_LICH, MK_REVENANT}};
    monsterClassCatalog[id++] = (monsterClass) {"jelly",           10,         15,         {MK_PINK_JELLY, MK_BLACK_JELLY, MK_ACID_JELLY}};
    monsterClassCatalog[id++] = (monsterClass) {"turret",          5,          18,         {MK_ARROW_TURRET, MK_SPARK_TURRET, MK_DART_TURRET, MK_FLAME_TURRET}};
    monsterClassCatalog[id++] = (monsterClass) {"infernal",        10,         -1,         {MK_FLAMEDANCER, MK_IMP, MK_REVENANT, MK_FURY, MK_PHANTOM}};
    monsterClassCatalog[id++] = (monsterClass) {"waterborne",      10,         17,         {MK_EEL, MK_NAGA, MK_KRAKEN}};
    monsterClassCatalog[id++] = (monsterClass) {"fireborne",       10,         12,         {MK_WILL_O_THE_WISP, MK_SALAMANDER, MK_FLAMEDANCER}};
    monsterClassCatalog[id++] = (monsterClass) {"troll",           10,         15,         {MK_TROLL}};
}

void initBlueprintCatalog() {
    blueprintCatalog = (blueprint *)malloc(sizeof(blueprint) * NUMBER_BLUEPRINTS);
    memset(blueprintCatalog, 0, sizeof(blueprint) * NUMBER_BLUEPRINTS);
    
    blueprint *bc = blueprintCatalog;
    
    unsigned int id = 0;
    //BLUEPRINTS:
    //depths            roomSize    freq    featureCt   dungeonProfileType  flags   (features on subsequent lines)
    
    //FEATURES:
    //DF        terrain     layer       instanceCtRange minInsts    itemCat     itemKind    monsterKind     reqSpace        hordeFl     itemFlags   featureFlags
    
    // -- REWARD ROOMS --
    
    // Mixed item library -- can check one item out at a time
    bc[id++] =  (blueprint){{1, 12},           {30, 50},   30,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {1,1},      1,          WAND,       WAND_EMPOWERMENT, 0,        2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_PLAYER_AVOIDS), (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {3,3},      3,          (WEAPON|ARMOR|WAND),-1, 0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_PLAYER_AVOIDS), (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {2,3},      2,          (STAFF|RING|CHARM),-1,  0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {0,         STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Single item category library -- can check one item out at a time
    bc[id++] =  (blueprint){{1, 12},           {30, 50},   15,     5,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {3,4},      3,          (RING),     -1,         0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_ALTERNATIVE | MF_IMPREGNABLE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {4,5},      4,          (STAFF),    -1,         0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_ALTERNATIVE | MF_IMPREGNABLE)},
        {0,         STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Treasure room -- apothecary or archive (potions or scrolls)
    bc[id++] =  (blueprint){{8, AMULET_LEVEL}, {20, 40},   20,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         0,          0,              {5,7},      2,          (POTION),   -1,         0,              2,              0,          0,          (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {0,         0,          0,              {4,6},      2,          (SCROLL),   -1,         0,              2,              0,          0,          (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {0,         FUNGUS_FOREST,SURFACE,      {3,4},      0,          0,          -1,         0,              2,              0,          0,          0},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {0,         STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Guaranteed good permanent item on a glowing pedestal (runic weapon/armor or 2 staffs)
    bc[id++] =  (blueprint){{5, 16},           {10, 30},   30,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {0,         PEDESTAL,   DUNGEON,        {1,1},      1,          (WEAPON),   -1,         0,              2,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING)},
        {0,         PEDESTAL,   DUNGEON,        {1,1},      1,          (ARMOR),    -1,         0,              2,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_TREAT_AS_BLOCKING)},
        {0,         PEDESTAL,   DUNGEON,        {2,2},      2,          (STAFF),    -1,         0,              2,              0,          (ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN),   (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Guaranteed good consumable item on glowing pedestals (scrolls of enchanting, potion of life)
    bc[id++] =  (blueprint){{10, AMULET_LEVEL},{10, 30},   30,     5,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {0,         PEDESTAL,   DUNGEON,        {1,1},      1,          (SCROLL),   SCROLL_ENCHANTING, 0,       2,              0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {0,         PEDESTAL,   DUNGEON,        {1,1},      1,          (POTION),   POTION_LIFE,0,              2,              0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Commutation altars
    bc[id++] =  (blueprint){{13, AMULET_LEVEL},{10, 30},   50,     4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {DF_MAGIC_PIPING,COMMUTATION_ALTAR,DUNGEON,{2,2},   2,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Resurrection altar
    bc[id++] =  (blueprint){{13, AMULET_LEVEL},{10, 30},   30,     4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {DF_MACHINE_FLOOR_TRIGGER_REPEATING, RESURRECTION_ALTAR,DUNGEON, {1,1}, 1, 0, -1,       0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Outsourced item -- same item possibilities as in the good permanent item reward room (plus charms), but directly adopted by 1-2 key machines.
    bc[id++] =  (blueprint){{5, 17},           {0, 0},     20,     4,          0,                  (BP_REWARD | BP_NO_INTERIOR_FLAG),  {
        {0,         0,          0,              {1,1},      1,          (WEAPON),   -1,         0,              0,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_NO_THROWING_WEAPONS | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)},
        {0,         0,          0,              {1,1},      1,          (ARMOR),    -1,         0,              0,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)},
        {0,         0,          0,              {2,2},      2,          (STAFF),    -1,         0,              0,              0,          ITEM_KIND_AUTO_ID, (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)},
        {0,         0,          0,              {1,2},      1,          (CHARM),    -1,         0,              0,              0,          ITEM_KIND_AUTO_ID, (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Dungeon -- two allies chained up for the taking
    bc[id++] =  (blueprint){{5, AMULET_LEVEL}, {30, 80},   12,     5,          0,                  (BP_ROOM | BP_REWARD),  {
        {0,         VOMIT,      SURFACE,        {2,2},      2,          0,          -1,         0,              2,              (HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING)},
        {DF_AMBIENT_BLOOD,MANACLE_T,SURFACE,    {1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_AMBIENT_BLOOD,MANACLE_L,SURFACE,    {1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_BONES,  0,          0,              {2,3},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_VOMIT,  0,          0,              {2,3},      1,          0,          -1,         0,              1,              0,          0,          0},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Kennel -- allies locked in cages in an open room; choose one or two to unlock and take with you.
    bc[id++] =  (blueprint){{5, AMULET_LEVEL}, {30, 80},   15,     4,          0,                  (BP_ROOM | BP_REWARD),  {
        {0,         MONSTER_CAGE_CLOSED,DUNGEON,{3,5},      3,          0,          -1,         0,              2,              (HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {0,         0,          0,              {1,2},      1,          KEY,        KEY_CAGE,   0,              1,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS),(MF_PERMIT_BLOCKING | MF_GENERATE_ITEM | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_SKELETON_KEY | MF_KEY_DISPOSABLE)},
        {DF_AMBIENT_BLOOD, 0,   0,              {3,5},      3,          0,          -1,         0,              1,              0,          0,          0},
        {DF_BONES,  0,          0,              {3,5},      3,          0,          -1,         0,              1,              0,          0,          0},
        {0,         TORCH_WALL, DUNGEON,        {2,3},      2,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)}}};
    // Vampire lair -- allies locked in cages and chained in a hidden room with a vampire in a coffin; vampire has one cage key.
    bc[id++] =  (blueprint){{10, AMULET_LEVEL},{50, 80},   5,      4,          0,                  (BP_ROOM | BP_REWARD | BP_SURROUND_WITH_WALLS | BP_PURGE_INTERIOR), {
        {DF_AMBIENT_BLOOD,0,    0,              {1,2},      1,          0,          -1,         0,              2,              (HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_AMBIENT_BLOOD,MONSTER_CAGE_CLOSED,DUNGEON,{2,4},2,          0,          -1,         0,              2,              (HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE | MF_NOT_IN_HALLWAY)},
        {DF_TRIGGER_AREA,COFFIN_CLOSED,0,       {1,1},      1,          KEY,        KEY_CAGE,   getVampireBossMonsterId(), 1,   0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS),(MF_GENERATE_ITEM | MF_SKELETON_KEY | MF_MONSTER_TAKE_ITEM | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN | MF_KEY_DISPOSABLE)},
        {DF_AMBIENT_BLOOD,SECRET_DOOR,DUNGEON,  {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)}}};
    // Legendary ally -- approach the altar with the crystal key to activate a portal and summon a legendary ally.
    bc[id++] =  (blueprint){{8, AMULET_LEVEL}, {30, 50},   15,     2,          0,                  (BP_ROOM | BP_REWARD),  {
        {DF_LUMINESCENT_FUNGUS, ALTAR_KEYHOLE, DUNGEON, {1,1}, 1,       KEY,        KEY_PORTAL, 0,              2,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS),(MF_GENERATE_ITEM | MF_NOT_IN_HALLWAY | MF_NEAR_ORIGIN | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_KEY_DISPOSABLE)},
        {DF_LUMINESCENT_FUNGUS, PORTAL, DUNGEON,{1,1},      1,          0,          -1,         0,              2,              HORDE_MACHINE_LEGENDARY_ALLY,0, (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN)}}};
    // Goblin warren
    bc[id++] =  (blueprint){{5, 15},           {100, 200}, 15,     9,          DP_GOBLIN_WARREN,   (BP_ROOM | BP_REWARD | BP_MAXIMIZE_INTERIOR | BP_REDESIGN_INTERIOR),    {
        {0,         MUD_FLOOR,  DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         MUD_DOORWAY,DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {0,         MUD_WALL,   DUNGEON,        {1,1},      100,        0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_EVERYWHERE)},
        {0,         PEDESTAL,   DUNGEON,        {1,1},      1,          (SCROLL),   SCROLL_ENCHANTING, getWarrenBossMonsterId(),2,          0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_MONSTER_SLEEPING | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {0,         PEDESTAL,   DUNGEON,        {1,1},      1,          (POTION),   POTION_LIFE, getWarrenBossMonsterId(),      2,          0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_MONSTER_SLEEPING | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {0,         0,          0,              {5, 8},     5,          0,          -1,         0,              2,              HORDE_MACHINE_GOBLIN_WARREN,    0,  (MF_GENERATE_HORDE | MF_NOT_IN_HALLWAY | MF_MONSTER_SLEEPING)},
        {0,         0,          0,              {2,3},      2,          (WEAPON|ARMOR), -1,     0,              1,              0,          0,          (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_HAY,    0,          0,              {10, 15},   1,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY)},
        {DF_JUNK,   0,          0,              {7, 12},    1,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY)}}};
    // Sentinel sanctuary
    bc[id++] =  (blueprint){{10, 23},           {100, 200}, 15,  10,           DP_SENTINEL_SANCTUARY, (BP_ROOM | BP_REWARD | BP_MAXIMIZE_INTERIOR | BP_REDESIGN_INTERIOR), {
        {0,         MARBLE_FLOOR,DUNGEON,       {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         CRYSTAL_WALL,DUNGEON,       {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_BUILD_IN_WALLS | MF_EVERYWHERE)},
        {0,         PEDESTAL, DUNGEON, {1,1},   1,          (SCROLL),   SCROLL_ENCHANTING,0,    2,              0,              (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {0,         PEDESTAL, DUNGEON, {1,1},   1,          (POTION),   POTION_LIFE,0,          2,              0,              (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {0,         MACHINE_GLYPH,DUNGEON,      {30, 35},   20,         0,          -1,         0,              1,              0,          0,          (MF_PERMIT_BLOCKING)},
        {0,         STATUE_INERT,DUNGEON,       {3, 5},     3,          0,          -1,         getSentinelMonsterId(), 2,      0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         STATUE_INERT,DUNGEON,       {10, 15},   8,          0,          -1,         getSentinelMonsterId(), 2,      0,          0,          MF_BUILD_IN_WALLS},
        {0,         0,          0,              {4, 6},     4,          0,          -1,         getGuardianMonsterId(), 1,      0,          0,          MF_TREAT_AS_BLOCKING},
        {0,         0,          0,              {0, 2},     0,          0,          -1,         getWingedGuardianMonsterId(),1, 0,          0,          MF_TREAT_AS_BLOCKING},
        {0,         0,          0,              {2,3},      2,          (SCROLL | POTION), -1,  0,              1,              0,          0,          (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    
    // -- AMULET HOLDER --
    // Statuary -- key on an altar, area full of statues; take key to cause statues to burst and reveal monsters
    bc[id++] =  (blueprint){{10, AMULET_LEVEL},{35, 40},   0,      4,          0,                  (BP_PURGE_INTERIOR | BP_OPEN_INTERIOR), {
        {DF_LUMINESCENT_FUNGUS, AMULET_SWITCH, DUNGEON, {1,1}, 1,       AMULET,     -1,         0,              2,              0,          0,          (MF_GENERATE_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         FUNGUS_FOREST,SURFACE,      {2,3},      0,          0,          -1,         0,              2,              0,          0,          MF_NOT_IN_HALLWAY},
        {0,         STATUE_INSTACRACK,DUNGEON,  {1,1},      1,          0,          -1,         getWardenMonsterId(), 1,        0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_IMPREGNABLE)},
        {0,         TORCH_WALL, DUNGEON,        {3,4},      0,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)}}};
    
    // -- VESTIBULES --
    
    // Plain locked door, key guarded by an adoptive room
    bc[id++] =  (blueprint){{1, AMULET_LEVEL}, {1, 1},     100,        1,      0,                  (BP_VESTIBULE), {
        {0,         LOCKED_DOOR, DUNGEON,       {1,1},      1,          KEY,        KEY_DOOR,   0,              1,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS), (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_GENERATE_ITEM | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_KEY_DISPOSABLE | MF_IMPREGNABLE)}}},
    // Plain secret door
    bc[id++] =  (blueprint){{2, AMULET_LEVEL}, {1, 1},     1,      1,          0,                  (BP_VESTIBULE), {
        {0,         SECRET_DOOR, DUNGEON,       {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING)}}};
    // Lever and either an exploding wall or a portcullis
    bc[id++] =  (blueprint){{4, AMULET_LEVEL}, {1, 1},     8,      3,          0,                  (BP_VESTIBULE), {
        {0,         WORM_TUNNEL_OUTER_WALL,DUNGEON,{1,1},   1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_IMPREGNABLE | MF_ALTERNATIVE)},
        {0,         PORTCULLIS_CLOSED,DUNGEON,  {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_IMPREGNABLE | MF_ALTERNATIVE)},
        {0,         WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Flammable barricade in the doorway -- burn the wooden barricade to enter
    bc[id++] =  (blueprint){{1, 6},            {1, 1},     10,     3,          0,                  (BP_VESTIBULE), {
        {0,         WOODEN_BARRICADE,DUNGEON,   {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {0,         0,          0,              {1,1},      1,          WEAPON,     INCENDIARY_DART, 0,         1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         0,          0,              {1,1},      1,          POTION,     POTION_INCINERATION, 0,     1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)}}};
    // Statue in the doorway -- use a scroll of shattering to enter
    bc[id++] =  (blueprint){{1, AMULET_LEVEL}, {1, 1},     6,      2,          0,                  (BP_VESTIBULE), {
        {0,         STATUE_INERT_DOORWAY,DUNGEON,       {1,1},1,        0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {0,         0,          0,              {1,1},      1,          SCROLL,     SCROLL_SHATTERING, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY)}}},
    // Statue in the doorway -- bursts to reveal monster
    bc[id++] =  (blueprint){{5, AMULET_LEVEL}, {2, 2},     6,      2,          0,                  (BP_VESTIBULE), {
        {0,         STATUE_DORMANT_DOORWAY,DUNGEON,     {1, 1}, 1,      0,          -1,         0,              1,              HORDE_MACHINE_STATUE,0, (MF_PERMIT_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {0,         MACHINE_TRIGGER_FLOOR,DUNGEON,{0,0},    1,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)}}};
    // Throwing tutorial -- toss an item onto the pressure plate to retract the portcullis
    bc[id++] =  (blueprint){{1, 4},            {70, 70},   8,      3,          0,                  (BP_VESTIBULE), {
        {DF_MEDIUM_HOLE, MACHINE_PRESSURE_PLATE, LIQUID, {1,1}, 1,      0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         PORTCULLIS_CLOSED,DUNGEON,  {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_IMPREGNABLE | MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {0,         WORM_TUNNEL_OUTER_WALL,DUNGEON,{1,1},   1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_IMPREGNABLE | MF_ALTERNATIVE)}}};
    // Pit traps -- area outside entrance is full of pit traps
    bc[id++] =  (blueprint){{1, AMULET_LEVEL}, {30, 60},   8,      3,          0,                  (BP_VESTIBULE | BP_OPEN_INTERIOR | BP_NO_INTERIOR_FLAG),    {
        {0,         DOOR,       DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {0,         SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_IMPREGNABLE | MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {0,         TRAP_DOOR_HIDDEN,DUNGEON,   {60, 60},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)}}};
    // Beckoning obstacle -- a mirrored totem guards the door, and glyph are around the doorway.
    bc[id++] =  (blueprint){{5, AMULET_LEVEL}, {15, 30},   8,      3,          0,                  (BP_VESTIBULE | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR), {
        {0,         DOOR,       DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {0,         0,          0,              {1,1},      1,          0,          -1,         getMirrorMonsterId(), 3,        0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN | MF_FAR_FROM_ORIGIN)},
        {0,         MACHINE_GLYPH,DUNGEON,      {1,1},      0,          0,          -1,         0,              1,              0,          0,          (MF_NEAR_ORIGIN | MF_EVERYWHERE)},
        {0,         MACHINE_GLYPH,DUNGEON,      {3,5},      2,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)}}};
    // Guardian obstacle -- a guardian is in the door on a glyph, with other glyphs scattered around.
    bc[id++] =  (blueprint){{6, AMULET_LEVEL}, {25, 25},   8,      4,          0,                  (BP_VESTIBULE | BP_OPEN_INTERIOR),  {
        {0,         DOOR,       DUNGEON,        {1,1},      1,          0,          0,          getGuardianMonsterId(), 2,      0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_ALTERNATIVE)},
        {0,         DOOR,       DUNGEON,        {1,1},      1,          0,          0,          getWingedGuardianMonsterId(), 2,0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_ALTERNATIVE)},
        {0,         MACHINE_GLYPH,DUNGEON,      {10,10},    3,          0,          -1,         0,              1,              0,          0,          (MF_PERMIT_BLOCKING| MF_NEAR_ORIGIN)},
        {0,         MACHINE_GLYPH,DUNGEON,      {1,1},      0,          0,          -1,         0,              2,              0,          0,          (MF_EVERYWHERE | MF_PERMIT_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    
    // -- KEY HOLDERS --
    
    // Nested item library -- can check one item out at a time, and one is a disposable key to another reward room
    bc[id++] =  (blueprint){{1, AMULET_LEVEL}, {30, 50},   35,     7,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_ADOPT_ITEM | BP_IMPREGNABLE), {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         WALL,       DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE | MF_EVERYWHERE)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {1,2},      1,          (WEAPON|ARMOR|WAND),-1, 0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_PLAYER_AVOIDS), (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {1,2},      1,          (STAFF|RING|CHARM),-1,  0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {0,         ALTAR_CAGE_OPEN,DUNGEON,    {1,1},      1,          0,          -1,         0,              2,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS | ITEM_MAX_CHARGES_KNOWN),    (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {0,         STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Secret room -- key on an altar in a secret room
    bc[id++] =  (blueprint){{1, AMULET_LEVEL}, {15, 100},  1,      2,          0,                  (BP_ROOM | BP_ADOPT_ITEM), {
        {0,         ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          ITEM_PLAYER_AVOIDS, (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)}}};
    // Throwing tutorial -- toss an item onto the pressure plate to retract the cage and reveal the key
    bc[id++] =  (blueprint){{1, 4},            {70, 80},   8,      2,          0,                  (BP_ADOPT_ITEM), {
        {0,         ALTAR_CAGE_RETRACTABLE,DUNGEON,{1,1},   1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_IMPREGNABLE | MF_NOT_IN_HALLWAY)},
        {DF_MEDIUM_HOLE, MACHINE_PRESSURE_PLATE, LIQUID, {1,1}, 1,      0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Rat trap -- getting the key triggers paralysis vents nearby and also causes rats to burst out of the walls
    bc[id++] =  (blueprint){{1,8},             {30, 70},   7,      3,          0,                  (BP_ADOPT_ITEM | BP_ROOM),  {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         MACHINE_PARALYSIS_VENT_HIDDEN,DUNGEON,{1,1},1,      0,          -1,         0,              2,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)},
        {0,         RAT_TRAP_WALL_DORMANT,DUNGEON,{10,20},  5,          0,          -1,         getRatTrapMonsterId(), 1,       0,          0,          (MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_NOT_ON_LEVEL_PERIMETER)}}};
    // Fun with fire -- trigger the fire trap and coax the fire over to the wooden barricade surrounding the altar and key
    bc[id++] =  (blueprint){{3, 10},           {80, 100},  10,     6,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR), {
        {DF_SURROUND_WOODEN_BARRICADE,ALTAR_INERT,DUNGEON,{1,1},1,      0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         GRASS,      SURFACE,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE | MF_ALTERNATIVE)},
        {DF_SWAMP,  0,          0,              {4,4},      2,          0,          -1,         0,              2,              0,          0,          (MF_ALTERNATIVE | MF_FAR_FROM_ORIGIN)},
        {0,         FLAMETHROWER_HIDDEN,DUNGEON,{1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NEAR_ORIGIN)},
        {0,         GAS_TRAP_POISON_HIDDEN,DUNGEON,{3, 3},  1,          0,          -1,         0,              5,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_ALTERNATIVE)},
        {0,         0,          0,              {2,2},      1,          POTION,     POTION_LICHEN, 0,           3,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)}}};
    // Flood room -- key on an altar in a room with pools of eel-infested waters; take key to flood room with shallow water
    bc[id++] =  (blueprint){{3, AMULET_LEVEL}, {80, 180},  10,     4,          0,                  (BP_ROOM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS | BP_PURGE_PATHING_BLOCKERS | BP_ADOPT_ITEM),  {
        {0,         FLOOR_FLOODABLE,LIQUID,     {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              5,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_SPREADABLE_WATER_POOL,0,0,          {2, 4},     1,          0,          -1,         0,              5,              HORDE_MACHINE_WATER_MONSTER,0,MF_GENERATE_HORDE},
        {DF_GRASS,  FOLIAGE,    SURFACE,        {3, 4},     3,          0,          -1,         0,              1,              0,          0,          0}}};
    // Fire trap room -- key on an altar, pools of water, fire traps all over the place.
    bc[id++] =  (blueprint){{4, AMULET_LEVEL}, {80, 180},  6,      5,          0,                  (BP_ROOM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS | BP_PURGE_PATHING_BLOCKERS | BP_ADOPT_ITEM),  {
        {0,         ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         0,          0,              {1, 1},     1,          0,          -1,         0,              4,              0,          0,          MF_BUILD_AT_ORIGIN},
        {0,         FLAMETHROWER_HIDDEN,DUNGEON,{40, 60},   20,         0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {DF_DEEP_WATER_POOL,0,  0,              {4, 4},     1,          0,          -1,         0,              4,              HORDE_MACHINE_WATER_MONSTER,0,MF_GENERATE_HORDE},
        {DF_GRASS,  FOLIAGE,    SURFACE,        {3, 4},     3,          0,          -1,         0,              1,              0,          0,          0}}};
    // Thief area -- empty altar, monster with item, permanently fleeing.
    bc[id++] =  (blueprint){{3, AMULET_LEVEL}, {15, 20},   10,     2,          0,                  (BP_ADOPT_ITEM),    {
        {DF_LUMINESCENT_FUNGUS, ALTAR_INERT,DUNGEON,{1,1},  1,          0,          -1,         0,              2,              HORDE_MACHINE_THIEF,0,          (MF_ADOPT_ITEM | MF_BUILD_AT_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_GENERATE_HORDE | MF_MONSTER_TAKE_ITEM | MF_MONSTER_FLEEING)},
        {0,         STATUE_INERT,0,             {3, 5},     2,          0,          -1,         0,              2,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Collapsing floor area -- key on an altar in an area; take key to cause the floor of the area to collapse
    bc[id++] =  (blueprint){{1, AMULET_LEVEL}, {45, 65},   13,     3,          0,                  (BP_ADOPT_ITEM | BP_TREAT_AS_BLOCKING), {
        {0,         FLOOR_FLOODABLE,DUNGEON,    {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         ALTAR_SWITCH_RETRACTING,DUNGEON,{1,1},  1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_ADD_MACHINE_COLLAPSE_EDGE_DORMANT,0,0,{3, 3},   2,          0,          -1,         0,              3,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)}}};
    // Pit traps -- key on an altar, room full of pit traps
    bc[id++] =  (blueprint){{1, AMULET_LEVEL}, {30, 100},  10,     3,          0,                  (BP_ROOM | BP_ADOPT_ITEM),  {
        {0,         ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         TRAP_DOOR_HIDDEN,DUNGEON,   {30, 40},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {0,         SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)}}};
    // Levitation challenge -- key on an altar, room filled with pit, levitation or lever elsewhere on level, bridge appears when you grab the key/lever.
    bc[id++] =  (blueprint){{1, 13},           {75, 120},  10,     9,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_SURROUND_WITH_WALLS),  {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         TORCH_WALL, DUNGEON,        {1,4},      0,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              3,              0,          0,          MF_BUILD_AT_ORIGIN},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM,LIQUID,{120, 120},1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM_WITH_HIDDEN_BRIDGE,LIQUID,{1,1},1,0,      0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {0,         0,          0,              {1,1},      1,          POTION,     POTION_LEVITATION, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL | MF_ALTERNATIVE)}}};
    // Web climbing -- key on an altar, room filled with pit, spider at altar to shoot webs, bridge appears when you grab the key
    bc[id++] =  (blueprint){{7, AMULET_LEVEL}, {55, 90},   10,     7,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_SURROUND_WITH_WALLS),  {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         getWebberMonsterId(), 3,        0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_IN_VIEW_OF_ORIGIN)},
        {0,         TORCH_WALL, DUNGEON,        {1,4},      0,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              3,              0,          0,          MF_BUILD_AT_ORIGIN},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM,LIQUID,   {120, 120}, 1,      0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM_WITH_HIDDEN_BRIDGE,LIQUID,{1,1},1,0,      0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)}}};
    // Lava moat room -- key on an altar, room filled with lava, levitation/fire immunity/lever elsewhere on level, lava retracts when you grab the key/lever
    bc[id++] =  (blueprint){{3, 13},           {75, 120},  7,      7,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR),  {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {0,         LAVA,       LIQUID,         {60,60},    1,          0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_LAVA_RETRACTABLE, 0, 0,             {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {0,         0,          0,              {1,1},      1,          POTION,     POTION_LEVITATION, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         0,          0,              {1,1},      1,          POTION,     POTION_FIRE_IMMUNITY, 0,    1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL | MF_ALTERNATIVE)}}};
    // Lava moat area -- key on an altar, surrounded with lava, levitation/fire immunity elsewhere on level, lava retracts when you grab the key
    bc[id++] =  (blueprint){{3, 13},           {40, 60},   3,      5,          0,                  (BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_TREAT_AS_BLOCKING),  {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_BUILD_AT_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         LAVA,       LIQUID,         {60,60},    1,          0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_LAVA_RETRACTABLE, 0, 0,             {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {0,         0,          0,              {1,1},      1,          POTION,     POTION_LEVITATION, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         0,          0,              {1,1},      1,          POTION,     POTION_FIRE_IMMUNITY, 0,    1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)}}};
    // Poison gas -- key on an altar; take key to cause a caustic gas vent to appear and the door to be blocked; there is a hidden trapdoor or an escape item somewhere inside
    bc[id++] =  (blueprint){{4, AMULET_LEVEL}, {35, 60},   7,      7,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM), {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING)},
        {0,         MACHINE_POISON_GAS_VENT_HIDDEN,DUNGEON,{1,2}, 1,    0,          -1,         0,              2,              0,          0,          0},
        {0,         TRAP_DOOR_HIDDEN,DUNGEON,   {1,1},      1,          0,          -1,         0,              2,              0,          0,          MF_ALTERNATIVE},
        {0,         0,          0,              {1,1},      1,          SCROLL,     SCROLL_TELEPORT, 0,         2,              0,          0,          (MF_GENERATE_ITEM | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         0,          0,              {1,1},      1,          POTION,     POTION_DESCENT, 0,          2,              0,          0,          (MF_GENERATE_ITEM | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         WALL_LEVER_HIDDEN_DORMANT,DUNGEON,{1,1},1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS)},
        {0,         PORTCULLIS_DORMANT,DUNGEON,{1,1},       1,          0,          0,          0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING)}}};
    // Explosive situation -- key on an altar; take key to cause a methane gas vent to appear and a pilot light to ignite
    bc[id++] =  (blueprint){{7, AMULET_LEVEL}, {80, 90},   10,     5,          0,                  (BP_ROOM | BP_PURGE_LIQUIDS | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM),  {
        {0,         DOOR,       DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {0,         FLOOR,      DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_FAR_FROM_ORIGIN)},
        {0,         MACHINE_METHANE_VENT_HIDDEN,DUNGEON,{1,1}, 1,       0,          -1,         0,              1,              0,          0,          MF_NEAR_ORIGIN},
        {0,         PILOT_LIGHT_DORMANT,DUNGEON,{1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_BUILD_IN_WALLS)}}};
    // Burning grass -- key on an altar; take key to cause pilot light to ignite grass in room
    bc[id++] =  (blueprint){{1, 7},            {40, 110},  10,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM | BP_OPEN_INTERIOR),  {
        {DF_SMALL_DEAD_GRASS,ALTAR_SWITCH_RETRACTING,DUNGEON,{1,1},1,   0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_FAR_FROM_ORIGIN)},
        {DF_DEAD_FOLIAGE,0,     SURFACE,        {2,3},      0,          0,          -1,         0,              1,              0,          0,          0},
        {0,         FOLIAGE,    SURFACE,        {1,4},      0,          0,          -1,         0,              1,              0,          0,          0},
        {0,         GRASS,      SURFACE,        {10,25},    0,          0,          -1,         0,              1,              0,          0,          0},
        {0,         DEAD_GRASS, SURFACE,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         PILOT_LIGHT_DORMANT,DUNGEON,{1,1},      1,          0,          -1,         0,              1,              0,          0,          MF_NEAR_ORIGIN | MF_BUILD_IN_WALLS}}};
    // Statuary -- key on an altar, area full of statues; take key to cause statues to burst and reveal monsters
    bc[id++] =  (blueprint){{10, AMULET_LEVEL},{35, 90},   10,     2,          0,                  (BP_ADOPT_ITEM | BP_NO_INTERIOR_FLAG),  {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         STATUE_DORMANT,DUNGEON,     {3,5},      3,          0,          -1,         0,              2,              HORDE_MACHINE_STATUE,0, (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN)}}};
    // Guardian water puzzle -- key held by a guardian, flood trap in the room, glyphs scattered. Lure the guardian into the water to have him drop the key.
    bc[id++] =  (blueprint){{4, AMULET_LEVEL}, {35, 70},   8,      4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM), {
        {0,         0,          0,              {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {0,         0,          0,              {1,1},      1,          0,          -1,         getGuardianMonsterId(), 2,      0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_MONSTER_TAKE_ITEM)},
        {0,         FLOOD_TRAP,DUNGEON,         {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         MACHINE_GLYPH,DUNGEON,      {1,1},      4,          0,          -1,         0,              2,              0,          0,          (MF_EVERYWHERE | MF_NOT_IN_HALLWAY)}}};
    // Guardian gauntlet -- key in a room full of guardians, glyphs scattered and unavoidable.
    bc[id++] =  (blueprint){{6, AMULET_LEVEL}, {50, 95},   10,     6,          0,                  (BP_ROOM | BP_ADOPT_ITEM),  {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {0,         DOOR,       DUNGEON,        {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {0,         0,          0,              {3,6},      3,          0,          -1,         getGuardianMonsterId(), 2,      0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         0,          0,              {1,2},      1,          0,          -1,         getWingedGuardianMonsterId(),2, 0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {0,         MACHINE_GLYPH,DUNGEON,      {10,15},   10,          0,          -1,         0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         MACHINE_GLYPH,DUNGEON,      {1,1},      0,          0,          -1,         0,              2,              0,          0,          (MF_EVERYWHERE | MF_PERMIT_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Guardian corridor -- key in a small room, with a connecting corridor full of glyphs, one guardian blocking the corridor.
    bc[id++] =  (blueprint){{4, AMULET_LEVEL}, {85, 100},   5,     7,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_SURROUND_WITH_WALLS),        {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         getGuardianMonsterId(), 3,      0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN  | MF_ALTERNATIVE)},
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         getWingedGuardianMonsterId(),3, 0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN  | MF_ALTERNATIVE)},
        {0,         MACHINE_GLYPH,DUNGEON,      {3,5},      2,          0,          0,          0,              2,              0,          0,          MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY},
        {0,         0,          0,              {1,1},      1,          0,          0,          0,              3,              0,          0,          MF_BUILD_AT_ORIGIN},
        {0,         WALL,DUNGEON,               {80,80},    1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {0,         MACHINE_GLYPH,DUNGEON,      {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {0,         MACHINE_GLYPH,DUNGEON,      {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_NOT_IN_HALLWAY | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Summoning circle -- key in a room with an eldritch totem, glyphs unavoidable. // DISABLED. (Not fun enough.)
    bc[id++] =  (blueprint){{12, AMULET_LEVEL}, {50, 100}, 0,      2,          0,                  (BP_ROOM | BP_OPEN_INTERIOR | BP_ADOPT_ITEM),   {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {DF_GLYPH_CIRCLE,0,     0,              {1,1},      1,          0,          -1,         /*MK_ELDRITCH_TOTEM*/ 0, 3,     0,          0,          (MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Beckoning obstacle -- key surrounded by glyphs in a room with a mirrored totem.
    bc[id++] =  (blueprint){{5, AMULET_LEVEL}, {60, 100},  10,     4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_ADOPT_ITEM), {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN | MF_IN_VIEW_OF_ORIGIN)},
        {0,         0,          0,              {1,1},      1,          0,          -1,         getMirrorMonsterId(), 3,        0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN)},
        {0,         0,          0,              {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {0,         MACHINE_GLYPH,DUNGEON,      {3,5},      2,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)}}};
    // Worms in the walls -- key on altar; take key to cause underworms to burst out of the walls
    bc[id++] =  (blueprint){{12,AMULET_LEVEL}, {7, 7},     7,      2,          0,                  (BP_ADOPT_ITEM),    {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         WALL_MONSTER_DORMANT,DUNGEON,{5,8},     5,          0,          -1,         getDiggerMonsterId(), 1,        0,          0,          (MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_NOT_ON_LEVEL_PERIMETER)}}};
    // Mud pit -- key on an altar, room full of mud, take key to cause bog monsters to spawn in the mud
    bc[id++] =  (blueprint){{12, AMULET_LEVEL},{40, 90},   10,     3,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS),  {
        {DF_SWAMP,      0,      0,              {5,5},      0,          0,          -1,         0,              1,              0,          0,          0},
        {DF_SWAMP,  ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {DF_MUD_DORMANT,0,      0,              {3,4},      3,          0,          -1,         0,              1,              HORDE_MACHINE_MUD,0,    (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT)}}},
    // Electric crystals -- key caged on an altar, darkened crystal globes around the room, lightning the globes to release the key.
    bc[id++] =  (blueprint){{6, AMULET_LEVEL},{40, 60},    10,     4,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_PURGE_INTERIOR),  {
        {0,         CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         ELECTRIC_CRYSTAL_OFF,DUNGEON,{3,4},     3,          0,          -1,         0,              3,              0,          0,          (MF_NOT_IN_HALLWAY | MF_IMPREGNABLE)},
        {0,         ALTAR_CAGE_RETRACTABLE,DUNGEON,{1,1},   1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_IMPREGNABLE | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {0,         TURRET_LEVER, DUNGEON,      {7,9},      4,          0,          -1,         getSparkMonsterId(), 3,         0,          0,          (MF_BUILD_IN_WALLS | MF_MONSTERS_DORMANT)}}};
    // Zombie crypt -- key on an altar; coffins scattered around; brazier in the room; take key to cause zombies to burst out of all of the coffins
    bc[id++] =  (blueprint){{12, AMULET_LEVEL},{60, 90},   10,     8,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_PURGE_INTERIOR), {
        {0,         DOOR,       DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {DF_BONES,  0,          0,              {3,4},      2,          0,          -1,         0,              1,              0,          0,          0},
        {DF_ASH,    0,          0,              {3,4},      2,          0,          -1,         0,              1,              0,          0,          0},
        {DF_AMBIENT_BLOOD,0,    0,              {1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_AMBIENT_BLOOD,0,    0,              {1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         BRAZIER,    DUNGEON,        {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         COFFIN_CLOSED, DUNGEON,     {6,8},      1,          0,          0,          getUndeadMonsterId(), 2,        0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_MONSTERS_DORMANT)}}};
    // Haunted house -- key on an altar; take key to cause the room to darken, ectoplasm to cover everything and phantoms to appear
    bc[id++] =  (blueprint){{16, AMULET_LEVEL},{45, 150},  10,     4,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS), {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         DARK_FLOOR_DORMANT,DUNGEON, {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         DARK_FLOOR_DORMANT,DUNGEON, {4,5},      4,          0,          -1,         getInvisibleMonsterId(), 1,     0,          0,          (MF_MONSTERS_DORMANT)},
        {0,         HAUNTED_TORCH_DORMANT,DUNGEON,{5,10},   3,          0,          -1,         0,              2,              0,          0,          (MF_BUILD_IN_WALLS)}}};
    // Worm tunnels -- hidden lever causes tunnels to open up revealing worm areas and a key
    bc[id++] =  (blueprint){{8, AMULET_LEVEL},{80, 175},   10,     6,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_MAXIMIZE_INTERIOR | BP_SURROUND_WITH_WALLS),  {
        {0,         ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {0,         0,          0,              {3,6},      3,          0,          -1,         getDiggerMonsterId(), 1,        0,          0,          0},
        {0,         GRANITE,    DUNGEON,        {150,150},  1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_WORM_TUNNEL_MARKER_DORMANT,GRANITE,DUNGEON,{0,0},0,         0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE | MF_PERMIT_BLOCKING)},
        {DF_TUNNELIZE,WORM_TUNNEL_OUTER_WALL,DUNGEON,{1,1}, 1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING)},
        {0,         WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Gauntlet -- key on an altar; take key to cause turrets to emerge
    bc[id++] =  (blueprint){{5, 24},           {35, 90},   10,     2,          0,                  (BP_ADOPT_ITEM | BP_NO_INTERIOR_FLAG),  {
        {0,         ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING)},
        {0,         TURRET_DORMANT,DUNGEON,     {4,6},      4,          0,          -1,         0,              2,              HORDE_MACHINE_TURRET,0, (MF_TREAT_AS_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_IN_VIEW_OF_ORIGIN)}}};
    // Boss -- key is held by a boss atop a pile of bones in a secret room. A few fungus patches light up the area.
    bc[id++] =  (blueprint){{5, AMULET_LEVEL}, {40, 100},  18,     3,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS), {
        {DF_BONES,  SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {DF_LUMINESCENT_FUNGUS, STATUE_INERT,DUNGEON,{7,7}, 0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {DF_BONES,  0,          0,              {1,1},      1,          0,          -1,         0,              1,              HORDE_MACHINE_BOSS, 0,  (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_MONSTER_TAKE_ITEM | MF_GENERATE_HORDE | MF_MONSTER_SLEEPING)}}};
    
    // -- FLAVOR MACHINES --
    
    // Bloodwort -- bloodwort stalk, some pods, and surrounding grass
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {5, 5},     0,          2,      0,                  (BP_TREAT_AS_BLOCKING), {
        {DF_GRASS,  BLOODFLOWER_STALK, SURFACE, {1, 1},     1,          0,          -1,         0,              0,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_NOT_IN_HALLWAY)},
        {DF_BLOODFLOWER_PODS_GROW_INITIAL,0, 0, {1, 1},     1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_TREAT_AS_BLOCKING)}}};
    // Shrine -- safe haven constructed and abandoned by a past adventurer
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {15, 25},   0,          3,      0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR), {
        {0,         SACRED_GLYPH,  DUNGEON,     {1, 1},     1,          0,          -1,         0,              3,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {0,         HAVEN_BEDROLL, SURFACE,     {1, 1},     1,          0,          -1,         0,              2,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {0,         BONES,      SURFACE,        {1, 1},     1,          (POTION|SCROLL|WEAPON|ARMOR|RING),-1,0, 2,              0,          0,          (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Idyll -- ponds and some grass and forest
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {80, 120},  0,      2,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_GRASS,  FOLIAGE,    SURFACE,        {3, 4},     3,          0,          -1,         0,              1,              0,          0,          0},
        {DF_DEEP_WATER_POOL,0,  0,              {2, 3},     2,          0,          -1,         0,              5,              0,          0,          (MF_NOT_IN_HALLWAY)}}};
    // Swamp -- mud, grass and some shallow water
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {50, 65},   0,      2,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_SWAMP,  0,          0,              {6, 8},     3,          0,          -1,         0,              1,              0,          0,          0},
        {DF_DEEP_WATER_POOL,0,  0,              {0, 1},     0,          0,          -1,         0,              3,              0,          0,          (MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING)}}};
    // Camp -- hay, junk, urine, vomit
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {40, 50},   0,      4,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_HAY,    0,          0,              {1, 3},     1,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN)},
        {DF_JUNK,   0,          0,              {1, 2},     1,          0,          -1,         0,              3,              0,          0,          (MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN)},
        {DF_URINE,  0,          0,              {3, 5},     1,          0,          -1,         0,              1,              0,          0,          MF_IN_VIEW_OF_ORIGIN},
        {DF_VOMIT,  0,          0,              {0, 2},     0,          0,          -1,         0,              1,              0,          0,          MF_IN_VIEW_OF_ORIGIN}}};
    // Remnant -- carpet surrounded by ash and with some statues
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {80, 120},  0,      2,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_REMNANT, 0,         0,              {6, 8},     3,          0,          -1,         0,              1,              0,          0,          0},
        {0,         STATUE_INERT,DUNGEON,       {3, 5},     2,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING)}}};
    // Dismal -- blood, bones, charcoal, some rubble
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {60, 70},   0,      3,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_AMBIENT_BLOOD, 0,   0,              {5,10},     3,          0,          -1,         0,              1,              0,          0,          MF_NOT_IN_HALLWAY},
        {DF_ASH,    0,          0,              {4, 8},     2,          0,          -1,         0,              1,              0,          0,          MF_NOT_IN_HALLWAY},
        {DF_BONES,  0,          0,              {3, 5},     2,          0,          -1,         0,              1,              0,          0,          MF_NOT_IN_HALLWAY}}};
    // Chasm catwalk -- narrow bridge over a chasm, possibly under fire from a turret or two
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL-1},{40, 80},  0,      4,          0,                  (BP_REQUIRE_BLOCKING | BP_OPEN_INTERIOR | BP_NO_INTERIOR_FLAG), {
        {DF_CHASM_HOLE, 0,      0,              {80, 80},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_CATWALK_BRIDGE,0,   0,              {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {0,         MACHINE_TRIGGER_FLOOR, DUNGEON, {0,1},  0,          0,          0,          0,              1,              0,          0,          (MF_NEAR_ORIGIN | MF_PERMIT_BLOCKING)},
        {0,         TURRET_DORMANT,DUNGEON,     {1, 2},     1,          0,          -1,         0,              2,              HORDE_MACHINE_TURRET,0, (MF_TREAT_AS_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_IN_VIEW_OF_ORIGIN)}}};
    // Lake walk -- narrow bridge of shallow water through a lake, possibly under fire from a turret or two
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {40, 80},   0,      3,          0,                  (BP_REQUIRE_BLOCKING | BP_OPEN_INTERIOR | BP_NO_INTERIOR_FLAG), {
        {DF_LAKE_CELL,  0,      0,              {80, 80},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {0,         MACHINE_TRIGGER_FLOOR, DUNGEON, {0,1},  0,          0,          0,          0,              1,              0,          0,          (MF_NEAR_ORIGIN | MF_PERMIT_BLOCKING)},
        {0,         TURRET_DORMANT,DUNGEON,     {1, 2},     1,          0,          -1,         0,              2,              HORDE_MACHINE_TURRET,0, (MF_TREAT_AS_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_IN_VIEW_OF_ORIGIN)}}};
    // Paralysis trap -- already-revealed pressure plate with a few hidden vents nearby.
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {35, 40},   0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {0,         GAS_TRAP_PARALYSIS, DUNGEON, {1,2},     1,          0,          0,          0,              3,              0,          0,          (MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY)},
        {0,         MACHINE_PARALYSIS_VENT_HIDDEN,DUNGEON,{3, 4},2,     0,          0,          0,              3,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)}}};
    // Paralysis trap -- hidden pressure plate with a few vents nearby.
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {35, 40},   0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {0,         GAS_TRAP_PARALYSIS_HIDDEN, DUNGEON, {1,2},1,        0,          0,          0,              3,              0,          0,          (MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY)},
        {0,         MACHINE_PARALYSIS_VENT_HIDDEN,DUNGEON,{3, 4},2,     0,          0,          0,              3,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)}}};
    // Statue comes alive -- innocent-looking statue that bursts to reveal a monster when the player approaches
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {5, 5},     0,      3,          0,                  (BP_NO_INTERIOR_FLAG), {
        {0,         STATUE_DORMANT,DUNGEON,     {1, 1},     1,          0,          -1,         0,              1,              HORDE_MACHINE_STATUE,0, (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {0,         STATUE_DORMANT,DUNGEON,     {1, 1},     1,          0,          -1,         0,              1,              HORDE_MACHINE_STATUE,0, (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_ALTERNATIVE | MF_NOT_ON_LEVEL_PERIMETER)},
        {0,         MACHINE_TRIGGER_FLOOR,DUNGEON,{0,0},    2,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)}}};
    // Worms in the walls -- step on trigger region to cause underworms to burst out of the walls
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {7, 7},     0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {0,         WALL_MONSTER_DORMANT,DUNGEON,{1, 3},    1,          0,          -1,         getDiggerMonsterId(), 1,        0,          0,          (MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_NOT_ON_LEVEL_PERIMETER)},
        {0,         MACHINE_TRIGGER_FLOOR,DUNGEON,{0,0},    2,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)}}};
    // Sentinels
    bc[id++] =  (blueprint){{1,DEEPEST_LEVEL}, {40, 40},   0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {0,         STATUE_DORMANT,DUNGEON,     {3, 3},     3,          0,          -1,         getSentinelMonsterId(), 2,      0,          0,          (MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING | MF_IN_VIEW_OF_ORIGIN)},
        {DF_ASH,    0,          0,              {2, 3},     0,          0,          -1,         0,              0,              0,          0,          0}}};

}
