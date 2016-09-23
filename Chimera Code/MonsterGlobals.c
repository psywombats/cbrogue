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
    
    catalogInitialized = true;
    return catalog;
}
