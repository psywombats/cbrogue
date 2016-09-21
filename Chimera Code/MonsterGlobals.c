/*
 *  MonsterGlobals.c
 *  cBrogue
 *
 *  Created by Aaron King on 9/6/16.
 *  Copyright 2016. All rights reserved.
 */

#include "MonsterGlobals.h"
#include "IncludeGlobals.h"

creatureType *getMonsterCatalog() {
    static creatureType *catalog;
    static boolean catalogInitialized = false;
    
    if (catalogInitialized) {
        return catalog;
    }
    
    catalog = (creatureType *)malloc(sizeof(creatureType) * NUMBER_MONSTER_KINDS);
    memset(catalog, 0, sizeof(creatureType) * NUMBER_MONSTER_KINDS);
    
    unsigned int id;
    
    //   id                           name           ch      color           HP      def     acc     damage          reg move    attack  blood           light   DFChance DFType
    id = MK_YOU;
    catalog[id] = (creatureType) {0, "you",  PLAYER_CHAR,    &playerInLightColor,30, 0,      100,    {1, 2, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0};
    catalog[id].flags = (MONST_MALE | MONST_FEMALE);
    
    id = MK_RAT;
    catalog[id] = (creatureType) {0, "rat",          'r',    &gray,          6,      0,      80,     {1, 3, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      1,      DF_URINE};
    
    id = MK_KOBOLD;
    catalog[id] = (creatureType) {0, "kobold",       'k',    &goblinColor,   7,      0,      80,     {1, 4, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0};
    
    id = MK_JACKAL;
    catalog[id] = (creatureType) {0, "jackal",       'j',    &jackalColor,   8,      0,      70,     {2, 4, 1},      20, 50,     100,    DF_RED_BLOOD,   0,      1,      DF_URINE};
    
    id = MK_EEL;
    catalog[id] = (creatureType) {0, "eel",          'e',    &eelColor,      18,     27,     100,    {3, 7, 2},      5,  50,     100,    0,              0,      0,      0};
    catalog[id].flags = (MONST_RESTRICTED_TO_LIQUID | MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS | MONST_NEVER_SLEEPS);
    
    id = MK_MONKEY;
    catalog[id] = (creatureType) {0, "monkey",       'm',    &ogreColor,     12,     17,     100,    {1, 3, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      1,      DF_URINE},
    catalog[id].abilityFlags = (MA_HIT_STEAL_FLEE);
    
    id = MK_BLOAT;
    catalog[id] = (creatureType) {0, "bloat",        'b',    &poisonGasColor,4,      0,      100,    {0, 0, 0},      5,  100,    100,    DF_PURPLE_BLOOD,0,      0,      DF_BLOAT_DEATH},
    catalog[id].flags = (MONST_FLIES | MONST_FLITS);
    catalog[id].abilityFlags = (MA_KAMIKAZE | MA_DF_ON_DEATH);
    
    id = MK_PIT_BLOAT;
    catalog[id] = (creatureType) {0, "pit bloat",    'b',    &lightBlue,     4,      0,      100,    {0, 0, 0},      5,  100,    100,    DF_PURPLE_BLOOD,0,      0,      DF_HOLE_POTION},
    catalog[id].flags = (MONST_FLIES | MONST_FLITS);
    catalog[id].abilityFlags = (MA_KAMIKAZE | MA_DF_ON_DEATH);
    
    id = MK_GOBLIN;
    catalog[id] = (creatureType) {0, "goblin",       'g',    &goblinColor,   15,     10,     70,     {2, 5, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0},
    catalog[id].abilityFlags = (MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS);
    
    id = MK_GOBLIN_CONJURER;
    catalog[id] = (creatureType) {0, "goblin conjurer",'g',  &goblinConjurerColor, 10,10,    70,     {2, 4, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0},
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CAST_SPELLS_SLOWLY | MONST_CARRY_ITEM_25);
    catalog[id].abilityFlags = (MA_CAST_SUMMON | MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS);
    
    id = MK_GOBLIN_MYSTIC;
    catalog[id] = (creatureType) {0, "goblin mystic",'g',    &goblinMysticColor, 10, 10,     70,     {2, 4, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0},
    
    catalog[id].flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    catalog[id].abilityFlags = (MA_AVOID_CORRIDORS);
    
    id = MK_GOBLIN_TOTEM;
    catalog[id] = (creatureType) {0, "goblin totem", TOTEM_CHAR, &orange,    30,     0,      0,      {0, 0, 0},      0,  100,    300,    DF_RUBBLE_BLOOD,IMP_LIGHT,0,    0,              {BOLT_HASTE, BOLT_SPARK}, (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS), (0)},
    
    id = MK_PINK_JELLY;
    catalog[id] = (creatureType) {0, "pink jelly",   'J',    &pinkJellyColor,50,     0,      85,     {1, 3, 1},      0,  100,    100,    DF_PURPLE_BLOOD,0,      0,      0,              {0},    (MONST_NEVER_SLEEPS), (MA_CLONE_SELF_ON_DEFEND)},
    
    id = MK_TOAD;
    catalog[id] = (creatureType) {0, "toad",         't',    &toadColor,     18,     0,      90,     {1, 4, 1},      10, 100,    100,    DF_GREEN_BLOOD, 0,      0,      0,              {0},    (0), (MA_HIT_HALLUCINATE)},
    
    id = MK_VAMPIRE_BAT;
    catalog[id] = (creatureType) {0, "vampire bat",  'v',    &gray,          18,     25,     100,    {2, 6, 1},      20, 50,     100,    DF_RED_BLOOD,   0,      0,      0,              {0},    (MONST_FLIES | MONST_FLITS), (MA_TRANSFERENCE)},
    
    id = MK_ARROW_TURRET;
    catalog[id] = (creatureType) {0, "arrow turret", TURRET_CHAR,&black,     30,     0,      90,     {2, 6, 1},      0,  100,    250,    0,              0,      0,      0,              {BOLT_DISTANCE_ATTACK}, (MONST_TURRET), (0)},
    
    id = MK_ACID_MOUND;
    catalog[id] = (creatureType) {0, "acid mound",   'a',    &acidBackColor, 15,     10,     70,     {1, 3, 1},      5,  100,    100,    DF_ACID_BLOOD,  0,      0,      0,              {0},    (MONST_DEFEND_DEGRADE_WEAPON), (MA_HIT_DEGRADE_ARMOR)},
    
    id = MK_CENTIPEDE;
    catalog[id] = (creatureType) {0, "centipede",    'c',    &centipedeColor,20,     20,     80,     {4, 12, 1},     20, 100,    100,    DF_GREEN_BLOOD, 0,      0,      0,              {0},    (0), (MA_CAUSES_WEAKNESS)},
    
    id = MK_OGRE;
    catalog[id] = (creatureType) {0, "ogre",         'O',    &ogreColor,     55,     60,     125,    {9, 13, 2},     20, 100,    200,    DF_RED_BLOOD,   0,      0,      0,              {0},    (MONST_MALE | MONST_FEMALE), (MA_AVOID_CORRIDORS)},
    
    id = MK_BOG_MONSTER;
    catalog[id] = (creatureType) {0, "bog monster",  'B',    &krakenColor,   55,     60,     5000,   {3, 4, 1},      3,  200,    100,    0,              0,      0,      0,              {0},    (MONST_RESTRICTED_TO_LIQUID | MONST_SUBMERGES | MONST_FLITS | MONST_FLEES_NEAR_DEATH), (MA_SEIZES)},
    
    id = MK_OGRE_TOTEM;
    catalog[id] = (creatureType) {0, "ogre totem",   TOTEM_CHAR, &green,     70,     0,      0,      {0, 0, 0},      0,  100,    400,    DF_RUBBLE_BLOOD,LICH_LIGHT,0,   0,              {BOLT_HEALING, BOLT_SLOW_2}, (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS), (0)},
    
    id = MK_SPIDER;
    catalog[id] = (creatureType) {0, "spider",       's',    &white,         20,     70,     90,     {3, 4, 2},      20, 100,    200,    DF_GREEN_BLOOD, 0,      0,      0,              {BOLT_SPIDERWEB}, (MONST_IMMUNE_TO_WEBS | MONST_CAST_SPELLS_SLOWLY | MONST_ALWAYS_USE_ABILITY), (MA_POISONS)},
    
    id = MK_SPARK_TURRET;
    catalog[id] = (creatureType) {0, "spark turret", TURRET_CHAR, &lightningColor,80,0,      100,    {0, 0, 0},      0,  100,    150,    0,              SPARK_TURRET_LIGHT, 0,  0,      {BOLT_SPARK}, (MONST_TURRET), (0)},
    
    id = MK_WILL_O_THE_WISP;
    catalog[id] = (creatureType) {0, "will-o-the-wisp",'w',  &wispLightColor,10,     90,     100,    {5, 8, 2},      5,  100,    100,    DF_ASH_BLOOD,   WISP_LIGHT, 0,  0,              {0},    (MONST_IMMUNE_TO_FIRE | MONST_FLIES | MONST_FLITS | MONST_NEVER_SLEEPS | MONST_FIERY | MONST_DIES_IF_NEGATED)},
    
    id = MK_WRAITH;
    catalog[id] = (creatureType) {0, "wraith",       'W',    &wraithColor,   50,     60,     120,    {6, 13, 2},     5,  50,     100,    DF_GREEN_BLOOD, 0,      0,      0,              {0},    (MONST_FLEES_NEAR_DEATH)},
    
    id = MK_ZOMBIE;
    catalog[id] = (creatureType) {0, "zombie",       'Z',    &vomitColor,    80,     0,      120,    {7, 12, 1},     0,  100,    100,    DF_ROT_GAS_BLOOD,0,     100,    DF_ROT_GAS_PUFF,{0}},
    
    id = MK_TROLL;
    catalog[id] = (creatureType) {0, "troll",        'T',    &trollColor,    65,     70,     125,    {10, 15, 3},    1,  100,    100,    DF_RED_BLOOD,   0,      0,      0,              {0},    (MONST_MALE | MONST_FEMALE)},
    
    id = MK_OGRE_SHAMAN;
    catalog[id] = (creatureType) {0, "ogre shaman",  'O',    &green,         45,     40,     100,    {5, 9, 1},      20, 100,    200,    DF_RED_BLOOD,   0,      0,      0,              {BOLT_HASTE, BOLT_SPARK}, (MONST_MAINTAINS_DISTANCE | MONST_CAST_SPELLS_SLOWLY | MONST_MALE | MONST_FEMALE), (MA_CAST_SUMMON | MA_AVOID_CORRIDORS)},
    
    id = MK_NAGA;
    catalog[id] = (creatureType) {0, "naga",         'N',    &trollColor,    75,     70,     150,    {7, 11, 4},     10, 100,    100,    DF_GREEN_BLOOD, 0,      100,    DF_PUDDLE,      {0},    (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_NEVER_SLEEPS | MONST_FEMALE), (MA_ATTACKS_ALL_ADJACENT)},
    
    id = MK_SALAMANDER;
    catalog[id] = (creatureType) {0, "salamander",   'S',    &salamanderColor,60,    70,     150,    {5, 11, 3},     10, 100,    100,    DF_ASH_BLOOD,   SALAMANDER_LIGHT, 100, DF_SALAMANDER_FLAME, {0}, (MONST_IMMUNE_TO_FIRE | MONST_SUBMERGES | MONST_NEVER_SLEEPS | MONST_FIERY | MONST_MALE), (MA_ATTACKS_EXTEND)},
    
    id = MK_BLOAT;
    catalog[id] = (creatureType) {0, "explosive bloat",'b',  &orange,        10,     0,      100,    {0, 0, 0},      5,  100,    100,    DF_RED_BLOOD,   EXPLOSIVE_BLOAT_LIGHT,0, DF_BLOAT_EXPLOSION, {0}, (MONST_FLIES | MONST_FLITS), (MA_KAMIKAZE | MA_DF_ON_DEATH)},
    
    id = MK_DAR_BLADEMASTER;
    catalog[id] = (creatureType) {0, "dar blademaster",'d',  &purple,        35,     70,     160,    {5, 9, 2},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,              {BOLT_BLINKING}, (MONST_CARRY_ITEM_25 | MONST_MALE | MONST_FEMALE), (MA_AVOID_CORRIDORS)},
    
    id = MK_DAR_PRIESTESS;
    catalog[id] = (creatureType) {0, "dar priestess", 'd',   &darPriestessColor,20,  60,     100,    {2, 5, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,              {BOLT_NEGATION, BOLT_HEALING, BOLT_HASTE, BOLT_SPARK}, (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_FEMALE), (MA_AVOID_CORRIDORS)},
    
    id = MK_DAR_BATTLEMAGE;
    catalog[id] = (creatureType) {0, "dar battlemage",'d',   &darMageColor,  20,     60,     100,    {1, 3, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,              {BOLT_FIRE, BOLT_SLOW_2, BOLT_DISCORD}, (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_MALE | MONST_FEMALE), (MA_AVOID_CORRIDORS)},
    
    id = MK_ACID_JELLY;
    catalog[id] = (creatureType) {0, "acidic jelly", 'J',    &acidBackColor, 60,     0,      115,    {2, 6, 1},      0,  100,    100,    DF_ACID_BLOOD,  0,      0,      0,              {0},    (MONST_DEFEND_DEGRADE_WEAPON), (MA_HIT_DEGRADE_ARMOR | MA_CLONE_SELF_ON_DEFEND)},
    
    id = MK_CENTAUR;
    catalog[id] = (creatureType) {0, "centaur",      'C',    &tanColor,      35,     50,     175,    {4, 8, 2},      20, 50,     100,    DF_RED_BLOOD,   0,      0,      0,              {BOLT_DISTANCE_ATTACK}, (MONST_MAINTAINS_DISTANCE | MONST_MALE), (0)},
    
    id = MK_UNDERWORM;
    catalog[id] = (creatureType) {0, "underworm",    'U',    &wormColor,     80,     40,     160,    {18, 22, 2},    3,  150,    200,    DF_WORM_BLOOD,  0,      0,      0,              {0},    (MONST_NEVER_SLEEPS)},
    
    id = MK_SENTINEL;
    catalog[id] = (creatureType) {0, "sentinel",     STATUE_CHAR, &sentinelColor, 50,0,      0,      {0, 0, 0},      0,  100,    175,    DF_RUBBLE_BLOOD,SENTINEL_LIGHT,0,0,             {BOLT_HEALING, BOLT_SPARK}, (MONST_TURRET | MONST_CAST_SPELLS_SLOWLY | MONST_DIES_IF_NEGATED), (0)},
    
    id = MK_ACID_TURRET;
    catalog[id] = (creatureType) {0, "acid turret", TURRET_CHAR, &acidBackColor,35,  0,      250,    {1, 2, 1},      0,  100,    250,    0,              0,      0,      0,              {BOLT_ACID_TURRET_ATTACK}, (MONST_TURRET), (MA_HIT_DEGRADE_ARMOR)},
    
    id = MK_DART_TURRET;
    catalog[id] = (creatureType) {0, "dart turret", TURRET_CHAR, &centipedeColor,20, 0,      140,    {1, 2, 1},      0,  100,    250,    0,              0,      0,      0,              {BOLT_POISON_DART}, (MONST_TURRET), (MA_CAUSES_WEAKNESS)},
    
    id = MK_KRAKEN;
    catalog[id] = (creatureType) {0, "kraken",       'K',    &krakenColor,   120,    0,      150,    {15, 20, 3},    1,  50,     100,    0,              0,      0,      0,              {0},    (MONST_RESTRICTED_TO_LIQUID | MONST_IMMUNE_TO_WATER | MONST_SUBMERGES | MONST_FLITS | MONST_NEVER_SLEEPS | MONST_FLEES_NEAR_DEATH), (MA_SEIZES)},
    
    id = MK_LICH;
    catalog[id] = (creatureType) {0, "lich",         'L',    &white,         35,     80,     175,    {2, 6, 1},      0,  100,    100,    DF_ASH_BLOOD,   LICH_LIGHT, 0,  0,              {BOLT_FIRE}, (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25 | MONST_NO_POLYMORPH), (MA_CAST_SUMMON)},
    
    id = MK_PHYLACTERY;
    catalog[id] = (creatureType) {0, "phylactery",   GEM_CHAR,&lichLightColor,30,    0,      0,      {0, 0, 0},      0,  100,    150,    DF_RUBBLE_BLOOD,LICH_LIGHT, 0,  0,              {0},    (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED), (MA_CAST_SUMMON | MA_ENTER_SUMMONS)},
    
    id = MK_PIXIE;
    catalog[id] = (creatureType) {0, "pixie",        'p',    &pixieColor,    10,     90,     100,    {1, 3, 1},      20, 50,     100,    DF_GREEN_BLOOD, PIXIE_LIGHT, 0, 0,              {BOLT_NEGATION, BOLT_SLOW_2, BOLT_DISCORD, BOLT_SPARK}, (MONST_MAINTAINS_DISTANCE | MONST_FLIES | MONST_FLITS | MONST_MALE | MONST_FEMALE), (0)},
    
    id = MK_PHANTOM;
    catalog[id] = (creatureType) {0, "phantom",      'P',    &ectoplasmColor,35,     70,     160,    {12, 18, 4},    0,  50,     200,    DF_ECTOPLASM_BLOOD, 0,  2,      DF_ECTOPLASM_DROPLET, {0}, (MONST_INVISIBLE | MONST_FLITS | MONST_FLIES | MONST_IMMUNE_TO_WEBS)},
    
    id = MK_FLAME_TURRET;
    catalog[id] = (creatureType) {0, "flame turret", TURRET_CHAR, &lavaForeColor,40, 0,      150,    {1, 2, 1},      0,  100,    250,    0,              LAVA_LIGHT, 0,  0,              {BOLT_FIRE}, (MONST_TURRET), (0)},
    
    id = MK_IMP;
    catalog[id] = (creatureType) {0, "imp",          'i',    &pink,          35,     90,     225,    {4, 9, 2},      10, 100,    100,    DF_GREEN_BLOOD, IMP_LIGHT,  0,  0,              {BOLT_BLINKING}, (0), (MA_HIT_STEAL_FLEE)},
    
    id = MK_FURY;
    catalog[id] = (creatureType) {0, "fury",         'f',    &darkRed,       19,     90,     200,    {6, 11, 4},     20, 50,     100,    DF_RED_BLOOD,   0,      0,      0,              {0},    (MONST_NEVER_SLEEPS | MONST_FLIES)},
    
    id = MK_REVENANT;
    catalog[id] = (creatureType) {0, "revenant",     'R',    &ectoplasmColor,30,     0,      200,    {15, 20, 5},    0,  100,    100,    DF_ECTOPLASM_BLOOD, 0,  0,      0,              {0},    (MONST_IMMUNE_TO_WEAPONS)},
    
    id = MK_TENTACLE_HORROR;
    catalog[id] = (creatureType) {0, "tentacle horror",'H',  &centipedeColor,120,    95,     225,    {25, 35, 3},    1,  100,    100,    DF_PURPLE_BLOOD,0,      0,      0,              {0}},
    
    id = MK_GOLEM;
    catalog[id] = (creatureType) {0, "golem",        'G',    &gray,          400,    70,     225,    {4, 8, 1},      0,  100,    100,    DF_RUBBLE_BLOOD,0,      0,      0,              {0},    (MONST_REFLECT_4 | MONST_DIES_IF_NEGATED)},
    
    id = MK_DRAGON;
    catalog[id] = (creatureType) {0, "dragon",       'D',    &dragonColor,   150,    90,     250,    {25, 50, 4},    20, 50,     200,    DF_GREEN_BLOOD, 0,      0,      0,              {BOLT_DRAGONFIRE}, (MONST_IMMUNE_TO_FIRE | MONST_CARRY_ITEM_100), (MA_ATTACKS_ALL_ADJACENT)},
        
    // bosses
    catalog[MK_GOBLIN_CHIEFTAN] = (creatureType){0, "goblin warlord",'g',   &blue,          30,     17,     100,    {3, 6, 1},      20, 100,    100,    DF_RED_BLOOD,   0,      0,      0,              {0},    (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25), (MA_CAST_SUMMON | MA_ATTACKS_PENETRATE | MA_AVOID_CORRIDORS)},
    catalog[MK_BLACK_JELLY] =   (creatureType)  {0, "black jelly",  'J',    &black,         120,    0,      130,    {3, 8, 1},      0,  100,    100,    DF_PURPLE_BLOOD,0,      0,      0,              {0},    (0), (MA_CLONE_SELF_ON_DEFEND)},
    catalog[MK_VAMPIRE] =       (creatureType)  {0, "vampire",      'V',    &white,         75,     60,     120,    {4, 15, 2},     6,  50,     100,    DF_RED_BLOOD,   0,      0,      DF_BLOOD_EXPLOSION, {BOLT_BLINKING, BOLT_DISCORD}, (MONST_FLEES_NEAR_DEATH | MONST_MALE), (MA_TRANSFERENCE | MA_DF_ON_DEATH | MA_CAST_SUMMON | MA_ENTER_SUMMONS)},
    catalog[MK_FLAMEDANCER] =   (creatureType)  {0, "flamedancer",  'F',    &white,         65,     80,     120,    {3, 8, 2},      0,  100,    100,    DF_EMBER_BLOOD, FLAMEDANCER_LIGHT,100,DF_FLAMEDANCER_CORONA, {BOLT_FIRE}, (MONST_MAINTAINS_DISTANCE | MONST_IMMUNE_TO_FIRE | MONST_FIERY), (0)},
        
    // special effect monsters
    catalog[MK_SPECTRAL_BLADE] = (creatureType) {0, "spectral blade",WEAPON_CHAR, &spectralBladeColor,1, 0, 150,    {1, 1, 1},      0,  50,     100,    0,              SPECTRAL_BLADE_LIGHT,0,0,       {0},    (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS | MB_DOES_NOT_TRACK_LEADER | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS | MONST_NOT_LISTED_IN_SIDEBAR)},
    catalog[MK_SPECTRAL_IMAGE] = (creatureType) {0, "spectral sword",WEAPON_CHAR, &spectralImageColor, 1,0, 150,    {1, 1, 1},      0,  50,     100,    0,              SPECTRAL_IMAGE_LIGHT,0,0,       {0},    (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_FLIES | MONST_WILL_NOT_USE_STAIRS | MB_DOES_NOT_TRACK_LEADER | MONST_DIES_IF_NEGATED | MONST_IMMUNE_TO_WEBS)},
    catalog[MK_GUARDIAN] =      (creatureType)  {0, "stone guardian",STATUE_CHAR, &white,   1000,   0,      200,    {12, 17, 2},    0,  100,    100,    DF_RUBBLE,      0,      100,      DF_GUARDIAN_STEP, {0},(MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY | MONST_GETS_TURN_ON_ACTIVATION)},
    catalog[MK_WINGED_GUARDIAN] = (creatureType){0, "winged guardian",STATUE_CHAR, &blue,   1000,   0,      200,    {12, 17, 2},    0,  100,    100,    DF_RUBBLE,      0,      100,      DF_SILENT_GLYPH_GLOW, {BOLT_BLINKING}, (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_WILL_NOT_USE_STAIRS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY), (0)},
    catalog[MK_CHARM_GUARDIAN] = (creatureType) {0, "guardian spirit",STATUE_CHAR, &spectralImageColor,1000,0,200,  {5, 12, 2},     0,  100,    100,    0,              SPECTRAL_IMAGE_LIGHT,100,0,     {0},    (MONST_INANIMATE | MONST_NEVER_SLEEPS | MONST_IMMUNE_TO_FIRE | MONST_IMMUNE_TO_WEAPONS | MONST_DIES_IF_NEGATED | MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY)},
    catalog[MK_WARDEN_OF_YENDOR] = (creatureType){0, "Warden of Yendor",'Y', &yendorLightColor,1000,   0,    300,    {12, 17, 2},    0,  200,    200,    DF_RUBBLE,      YENDOR_LIGHT, 100, 0,           {0},    (MONST_NEVER_SLEEPS | MONST_ALWAYS_HUNTING | MONST_INVULNERABLE | MONST_NO_POLYMORPH)},
    catalog[MK_ELDRITCH_TOTEM] = (creatureType) {0, "eldritch totem",TOTEM_CHAR, &glyphColor,80,    0,      0,      {0, 0, 0},      0,  100,    100,    DF_RUBBLE_BLOOD,0,      0,      0,              {0},    (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY), (MA_CAST_SUMMON)},
    catalog[MK_MIRRORED_TOTEM] = (creatureType) {0, "mirrored totem",TOTEM_CHAR, &beckonColor,80,   0,      0,      {0, 0, 0},      0,  100,    100,    DF_RUBBLE_BLOOD,0,      100,    DF_MIRROR_TOTEM_STEP, {BOLT_BECKONING}, (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_ALWAYS_HUNTING | MONST_WILL_NOT_USE_STAIRS | MONST_GETS_TURN_ON_ACTIVATION | MONST_ALWAYS_USE_ABILITY | MONST_REFLECT_4 | MONST_IMMUNE_TO_WEAPONS | MONST_IMMUNE_TO_FIRE), (0)},
        
    // legendary allies
    catalog[MK_UNICORN] =       (creatureType)  {0, "unicorn",      UNICORN_CHAR, &white,   40,     60,     175,    {2, 10, 2},     20, 50,     100,    DF_RED_BLOOD,   UNICORN_LIGHT,1,DF_UNICORN_POOP, {BOLT_HEALING, BOLT_SHIELDING}, (MONST_MAINTAINS_DISTANCE | MONST_MALE | MONST_FEMALE), (0)},
    catalog[MK_IFRIT] =         (creatureType)  {0, "ifrit",        'I',    &ifritColor,    40,     75,     175,    {5, 13, 2},     1,  50,     100,    DF_ASH_BLOOD,   IFRIT_LIGHT,0,  0,              {BOLT_DISCORD}, (MONST_IMMUNE_TO_FIRE | MONST_FLIES | MONST_MALE), (0)},
    catalog[MK_PHOENIX] =       (creatureType)  {0, "phoenix",      'P',    &phoenixColor,  30,     70,     175,    {4, 10, 2},     0,  50,     100,    DF_ASH_BLOOD,   PHOENIX_LIGHT,0,0,              {0},    (MONST_IMMUNE_TO_FIRE| MONST_FLIES)},
    catalog[MK_PHOENIX_EGG] =   (creatureType)  {0, "phoenix egg",  GEM_CHAR,&phoenixColor, 150,    0,      0,      {0, 0, 0},      0,  100,    150,    DF_ASH_BLOOD,   PHOENIX_EGG_LIGHT,  0,  0,      {0},    (MONST_IMMUNE_TO_FIRE| MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE | MONST_WILL_NOT_USE_STAIRS | MONST_NO_POLYMORPH | MONST_ALWAYS_HUNTING), (MA_CAST_SUMMON | MA_ENTER_SUMMONS)},
    catalog[MK_ANCIENT_SPIRIT] = (creatureType) {0, "mangrove dryad",'M',   &tanColor,      70,     60,     175,    {2, 8, 2},      6,  100,    100,    DF_ASH_BLOOD,   0,      0,      0,              {BOLT_ANCIENT_SPIRIT_VINES}, (MONST_IMMUNE_TO_WEBS | MONST_ALWAYS_USE_ABILITY | MONST_MAINTAINS_DISTANCE | MONST_MALE | MONST_FEMALE), (0)},
    
    catalogInitialized = true;
    return catalog;
}

monsterWords *getMonsterText() {
    if (storedWords != NULL) {
        return storedWords;
    }
    
    storedWords = (monsterWords[]) {
        {"A naked adventurer in an unforgiving place, bereft of equipment and confused about the circumstances.",
            "studying", "Studying",
            {"hit", {0}}},
        {"The rat is a scavenger of the shallows, perpetually in search of decaying animal matter.",
            "gnawing at", "Eating",
            {"scratches", "bites", {0}}},
        {"The kobold is a lizardlike humanoid of the upper dungeon.",
            "poking at", "Examining",
            {"clubs", "bashes", {0}}},
        {"The jackal prowls the caverns for intruders to rend with $HISHER powerful jaws.",
            "tearing at", "Eating",
            {"claws", "bites", "mauls", {0}}},
        {"The eel slips silently through the subterranean lake, waiting for unsuspecting prey to set foot in $HISHER dark waters.",
            "eating", "Eating",
            {"shocks", "bites", {0}}},
        {"Mischievous trickster that $HESHE is, the monkey lives to steal shiny trinkets from passing adventurers.",
            "examining", "Examining",
            {"tweaks", "bites", "punches", {0}}},
        {"A bladder of deadly gas buoys the bloat through the air, $HISHER thin veinous membrane ready to rupture at the slightest stress.",
            "gazing at", "Gazing",
            {"bumps", {0}},
            "bursts, leaving behind an expanding cloud of caustic gas!"},
        {"This rare subspecies of bloat is filled with a peculiar vapor that, if released, will cause the floor to vanish out from underneath $HIMHER.",
            "gazing at", "Gazing",
            {"bumps", {0}},
            "bursts, causing the floor underneath $HIMHER to disappear!"},
        {"A filthy little primate, the tribalistic goblin often travels in packs and carries a makeshift stone spear.",
            "chanting over", "Chanting",
            {"cuts", "stabs", "skewers", {0}}},
        {"This goblin is covered with glowing sigils that pulse with power. $HESHE can call into existence phantom blades to attack $HISHER foes.",
            "performing a ritual on", "Performing ritual",
            {"thumps", "whacks", "wallops", {0}},
            {0},
            "gestures ominously!"},
        {"This goblin carries no weapon, and $HISHER eyes sparkle with golden light. $HESHE can invoke a powerful shielding magic to protect $HISHER escorts from harm.",
            "performing a ritual on", "Performing ritual",
            {"slaps", "punches", "kicks", {0}}},
        {"Goblins have created this makeshift totem and imbued $HIMHER with a shamanistic power.",
            "gazing at", "Gazing",
            {"hits", {0}}},
        {"This mass of caustic pink goo slips across the ground in search of a warm meal.",
            "absorbing", "Feeding",
            {"smears", "slimes", "drenches"}},
        {"The enormous, warty toad secretes a powerful hallucinogenic slime to befuddle the senses of any creatures that come in contact with $HIMHER.",
            "eating", "Eating",
            {"slimes", "slams", {0}}},
        {"Often hunting in packs, leathery wings and keen senses guide the vampire bat unerringly to $HISHER prey.",
            "draining", "Feeding",
            {"nips", "bites", {0}}},
        {"A mechanical contraption embedded in the wall, the spring-loaded arrow turret will fire volley after volley of arrows at intruders.",
            "gazing at", "Gazing",
            {"shoots", {0}}},
        {"The acid mound squelches softly across the ground, leaving a trail of acidic goo in $HISHER path.",
            "liquefying", "Feeding",
            {"slimes", "douses", "drenches", {0}}},
        {"This monstrous centipede's incisors are imbued with a horrible venom that will slowly kill $HISHER prey.",
            "eating", "Eating",
            {"pricks", "stings", {0}}},
        {"This lumbering creature carries an enormous club that $HESHE can swing with incredible force.",
            "examining", "Studying",
            {"cudgels", "clubs", "batters", {0}}},
        {"The horrifying bog monster dwells beneath the surface of mud-filled swamps. When $HISHER prey ventures into the mud, the bog monster will ensnare the unsuspecting victim in $HISHER pale tentacles and squeeze its life away.",
            "draining", "Feeding",
            {"squeezes", "strangles", "crushes", {0}}},
        {"Ancient ogres versed in the eldritch arts have assembled this totem and imbued $HIMHER with occult power.",
            "gazing at", "Gazing",
            {"hits", {0}}},
        {"The spider's red eyes pierce the darkness in search of enemies to ensnare with $HISHER projectile webs and dissolve with deadly poison.",
            "draining", "Feeding",
            {"bites", "stings", {0}}},
        {"This contraption hums with electrical charge that $HISHER embedded crystals and magical sigils can direct at intruders in deadly arcs.",
            "gazing at", "Gazing",
            {"shocks", {0}}},
        {"An ethereal blue flame dances through the air, flickering and pulsing in time to an otherworldly rhythm.",
            "consuming", "Feeding",
            {"scorches", "burns", {0}}},
        {"The wraith's hollow eye sockets stare hungrily at the world from $HISHER emaciated frame, and $HISHER long, bloodstained nails grope ceaselessly at the air for a fresh victim.",
            "devouring", "Feeding",
            {"clutches", "claws", "bites", {0}}},
        {"The zombie is the accursed product of a long-forgotten ritual. Perpetually decaying flesh, hanging from $HISHER bones in shreds, releases a putrid and flammable stench that will induce violent nausea with one whiff.",
            "rending", "Eating",
            {"hits", "bites", {0}}},
        {"An enormous, disfigured creature covered in phlegm and warts, the troll regenerates very quickly and attacks with astonishing strength. Many adventures have ended at $HISHER misshapen hands.",
            "eating", "Eating",
            {"cudgels", "clubs", "bludgeons", "pummels", "batters"}},
        {"This ogre is bent with age, but what $HESHE has lost in physical strength, $HESHE has more than gained in occult power.",
            "performing a ritual on", "Performing ritual",
            {"cudgels", "clubs", {0}},
            {0},
            "chants in a harsh, guttural tongue!"},
        {"The serpentine naga live beneath the subterranean waters and emerge to attack unsuspecting adventurers.",
            "studying", "Studying",
            {"claws", "bites", "tail-whips", {0}}},
        {"A serpent wreathed in flames and carrying a burning lash, salamanders dwell in lakes of fire and emerge when they sense a nearby victim, leaving behind a trail of glowing embers.",
            "studying", "Studying",
            {"whips", "lashes", {0}}},
        {"This rare subspecies of bloat is little more than a thin membrane surrounding a bladder of highly explosive gases. The slightest stress will cause $HIMHER to rupture in spectacular and deadly fashion.",
            "gazing at", "Gazing",
            {"bumps", {0}},
            "detonates with terrifying force!"},
        {"An elf of the deep, the dar blademaster leaps toward $HISHER enemies with frightening speed to engage in deadly swordplay.",
            "studying", "Studying",
            {"grazes", "cuts", "slices", "slashes", "stabs"}},
        {"The dar priestess carries a host of religious relics that jangle as $HESHE walks.",
            "praying over", "Praying",
            {"cuts", "slices", {0}}},
        {"The dar battlemage's eyes glow like embers and $HISHER hands radiate an occult heat.",
            "transmuting", "Transmuting",
            {"cuts", {0}}},
        {"A jelly subsisting on a diet of acid mounds will eventually express the characteristics of $HISHER prey, corroding any weapons or armor that come in contact with $HIMHER.",
            "transmuting", "Transmuting",
            {"burns", {0}}},
        {"Half man and half horse, the centaur is an expert with the bow and arrow -- hunter and steed fused into a single creature.",
            "studying", "Studying",
            {"shoots", {0}}},
        {"A strange and horrifying creature of the earth's deepest places, larger than an ogre but capable of squeezing through tiny openings. When hungry, the underworm will burrow behind the walls of a cavern and lurk dormant and motionless -- often for months -- until $HESHE can feel the telltale vibrations of nearby prey.",
            "consuming", "Consuming",
            {"slams", "bites", "tail-whips", {0}}},
        {"An ancient statue of an unrecognizable humanoid figure, the sentinel holds aloft a crystal that gleams with ancient warding magic. Sentinels are always found in groups, and each will attempt to repair any damage done to the others.",
            "focusing on", "Focusing",
            {"hits", {0}}},
        {"A green-flecked nozzle is embedded in the wall, ready to spew a stream of corrosive acid at intruders.",
            "gazing at", "Gazing",
            {"douses", "drenches", {0}}},
        {"This spring-loaded contraption fires darts that are imbued with a strength-sapping poison.",
            "gazing at", "Gazing",
            {"pricks", {0}}},
        {"This tentacled nightmare will emerge from the subterranean waters to ensnare and devour any creature foolish enough to set foot into $HISHER lake.",
            "devouring", "Feeding",
            {"slaps", "smites", "batters", {0}}},
        {"The desiccated form of an ancient sorcerer, animated by dark arts and lust for power, commands the obedience of the infernal planes. $HISHER essence is anchored to reality by a phylactery that is always in $HISHER possession, and the lich cannot die unless $HISHER phylactery is destroyed.",
            "enchanting", "Enchanting",
            {"touches", {0}},
            {0},
            "rasps a terrifying incantation!"},
        {"This gem was the fulcrum of a dark rite, performed centuries ago, that bound the soul of an ancient and terrible sorcerer. Hurry and destroy the gem, before the lich can gather its power and regenerate its corporeal form!",
            "enchanting", "Enchanting",
            {"touches", {0}},
            {0},
            "swirls with dark sorcery as the lich regenerates its form!"},
        {"A tiny humanoid sparkles in the gloom, the hum of $HISHER beating wings punctuated by intermittent peals of high-pitched laughter. What $HESHE lacks in physical endurance, $HESHE makes up for with $HISHER wealth of mischievous magical abilities.",
            "sprinkling dust on", "Dusting",
            {"pokes", {0}}},
        {"A silhouette of mournful rage against an empty backdrop, the phantom slips through the dungeon invisibly in clear air, leaving behind glowing droplets of ectoplasm and the cries of $HISHER unsuspecting victims.",
            "permeating", "Permeating",
            {"hits", {0}}},
        {"This infernal contraption spits blasts of flame at intruders.",
            "incinerating", "Incinerating",
            {"pricks", {0}}},
        {"This trickster demon moves with astonishing speed and delights in stealing from $HISHER enemies and blinking away.",
            "dissecting", "Dissecting",
            {"slices", "cuts", {0}}},
        {"A creature of inchoate rage made flesh, the fury's moist wings beat loudly in the darkness.",
            "flagellating", "Flagellating",
            {"drubs", "fustigates", "castigates", {0}}},
        {"This unholy specter stalks the deep places of the earth without fear, impervious to conventional attacks.",
            "desecrating", "Desecrating",
            {"hits", {0}}},
        {"This seething, towering nightmare of fleshy tentacles slinks through the bowels of the world. The tentacle horror's incredible strength and regeneration make $HIMHER one of the most fearsome creatures of the dungeon.",
            "sucking on", "Consuming",
            {"slaps", "batters", "crushes", {0}}},
        {"A statue animated by an ancient and tireless magic, the golem does not regenerate and attacks with only moderate strength, but $HISHER stone form can withstand incredible damage before collapsing into rubble.",
            "cradling", "Cradling",
            {"backhands", "punches", "kicks", {0}}},
        {"An ancient serpent of the world's deepest places, the dragon's immense form belies its lightning-quick speed and testifies to $HISHER breathtaking strength. An undying furnace of white-hot flames burns within $HISHER scaly hide, and few could withstand a single moment under $HISHER infernal lash.",
            "consuming", "Consuming",
            {"claws", "tail-whips", "bites", {0}}},
        
        {"Taller, stronger and smarter than other goblins, the warlord commands the loyalty of $HISHER kind and can summon them into battle.",
            "chanting over", "Chanting",
            {"slashes", "cuts", "stabs", "skewers", {0}},
            {0},
            "lets loose a deafening war cry!"},
        {"This blob of jet-black goo is as rare as $HESHE is deadly. Few creatures of the dungeon can withstand $HISHER caustic assault. Beware.",
            "absorbing", "Feeding",
            {"smears", "slimes", "drenches"}},
        {"This vampire lives a solitary life deep underground, consuming any warm-blooded creature unfortunate to venture near $HISHER lair.",
            "draining", "Drinking",
            {"grazes", "bites", "buries $HISHER fangs in", {0}},
            {0},
            "spreads his cloak and bursts into a cloud of bats!"},
        {"An elemental creature from another plane of existence, the infernal flamedancer burns with such intensity that $HESHE is painful to behold.",
            "immolating", "Consuming",
            {"singes", "burns", "immolates", {0}}},
        
        {"Eldritch forces have coalesced to form this flickering, ethereal weapon.",
            "gazing at", "Gazing",
            {"nicks",  {0}}},
        {"Eldritch energies bound up in your armor have leapt forth to project this spectral image.",
            "gazing at", "Gazing",
            {"hits",  {0}}},
        {"Guarding the room is a weathered stone statue of a knight carrying a battleaxe, connected to the glowing glyphs on the floor by invisible strands of enchantment.",
            "gazing at", "Gazing",
            {"strikes",  {0}}},
        {"A statue of a sword-wielding angel surveys the room, connected to the glowing glyphs on the floor by invisible strands of enchantment.",
            "gazing at", "Gazing",
            {"strikes",  {0}}},
        {"A spectral outline of a knight carrying a battleaxe casts an ethereal light on its surroundings.",
            "gazing at", "Gazing",
            {"strikes",  {0}}},
        {"An immortal presence stalks through the dungeon, implacably hunting that which was taken and the one who took it.",
            "gazing at", "Gazing",
            {"strikes",  {0}}},
        {"This totem sits at the center of a summoning circle that radiates a strange energy.",
            "gazing at", "Gazing",
            {"strikes",  {0}},
            {0},
            "crackles with energy as you touch the glyph!"},
        {"A prism of shoulder-high mirrored surfaces gleams in the darkness.",
            "gazing at", "Gazing",
            {"strikes",  {0}}},
        
        {"The unicorn's flowing mane and tail shine with rainbow light, $HISHER horn glows with healing and protective magic, and $HISHER eyes implore you to always chase your dreams. Unicorns are rumored to be attracted to virgins -- is there a hint of accusation in $HISHER gaze?",
            "consecrating", "Consecrating",
            {"pokes", "stabs", "gores", {0}}},
        {"A whirling desert storm given human shape, the ifrit's twin scimitars flicker in the darkness and $HISHER eyes burn with otherworldly zeal.",
            "absorbing", "Absorbing",
            {"cuts", "slashes", "lacerates", {0}}},
        {"This legendary bird shines with a brilliant light, and $HISHER wings crackle and pop like embers as they beat the air. When $HESHE dies, legend has it that an egg will form and a newborn phoenix will rise from its ashes.",
            "cremating", "Cremating",
            {"pecks", "scratches", "claws", {0}}},
        {"Cradled in a nest of cooling ashes, the translucent membrane of the phoenix egg reveals a yolk that glows ever brighter by the second.",
            "cremating", "Cremating",
            {"touches", {0}},
            {0},
            "bursts as a newborn phoenix rises from the ashes!"},
        {"This mangrove dryad is as old as the earth, and $HISHER gnarled figure houses an ancient power. When angered, $HESHE can call upon the forces of nature to bind $HISHER foes and tear them to shreds.",
            "absorbing", "Absorbing",
            {"whips", "lashes", "thrashes", "lacerates", {0}}},
    };
    
    return storedWords;
}
