//
//  Blueprints.c
//  cBrogue
//
//  Created by Aaron King on 2/16/17.
//
//

#include "Blueprints.h"
#include "MonsterGlobals.h"

blueprint *blueprintCatalog;

void ensureBlueprintsInitialized();
void initBlueprintCatalog();

blueprint *getBlueprintCatalog() {
    ensureBlueprintsInitialized();
    return blueprintCatalog;
}

void ensureBlueprintsInitialized() {
    static bool blueprintsInitialized = false;
    if (blueprintsInitialized) {
        return;
    }
    
    initBlueprintCatalog();
    
    blueprintsInitialized = true;
}

void initBlueprintCatalog() {
    blueprintCatalog = (blueprint *)malloc(sizeof(blueprint) * NUMBER_BLUEPRINTS);
    memset(blueprintCatalog, 0, sizeof(blueprint) * NUMBER_BLUEPRINTS);
    
    blueprint *bc = blueprintCatalog;
    
    unsigned int id;
    short dl, low, high, freq;
    
    dungeonFeatureTypes ndf = DF_NONE;
    tileType ntt = NOTHING;
    dungeonLayers ndl = DUNGEON;
    
    //BLUEPRINTS:
    //                      depths                  roomSize    freq    featureCt   dungeonProfileType  flags   (features on subsequent lines)
    
    //FEATURES:
    //DF        terrain     layer       instanceCtRange minInsts    itemCat     itemKind    monsterKind     reqSpace        hordeFl     itemFlags   featureFlags
    
    // -- REWARD ROOMS --
    
    // Mixed item library -- can check one item out at a time
    id = MT_REWARD_MULTI_LIBRARY;
    bc[id] =    (blueprint){{1, 12},                {30, 50},   30,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {1,1},      1,          WAND,       WAND_EMPOWERMENT, 0,        2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_PLAYER_AVOIDS), (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {3,3},      3,          (WEAPON|ARMOR|WAND),-1, 0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_PLAYER_AVOIDS), (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {2,3},      2,          (STAFF|RING|CHARM),-1,  0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {ndf,   STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Single item category library -- can check one item out at a time
    id = MT_REWARD_MONO_LIBRARY;
    bc[id] =    (blueprint){{1, 12},                {30, 50},   15,     5,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {3,4},      3,          (RING),     -1,         0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_ALTERNATIVE | MF_IMPREGNABLE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {4,5},      4,          (STAFF),    -1,         0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_ALTERNATIVE | MF_IMPREGNABLE)},
        {ndf,   STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Treasure room -- apothecary or archive (potions or scrolls)
    id = MT_REWARD_CONSUMABLES;
    bc[id] =    (blueprint){{8, AMULET_LEVEL},      {20, 40},   20,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   ntt,        ndl,            {5,7},      2,          (POTION),   -1,         0,              2,              0,          0,          (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {ndf,   ntt,        ndl,            {4,6},      2,          (SCROLL),   -1,         0,              2,              0,          0,          (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {ndf,   FUNGUS_FOREST,SURFACE,      {3,4},      0,          0,          -1,         0,              2,              0,          0,          0},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {ndf,   STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Guaranteed good permanent item on a glowing pedestal (runic weapon/armor or 2 staffs)
    id = MT_REWARD_PEDESTALS_PERMANENT;
    bc[id] =    (blueprint){{5, 16},                {10, 30},   30,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   STATUE_INERT,DUNGEON,       {2,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {ndf,   PEDESTAL,   DUNGEON,        {1,1},      1,          (WEAPON),   -1,         0,              2,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING)},
        {ndf,   PEDESTAL,   DUNGEON,        {1,1},      1,          (ARMOR),    -1,         0,              2,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_TREAT_AS_BLOCKING)},
        {ndf,   PEDESTAL,   DUNGEON,        {2,2},      2,          (STAFF),    -1,         0,              2,              0,          (ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN),   (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Guaranteed good consumable item on glowing pedestals (scrolls of enchanting, potion of life)
    id = MT_REWARD_PEDESTALS_CONSUMABLE;
    bc[id] =    (blueprint){{10, AMULET_LEVEL},     {10, 30},   30,     5,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {ndf,   PEDESTAL,   DUNGEON,        {1,1},      1,          (SCROLL),   SCROLL_ENCHANTING, 0,       2,              0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {ndf,   PEDESTAL,   DUNGEON,        {1,1},      1,          (POTION),   POTION_LIFE,0,              2,              0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Commutation altars
    id = MT_REWARD_COMMUTATION_ALTARS;
    bc[id] =    (blueprint){{13, AMULET_LEVEL},     {10, 30},   50,     4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {DF_MAGIC_PIPING,COMMUTATION_ALTAR,DUNGEON,{2,2},   2,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Resurrection altar
    id = MT_REWARD_RESURRECTION_ALTAR;
    bc[id] =    (blueprint){{13, AMULET_LEVEL},     {10, 30},   30,     4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_IMPREGNABLE | BP_REWARD), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)},
        {DF_MACHINE_FLOOR_TRIGGER_REPEATING, RESURRECTION_ALTAR,DUNGEON, {1,1}, 1, 0, -1,       0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Outsourced item -- same item possibilities as in the good permanent item reward room (plus charms), but directly adopted by 1-2 key machines.
    id = MT_REWARD_ADOPTED_ITEM;
    bc[id] =    (blueprint){{5, 17},                {0,0},      20,     4,          0,                  (BP_REWARD | BP_NO_INTERIOR_FLAG),  {
        {ndf,   ntt,        ndl,            {1,1},      1,          (WEAPON),   -1,         0,              0,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_NO_THROWING_WEAPONS | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)},
        {ndf,   ntt,        ndl,            {1,1},      1,          (ARMOR),    -1,         0,              0,              0,          ITEM_IDENTIFIED,(MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_REQUIRE_GOOD_RUNIC | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)},
        {ndf,   ntt,        ndl,            {2,2},      2,          (STAFF),    -1,         0,              0,              0,          ITEM_KIND_AUTO_ID, (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)},
        {ndf,   ntt,        ndl,            {1,2},      1,          (CHARM),    -1,         0,              0,              0,          ITEM_KIND_AUTO_ID, (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Dungeon -- two allies chained up for the taking
    id = MT_REWARD_DUNGEON;
    bc[id] =    (blueprint){{5, AMULET_LEVEL},      {30, 80},   12,     5,          0,                  (BP_ROOM | BP_REWARD),  {
        {ndf,   VOMIT,      SURFACE,        {2,2},      2,          0,          -1,         0,              2,              (HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING)},
        {DF_AMBIENT_BLOOD,MANACLE_T,SURFACE,{1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_AMBIENT_BLOOD,MANACLE_L,SURFACE,{1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_BONES,  ntt,        ndl,        {2,3},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_VOMIT,  ntt,        ndl,        {2,3},      1,          0,          -1,         0,              1,              0,          0,          0},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)}}};
    // Kennel -- allies locked in cages in an open room; choose one or two to unlock and take with you.
    id = MT_REWARD_KENNEL;
    bc[id] =    (blueprint){{5, AMULET_LEVEL},      {30, 80},   15,     4,          0,                  (BP_ROOM | BP_REWARD),  {
        {ndf,   MONSTER_CAGE_CLOSED,DUNGEON,{3,5},      3,          0,          -1,         0,              2,              (HORDE_MACHINE_KENNEL | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {ndf,   ntt,        ndl,            {1,2},      1,          KEY,        KEY_CAGE,   0,              1,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS),(MF_PERMIT_BLOCKING | MF_GENERATE_ITEM | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_SKELETON_KEY | MF_KEY_DISPOSABLE)},
        {DF_AMBIENT_BLOOD, ntt, ndl,        {3,5},      3,          0,          -1,         0,              1,              0,          0,          0},
        {DF_BONES,  ntt,        ndl,        {3,5},      3,          0,          -1,         0,              1,              0,          0,          0},
        {ndf,   TORCH_WALL, DUNGEON,        {2,3},      2,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)}}};
    // Vampire lair -- allies locked in cages and chained in a hidden room with a vampire in a coffin; vampire has one cage key.
    id = MT_REWARD_VAMPIRE_LAIR;
    freq = getVampireBossMonsterId() > 0 ? 15 : 0;
    dl = getMonsterCatalog()[getVampireBossMonsterId()].danger > 0 ? getMonsterCatalog()[getVampireBossMonsterId()].danger : 10;
    low = dl - 5;
    bc[id] =    (blueprint){{low, AMULET_LEVEL},     {50, 80},   freq,   4,          0,                  (BP_ROOM | BP_REWARD | BP_SURROUND_WITH_WALLS | BP_PURGE_INTERIOR), {
        {DF_AMBIENT_BLOOD,ntt,    ndl,      {1,2},      1,          0,          -1,         0,              2,              (HORDE_MACHINE_CAPTIVE | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_AMBIENT_BLOOD,MONSTER_CAGE_CLOSED,DUNGEON,{2,4},2,          0,          -1,         0,              2,              (HORDE_VAMPIRE_FODDER | HORDE_LEADER_CAPTIVE), 0, (MF_GENERATE_HORDE | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE | MF_NOT_IN_HALLWAY)},
        {DF_TRIGGER_AREA,COFFIN_CLOSED,ndl, {1,1},      1,          KEY,        KEY_CAGE,   getVampireBossMonsterId(), 1,   0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS),(MF_GENERATE_ITEM | MF_SKELETON_KEY | MF_MONSTER_TAKE_ITEM | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN | MF_KEY_DISPOSABLE)},
        {DF_AMBIENT_BLOOD,SECRET_DOOR,DUNGEON,{1,1},    1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)}}};
    // Legendary ally -- approach the altar with the crystal key to activate a portal and summon a legendary ally.
    id = MT_REWARD_ASTRAL_PORTAL;
    freq = 0; // disabled for now
    bc[id] =    (blueprint){{8, AMULET_LEVEL},      {30, 50},       0,      2,          0,                  (BP_ROOM | BP_REWARD),  {
        {DF_LUMINESCENT_FUNGUS, ALTAR_KEYHOLE, DUNGEON, {1,1}, 1,       KEY,        KEY_PORTAL, 0,              2,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS),(MF_GENERATE_ITEM | MF_NOT_IN_HALLWAY | MF_NEAR_ORIGIN | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_KEY_DISPOSABLE)},
        {DF_LUMINESCENT_FUNGUS, PORTAL, DUNGEON,{1,1},      1,          0,          -1,         0,              2,              HORDE_MACHINE_LEGENDARY_ALLY,0, (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN)}}};
    // Goblin warren
    id = MT_REWARD_GOBLIN_WARREN;
    freq = getWarrenBossMonsterId() > 0 ? 15 : 0;
    dl = getMonsterCatalog()[getWarrenBossMonsterId()].danger > 0 ? getMonsterCatalog()[getWarrenBossMonsterId()].danger : 10;
    low = dl - 5;
    high = dl + 5;
    bc[id] =    (blueprint){{low, high},            {100, 200}, freq,   9,          DP_GOBLIN_WARREN,   (BP_ROOM | BP_REWARD | BP_MAXIMIZE_INTERIOR | BP_REDESIGN_INTERIOR),    {
        {ndf,   MUD_FLOOR,  DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   MUD_DOORWAY,DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {ndf,   MUD_WALL,   DUNGEON,        {1,1},      100,        0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_EVERYWHERE)},
        {ndf,   PEDESTAL,   DUNGEON,        {1,1},      1,          (SCROLL),   SCROLL_ENCHANTING, getWarrenBossMonsterId(),2,          0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_MONSTER_SLEEPING | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {ndf,   PEDESTAL,   DUNGEON,        {1,1},      1,          (POTION),   POTION_LIFE, getWarrenBossMonsterId(),      2,          0,          (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_MONSTER_SLEEPING | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {ndf,   ntt,        ndl,            {5, 8},     5,          0,          -1,         0,              2,              HORDE_MACHINE_GOBLIN_WARREN,    0,  (MF_GENERATE_HORDE | MF_NOT_IN_HALLWAY | MF_MONSTER_SLEEPING)},
        {ndf,   ntt,        ndl,            {2,3},      2,          (WEAPON|ARMOR), -1,     0,              1,              0,          0,          (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_HAY,ntt,        ndl,            {10, 15},   1,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY)},
        {DF_JUNK,ntt,       ndl,            {7, 12},    1,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY)}}};
    // Sentinel sanctuary
    id = MT_REWARD_SENTINEL_SANCTUARY;
    freq = getSentinelMonsterId() > 0 ? 15 : 0;
    dl = getMonsterCatalog()[getSentinelMonsterId()].danger > 0 ? getMonsterCatalog()[getSentinelMonsterId()].danger : 16;
    low = dl - 7;
    high = dl + 7;
    bc[id] =    (blueprint){{10, 23},               {100, 200}, 15,  10,           DP_SENTINEL_SANCTUARY, (BP_ROOM | BP_REWARD | BP_MAXIMIZE_INTERIOR | BP_REDESIGN_INTERIOR), {
        {ndf,   MARBLE_FLOOR,DUNGEON,       {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   CRYSTAL_WALL,DUNGEON,       {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_BUILD_IN_WALLS | MF_EVERYWHERE)},
        {ndf,   PEDESTAL, DUNGEON, {1,1},   1,          (SCROLL),   SCROLL_ENCHANTING,0,    2,              0,              (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {ndf,   PEDESTAL, DUNGEON, {1,1},   1,          (POTION),   POTION_LIFE,0,          2,              0,              (ITEM_KIND_AUTO_ID),    (MF_GENERATE_ITEM | MF_ALTERNATIVE | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {30, 35},   20,         0,          -1,         0,              1,              0,          0,          (MF_PERMIT_BLOCKING)},
        {ndf,   STATUE_INERT,DUNGEON,       {3, 5},     3,          0,          -1,         getSentinelMonsterId(), 2,      0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   STATUE_INERT,DUNGEON,       {10, 15},   8,          0,          -1,         getSentinelMonsterId(), 2,      0,          0,          MF_BUILD_IN_WALLS},
        {ndf,   ntt,        ndl,            {4, 6},     4,          0,          -1,         getGuardianMonsterId(), 1,      0,          0,          MF_TREAT_AS_BLOCKING},
        {ndf,   ntt,        ndl,            {0,2},      0,          0,          -1,         getWingedGuardianMonsterId(),1, 0,          0,          MF_TREAT_AS_BLOCKING},
        {ndf,   ntt,        ndl,            {2,3},      2,          (SCROLL | POTION), -1,  0,              1,              0,          0,          (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};

    // -- AMULET HOLDER --
    // Statuary -- key on an altar, area full of statues; take key to cause statues to burst and reveal monsters
    id = MT_AMULET_AREA;
    bc[id] =    (blueprint){{10, AMULET_LEVEL},     {35, 40},   0,      4,          0,                  (BP_PURGE_INTERIOR | BP_OPEN_INTERIOR), {
        {DF_LUMINESCENT_FUNGUS, AMULET_SWITCH, DUNGEON, {1,1}, 1,       AMULET,     -1,         0,              2,              0,          0,          (MF_GENERATE_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   FUNGUS_FOREST,SURFACE,      {2,3},      0,          0,          -1,         0,              2,              0,          0,          MF_NOT_IN_HALLWAY},
        {ndf,   STATUE_INSTACRACK,DUNGEON,  {1,1},      1,          0,          -1,         getWardenMonsterId(), 1,        0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_IMPREGNABLE)},
        {ndf,   TORCH_WALL, DUNGEON,        {3,4},      0,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)}}};
    
    // -- VESTIBULES --
    
    // Plain locked door, key guarded by an adoptive room
    id = MT_LOCKED_DOOR_VESTIBULE;
    bc[id] =    (blueprint){{1, AMULET_LEVEL},      {1, 1},     100,        1,      0,                  (BP_VESTIBULE), {
        {ndf,   LOCKED_DOOR, DUNGEON,       {1,1},      1,          KEY,        KEY_DOOR,   0,              1,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS), (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_GENERATE_ITEM | MF_OUTSOURCE_ITEM_TO_MACHINE | MF_KEY_DISPOSABLE | MF_IMPREGNABLE)}}},
    // Plain secret door
    id = MT_SECRET_DOOR_VESTIBULE;
    bc[id] =    (blueprint){{2, AMULET_LEVEL},      {1, 1},     1,      1,          0,                  (BP_VESTIBULE), {
        {ndf,   SECRET_DOOR, DUNGEON,       {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING)}}};
    // Lever and either an exploding wall or a portcullis
    id = MT_SECRET_LEVER_VESTIBULE;
    bc[id] =    (blueprint){{4, AMULET_LEVEL},      {1, 1},     8,      3,          0,                  (BP_VESTIBULE), {
        {ndf,   WORM_TUNNEL_OUTER_WALL,DUNGEON,{1,1},   1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_IMPREGNABLE | MF_ALTERNATIVE)},
        {ndf,   PORTCULLIS_CLOSED,DUNGEON,  {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_IMPREGNABLE | MF_ALTERNATIVE)},
        {ndf,   WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Flammable barricade in the doorway -- burn the wooden barricade to enter
    id = MT_FLAMMABLE_BARRICADE_VESTIBULE;
    bc[id] =    (blueprint){{1, 6},                 {1, 1},     10,     3,          0,                  (BP_VESTIBULE), {
        {ndf,   WOODEN_BARRICADE,DUNGEON,   {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {ndf,   ntt,        ndl,            {1,1},      1,          WEAPON,     INCENDIARY_DART, 0,         1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          POTION,     POTION_INCINERATION, 0,     1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)}}};
    // Statue in the doorway -- use a scroll of shattering to enter
    id = MT_STATUE_SHATTERING_VESTIBULE;
    bc[id] =    (blueprint){{1, AMULET_LEVEL},      {1, 1},     6,      2,          0,                  (BP_VESTIBULE), {
        {ndf,   STATUE_INERT_DOORWAY,DUNGEON,{1,1},1,        0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {ndf,   ntt,        ndl,            {1,1},      1,          SCROLL,     SCROLL_SHATTERING, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY)}}},
    // Statue in the doorway -- bursts to reveal monster
    id = MT_STATUE_MONSTER_VESTIBULE;
    bc[id] =    (blueprint){{5, AMULET_LEVEL}, {2, 2},     6,      2,          0,                  (BP_VESTIBULE), {
        {ndf,   STATUE_DORMANT_DOORWAY,DUNGEON,     {1, 1}, 1,      0,          -1,         0,              1,              HORDE_MACHINE_STATUE,0, (MF_PERMIT_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {ndf,   MACHINE_TRIGGER_FLOOR,DUNGEON,{0,0},    1,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)}}};
    // Throwing tutorial -- toss an item onto the pressure plate to retract the portcullis
    id = MT_THROWING_TUTORIAL_VESTIBULE;
    bc[id] =    (blueprint){{1, 4},         {70, 70},   8,      3,          0,                  (BP_VESTIBULE), {
        {DF_MEDIUM_HOLE, MACHINE_PRESSURE_PLATE, LIQUID, {1,1}, 1,      0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   PORTCULLIS_CLOSED,DUNGEON,  {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_IMPREGNABLE | MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {ndf,   WORM_TUNNEL_OUTER_WALL,DUNGEON,{1,1},   1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_IMPREGNABLE | MF_ALTERNATIVE)}}};
    // Pit traps -- area outside entrance is full of pit traps
    id = MT_PIT_TRAPS_VESTIBULE;
    bc[id] =    (blueprint){{1, AMULET_LEVEL},{30, 60},   8,    3,          0,                  (BP_VESTIBULE | BP_OPEN_INTERIOR | BP_NO_INTERIOR_FLAG),    {
        {ndf,   DOOR,       DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {ndf,   SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_IMPREGNABLE | MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {ndf,   TRAP_DOOR_HIDDEN,DUNGEON,   {60, 60},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)}}};
    // Beckoning obstacle -- a mirrored totem guards the door, and glyph are around the doorway.
    id = MT_BECKONING_OBSTACLE_VESTIBULE;
    freq = getMirrorMonsterId() > 0 ? 8 : 0;
    bc[id] =    (blueprint){{5, AMULET_LEVEL}, {15, 30},        freq,       3,          0,                  (BP_VESTIBULE | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR), {
        {ndf,   DOOR,       DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          -1,         getMirrorMonsterId(), 3,        0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN | MF_FAR_FROM_ORIGIN)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {1,1},      0,          0,          -1,         0,              1,              0,          0,          (MF_NEAR_ORIGIN | MF_EVERYWHERE)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {3,5},      2,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)}}};
    // Guardian obstacle -- a guardian is in the door on a glyph, with other glyphs scattered around.
    freq = getGuardianMonsterId() > 0 ? 8 : 0;
    freq = getWingedGuardianMonsterId() > 0 ? freq : 0;
    id = MT_GUARDIAN_VESTIBULE;
    bc[id] =    (blueprint){{6, AMULET_LEVEL}, {25, 25},        freq,       4,          0,                  (BP_VESTIBULE | BP_OPEN_INTERIOR),  {
        {ndf,   DOOR,       DUNGEON,        {1,1},      1,          0,          0,          getGuardianMonsterId(), 2,      0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_ALTERNATIVE)},
        {ndf,   DOOR,       DUNGEON,        {1,1},      1,          0,          0,          getWingedGuardianMonsterId(), 2,0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_ALTERNATIVE)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {10,10},    3,          0,          -1,         0,              1,              0,          0,          (MF_PERMIT_BLOCKING| MF_NEAR_ORIGIN)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {1,1},      0,          0,          -1,         0,              2,              0,          0,          (MF_EVERYWHERE | MF_PERMIT_BLOCKING | MF_NOT_IN_HALLWAY)}}};

    // -- KEY HOLDERS --
    
    // Nested item library -- can check one item out at a time, and one is a disposable key to another reward room
    id = MT_KEY_REWARD_LIBRARY;
    bc[id] =    (blueprint){{1, AMULET_LEVEL}, {30, 50},   35,     7,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_ADOPT_ITEM | BP_IMPREGNABLE), {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   WALL,       DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE | MF_EVERYWHERE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING | MF_BUILD_VESTIBULE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {1,2},      1,          (WEAPON|ARMOR|WAND),-1, 0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_PLAYER_AVOIDS), (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {1,2},      1,          (STAFF|RING|CHARM),-1,  0,              2,              0,          (ITEM_IS_KEY | ITEM_KIND_AUTO_ID | ITEM_MAX_CHARGES_KNOWN | ITEM_PLAYER_AVOIDS),    (MF_GENERATE_ITEM | MF_NO_THROWING_WEAPONS | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {ndf,   ALTAR_CAGE_OPEN,DUNGEON,    {1,1},      1,          0,          -1,         0,              2,              0,          (ITEM_IS_KEY | ITEM_PLAYER_AVOIDS | ITEM_MAX_CHARGES_KNOWN),    (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_IMPREGNABLE)},
        {ndf,   STATUE_INERT,DUNGEON,       {1,3},      0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_BUILD_IN_WALLS | MF_IMPREGNABLE)}}};
    // Secret room -- key on an altar in a secret room
    id = MT_KEY_SECRET_ROOM;
    bc[id] =    (blueprint){{1, AMULET_LEVEL}, {15, 100},  1,      2,          0,                  (BP_ROOM | BP_ADOPT_ITEM), {
        {ndf,   ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          ITEM_PLAYER_AVOIDS, (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)}}};
    // Throwing tutorial -- toss an item onto the pressure plate to retract the cage and reveal the key
    id = MT_KEY_THROWING_TUTORIAL_AREA;
    bc[id] =    (blueprint){{1, 4},            {70, 80},   8,      2,          0,                  (BP_ADOPT_ITEM), {
        {ndf,   ALTAR_CAGE_RETRACTABLE,DUNGEON,{1,1},   1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_IMPREGNABLE | MF_NOT_IN_HALLWAY)},
        {DF_MEDIUM_HOLE, MACHINE_PRESSURE_PLATE, LIQUID, {1,1}, 1,      0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Rat trap -- getting the key triggers paralysis vents nearby and also causes rats to burst out of the walls
    id = MT_KEY_RAT_TRAP_ROOM;
    freq = getRatTrapMonsterId() > 0 ? 7 : 0;
    bc[id] =    (blueprint){{1,8},             {30, 70},   freq,        3,          0,                  (BP_ADOPT_ITEM | BP_ROOM),  {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   MACHINE_PARALYSIS_VENT_HIDDEN,DUNGEON,{1,1},1,      0,          -1,         0,              2,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)},
        {ndf,   RAT_TRAP_WALL_DORMANT,DUNGEON,{10,20},  5,          0,          -1,         getRatTrapMonsterId(), 1,       0,          0,          (MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_NOT_ON_LEVEL_PERIMETER)}}};
    // Fun with fire -- trigger the fire trap and coax the fire over to the wooden barricade surrounding the altar and key
    id = MT_KEY_FIRE_TRANSPORTATION_ROOM;
    bc[id] =    (blueprint){{3, 10},           {80, 100},  10,     6,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR), {
        {DF_SURROUND_WOODEN_BARRICADE,ALTAR_INERT,DUNGEON,{1,1},1,      0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   GRASS,      SURFACE,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE | MF_ALTERNATIVE)},
        {DF_SWAMP,  ntt,        ndl,        {4,4},      2,          0,          -1,         0,              2,              0,          0,          (MF_ALTERNATIVE | MF_FAR_FROM_ORIGIN)},
        {ndf,   FLAMETHROWER_HIDDEN,DUNGEON,{1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NEAR_ORIGIN)},
        {ndf,   GAS_TRAP_POISON_HIDDEN,DUNGEON,{3, 3},  1,          0,          -1,         0,              5,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_ALTERNATIVE)},
        {ndf,   ntt,        ndl,            {2,2},      1,          POTION,     POTION_LICHEN, 0,           3,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)}}};
    // Flood room -- key on an altar in a room with pools of eel-infested waters; take key to flood room with shallow water
    id = MT_KEY_FLOOD_TRAP_ROOM;
    bc[id] =    (blueprint){{3, AMULET_LEVEL}, {80, 180},  10,     4,          0,                  (BP_ROOM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS | BP_PURGE_PATHING_BLOCKERS | BP_ADOPT_ITEM),  {
        {ndf,   FLOOR_FLOODABLE,LIQUID,     {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              5,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_SPREADABLE_WATER_POOL,NOTHING,DUNGEON,          {2, 4},     1,          0,          -1,         0,              5,              HORDE_MACHINE_WATER_MONSTER,0,MF_GENERATE_HORDE},
        {DF_GRASS,  FOLIAGE,    SURFACE,        {3, 4},     3,          0,          -1,         0,              1,              0,          0,          0}}};
    // Fire trap room -- key on an altar, pools of water, fire traps all over the place.
    id = MT_KEY_FIRE_TRAP_ROOM;
    bc[id] =    (blueprint){{4, AMULET_LEVEL}, {80, 180},  6,      5,          0,                  (BP_ROOM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS | BP_PURGE_PATHING_BLOCKERS | BP_ADOPT_ITEM),  {
        {ndf,   ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   ntt,        ndl,            {1, 1},     1,          0,          -1,         0,              4,              0,          0,          MF_BUILD_AT_ORIGIN},
        {ndf,   FLAMETHROWER_HIDDEN,DUNGEON,{40, 60},   20,         0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {DF_DEEP_WATER_POOL,ntt,  ndl,      {4, 4},     1,          0,          -1,         0,              4,              HORDE_MACHINE_WATER_MONSTER,0,MF_GENERATE_HORDE},
        {DF_GRASS,  FOLIAGE,    SURFACE,        {3, 4},     3,          0,          -1,         0,              1,              0,          0,          0}}};
    // Thief area -- empty altar, monster with item, permanently fleeing.
    id = MT_KEY_THIEF_AREA;
    bc[id] =    (blueprint){{3, AMULET_LEVEL}, {15, 20},   10,     2,          0,                  (BP_ADOPT_ITEM),    {
        {DF_LUMINESCENT_FUNGUS, ALTAR_INERT,DUNGEON,{1,1},  1,          0,          -1,         0,              2,              HORDE_MACHINE_THIEF,0,          (MF_ADOPT_ITEM | MF_BUILD_AT_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_GENERATE_HORDE | MF_MONSTER_TAKE_ITEM | MF_MONSTER_FLEEING)},
        {ndf,   STATUE_INERT,DUNGEON,       {3, 5},     2,          0,          -1,         0,              2,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Collapsing floor area -- key on an altar in an area; take key to cause the floor of the area to collapse
    id = MT_KEY_COLLAPSING_FLOOR_AREA;
    bc[id] =    (blueprint){{1, AMULET_LEVEL}, {45, 65},   13,     3,          0,                  (BP_ADOPT_ITEM | BP_TREAT_AS_BLOCKING), {
        {ndf,   FLOOR_FLOODABLE,DUNGEON,    {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   ALTAR_SWITCH_RETRACTING,DUNGEON,{1,1},  1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {DF_ADD_MACHINE_COLLAPSE_EDGE_DORMANT,ntt,ndl,{3, 3},   2,          0,          -1,         0,              3,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)}}};
    // Pit traps -- key on an altar, room full of pit traps
    id = MT_KEY_PIT_TRAP_ROOM;
    bc[id] =    (blueprint){{1, AMULET_LEVEL}, {30, 100},  10,     3,          0,                  (BP_ROOM | BP_ADOPT_ITEM),  {
        {ndf,   ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   TRAP_DOOR_HIDDEN,DUNGEON,   {30, 40},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {ndf,   SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)}}};
    // Levitation challenge -- key on an altar, room filled with pit, levitation or lever elsewhere on level, bridge appears when you grab the key/lever.
    id = MT_KEY_LEVITATION_ROOM;
    bc[id] =    (blueprint){{1, 13},           {75, 120},  10,     9,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_SURROUND_WITH_WALLS),  {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   TORCH_WALL, DUNGEON,        {1,4},      0,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              3,              0,          0,          MF_BUILD_AT_ORIGIN},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM,LIQUID,{120, 120},1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM_WITH_HIDDEN_BRIDGE,LIQUID,{1,1},1,0,      0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          POTION,     POTION_LEVITATION, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL | MF_ALTERNATIVE)}}};
    // Web climbing -- key on an altar, room filled with pit, spider at altar to shoot webs, bridge appears when you grab the key
    id = MT_KEY_WEB_CLIMBING_ROOM;
    freq = getWebberMonsterId() > 0 ? 10 : 0;
    bc[id] =    (blueprint){{7, AMULET_LEVEL}, {55, 90},   freq,        7,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_SURROUND_WITH_WALLS),  {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         getWebberMonsterId(), 3,        0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_IN_VIEW_OF_ORIGIN)},
        {ndf,   TORCH_WALL, DUNGEON,        {1,4},      0,          0,          0,          0,              1,              0,          0,          (MF_BUILD_IN_WALLS)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              3,              0,          0,          MF_BUILD_AT_ORIGIN},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM,LIQUID,   {120, 120}, 1,      0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_ADD_DORMANT_CHASM_HALO, CHASM_WITH_HIDDEN_BRIDGE,LIQUID,{1,1},1,0,      0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)}}};
    // Lava moat room -- key on an altar, room filled with lava, levitation/fire immunity/lever elsewhere on level, lava retracts when you grab the key/lever
    id = MT_KEY_LAVA_MOAT_ROOM;
    bc[id] =    (blueprint){{3, 13},           {75, 120},  7,      7,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR),  {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {ndf,   LAVA,       LIQUID,         {60,60},    1,          0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_LAVA_RETRACTABLE, ntt, ndl,     {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          POTION,     POTION_LEVITATION, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          POTION,     POTION_FIRE_IMMUNITY, 0,    1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL | MF_ALTERNATIVE)}}};
    // Lava moat area -- key on an altar, surrounded with lava, levitation/fire immunity elsewhere on level, lava retracts when you grab the key
    id = MT_KEY_LAVA_MOAT_AREA;
    bc[id] =    (blueprint){{3, 13},           {40, 60},   3,      5,          0,                  (BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_TREAT_AS_BLOCKING),  {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_BUILD_AT_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   LAVA,       LIQUID,         {60,60},    1,          0,          0,          0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_LAVA_RETRACTABLE, ntt, ndl,     {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          POTION,     POTION_LEVITATION, 0,       1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          POTION,     POTION_FIRE_IMMUNITY, 0,    1,              0,          0,          (MF_GENERATE_ITEM | MF_BUILD_ANYWHERE_ON_LEVEL | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)}}};
    // Poison gas -- key on an altar; take key to cause a caustic gas vent to appear and the door to be blocked; there is a hidden trapdoor or an escape item somewhere inside
    id = MT_KEY_POISON_GAS_TRAP_ROOM;
    bc[id] =    (blueprint){{4, AMULET_LEVEL}, {35, 60},   7,      7,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM), {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING)},
        {ndf,   MACHINE_POISON_GAS_VENT_HIDDEN,DUNGEON,{1,2}, 1,    0,          -1,         0,              2,              0,          0,          0},
        {ndf,   TRAP_DOOR_HIDDEN,DUNGEON,   {1,1},      1,          0,          -1,         0,              2,              0,          0,          MF_ALTERNATIVE},
        {ndf,   ntt,        ndl,            {1,1},      1,          SCROLL,     SCROLL_TELEPORT, 0,         2,              0,          0,          (MF_GENERATE_ITEM | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   ntt,        ndl,            {1,1},      1,          POTION,     POTION_DESCENT, 0,          2,              0,          0,          (MF_GENERATE_ITEM | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   WALL_LEVER_HIDDEN_DORMANT,DUNGEON,{1,1},1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS)},
        {ndf,   PORTCULLIS_DORMANT,DUNGEON,{1,1},       1,          0,          0,          0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING)}}};
    // Explosive situation -- key on an altar; take key to cause a methane gas vent to appear and a pilot light to ignite
    id = MT_KEY_EXPLOSIVE_TRAP_ROOM;
    bc[id] =    (blueprint){{7, AMULET_LEVEL}, {80, 90},   10,     5,          0,                  (BP_ROOM | BP_PURGE_LIQUIDS | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM),  {
        {ndf,   DOOR,       DUNGEON,        {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {ndf,   FLOOR,      DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_FAR_FROM_ORIGIN)},
        {ndf,   MACHINE_METHANE_VENT_HIDDEN,DUNGEON,{1,1}, 1,       0,          -1,         0,              1,              0,          0,          MF_NEAR_ORIGIN},
        {ndf,   PILOT_LIGHT_DORMANT,DUNGEON,{1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_BUILD_IN_WALLS)}}};
    // Burning grass -- key on an altar; take key to cause pilot light to ignite grass in room
    id = MT_KEY_BURNING_TRAP_ROOM;
    bc[id] =    (blueprint){{1, 7},            {40, 110},  10,     6,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM | BP_OPEN_INTERIOR),  {
        {DF_SMALL_DEAD_GRASS,ALTAR_SWITCH_RETRACTING,DUNGEON,{1,1},1,   0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_FAR_FROM_ORIGIN)},
        {DF_DEAD_FOLIAGE,ntt,SURFACE,       {2,3},      0,          0,          -1,         0,              1,              0,          0,          0},
        {ndf,   FOLIAGE,    SURFACE,        {1,4},      0,          0,          -1,         0,              1,              0,          0,          0},
        {ndf,   GRASS,      SURFACE,        {10,25},    0,          0,          -1,         0,              1,              0,          0,          0},
        {ndf,   DEAD_GRASS, SURFACE,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   PILOT_LIGHT_DORMANT,DUNGEON,{1,1},      1,          0,          -1,         0,              1,              0,          0,          MF_NEAR_ORIGIN | MF_BUILD_IN_WALLS}}};
    // Statuary -- key on an altar, area full of statues; take key to cause statues to burst and reveal monsters
    id = MT_KEY_STATUARY_TRAP_AREA;
    bc[id] =    (blueprint){{10, AMULET_LEVEL},{35, 90},   10,     2,          0,                  (BP_ADOPT_ITEM | BP_NO_INTERIOR_FLAG),  {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   STATUE_DORMANT,DUNGEON,     {3,5},      3,          0,          -1,         0,              2,              HORDE_MACHINE_STATUE,0, (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_FAR_FROM_ORIGIN)}}};
    // Guardian water puzzle -- key held by a guardian, flood trap in the room, glyphs scattered. Lure the guardian into the water to have him drop the key.
    id = MT_KEY_GUARDIAN_WATER_PUZZLE_ROOM;
    freq = getGuardianMonsterId() > 0 ? 8 : 0;
    bc[id] =    (blueprint){{4, AMULET_LEVEL}, {35, 70},    freq,        4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_ADOPT_ITEM), {
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          -1,         getGuardianMonsterId(), 2,      0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_MONSTER_TAKE_ITEM)},
        {ndf,   FLOOD_TRAP,DUNGEON,         {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {1,1},      4,          0,          -1,         0,              2,              0,          0,          (MF_EVERYWHERE | MF_NOT_IN_HALLWAY)}}};
    // Guardian gauntlet -- key in a room full of guardians, glyphs scattered and unavoidable.
    id = MT_KEY_GUARDIAN_GAUNTLET_ROOM;
    freq = getGuardianMonsterId() > 0 ? 10 : 0;
    bc[id] =    (blueprint){{6, AMULET_LEVEL}, {50, 95},    freq,        6,          0,                  (BP_ROOM | BP_ADOPT_ITEM),  {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {ndf,   DOOR,       DUNGEON,        {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {ndf,   ntt,        ndl,            {3,6},      3,          0,          -1,         getGuardianMonsterId(), 2,      0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   ntt,        ndl,            {1,2},      1,          0,          -1,         getWingedGuardianMonsterId(),2, 0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_ALTERNATIVE)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {10,15},   10,          0,          -1,         0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {1,1},      0,          0,          -1,         0,              2,              0,          0,          (MF_EVERYWHERE | MF_PERMIT_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Guardian corridor -- key in a small room, with a connecting corridor full of glyphs, one guardian blocking the corridor.
    id = MT_KEY_GUARDIAN_CORRIDOR_ROOM;
    freq = getGuardianMonsterId() > 0 ? 5 : 0;
    bc[id] =    (blueprint){{4, AMULET_LEVEL}, {85, 100},       freq,       7,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_OPEN_INTERIOR | BP_SURROUND_WITH_WALLS),        {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         getGuardianMonsterId(), 3,      0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN  | MF_ALTERNATIVE)},
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         getWingedGuardianMonsterId(),3, 0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN  | MF_ALTERNATIVE)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {3,5},      2,          0,          0,          0,              2,              0,          0,          MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          0,          0,              3,              0,          0,          MF_BUILD_AT_ORIGIN},
        {ndf,   WALL,DUNGEON,               {80,80},    1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {1,1},      1,          0,          0,          0,              1,              0,          0,          (MF_PERMIT_BLOCKING | MF_EVERYWHERE)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_NOT_IN_HALLWAY | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Summoning circle -- key in a room with an eldritch totem, glyphs unavoidable. // DISABLED. (Not fun enough.)
    id = MT_KEY_SUMMONING_CIRCLE_ROOM;
    bc[id] =    (blueprint){{12, AMULET_LEVEL}, {50, 100}, 0,      2,          0,                  (BP_ROOM | BP_OPEN_INTERIOR | BP_ADOPT_ITEM),   {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {DF_GLYPH_CIRCLE,NOTHING,   DUNGEON,    {1,1},      1,          0,          -1,         /*MK_ELDRITCH_TOTEM*/ 0, 3,     0,          0,          (MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Beckoning obstacle -- key surrounded by glyphs in a room with a mirrored totem.
    id = MT_KEY_BECKONING_OBSTACLE_ROOM;
    freq = getMirrorMonsterId() > 0 ? 10 : 0;
    bc[id] =    (blueprint){{5, AMULET_LEVEL}, {60, 100},  freq,        4,          0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_ADOPT_ITEM), {
        {DF_GLYPH_CIRCLE,ALTAR_INERT,DUNGEON,   {1,1},      1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN | MF_IN_VIEW_OF_ORIGIN)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          -1,         getMirrorMonsterId(), 3,        0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN)},
        {ndf,   ntt,        ndl,            {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {ndf,   MACHINE_GLYPH,DUNGEON,      {3,5},      2,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)}}};
    // Worms in the walls -- key on altar; take key to cause underworms to burst out of the walls
    id = MT_KEY_WORM_TRAP_AREA;
    freq = getDiggerMonsterId() > 0 ? 7 : 0;
    bc[id] =    (blueprint){{12,AMULET_LEVEL}, {7, 7},     freq,        2,          0,                  (BP_ADOPT_ITEM),    {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   WALL_MONSTER_DORMANT,DUNGEON,{5,8},     5,          0,          -1,         getDiggerMonsterId(), 1,        0,          0,          (MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_NOT_ON_LEVEL_PERIMETER)}}};
    // Mud pit -- key on an altar, room full of mud, take key to cause bog monsters to spawn in the mud
    id = MT_KEY_MUD_TRAP_ROOM;
    freq = 0; // disabled
    bc[id] =    (blueprint){{12, AMULET_LEVEL},{40, 90},   0,     3,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS),  {
        {DF_SWAMP,      NOTHING, DUNGEON,               {5,5},      0,          0,          -1,         0,              1,              0,          0,          0},
        {DF_SWAMP,  ALTAR_SWITCH,DUNGEON,               {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {DF_MUD_DORMANT,NOTHING, DUNGEON,               {3,4},      3,          0,          -1,         0,              1,              HORDE_MACHINE_MUD,0,    (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT)}}},
    // Electric crystals -- key caged on an altar, darkened crystal globes around the room, lightning the globes to release the key.
    id = MT_KEY_ELECTRIC_CRYSTALS_ROOM;
    freq = getSparkMonsterId() > 0 ? 10 : 0;
    bc[id] =    (blueprint){{6, AMULET_LEVEL},{40, 60},     freq,       4,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR | BP_PURGE_INTERIOR),  {
        {ndf,   CARPET,     DUNGEON,        {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   ELECTRIC_CRYSTAL_OFF,DUNGEON,{3,4},     3,          0,          -1,         0,              3,              0,          0,          (MF_NOT_IN_HALLWAY | MF_IMPREGNABLE)},
        {ndf,   ALTAR_CAGE_RETRACTABLE,DUNGEON,{1,1},   1,          0,          -1,         0,              3,              0,          0,          (MF_ADOPT_ITEM | MF_IMPREGNABLE | MF_NOT_IN_HALLWAY | MF_FAR_FROM_ORIGIN)},
        {ndf,   TURRET_LEVER, DUNGEON,      {7,9},      4,          0,          -1,         getSparkMonsterId(), 3,         0,          0,          (MF_BUILD_IN_WALLS | MF_MONSTERS_DORMANT)}}};
    // Zombie crypt -- key on an altar; coffins scattered around; brazier in the room; take key to cause zombies to burst out of all of the coffins
    id = MT_KEY_ZOMBIE_TRAP_ROOM;
    freq = getUndeadMonsterId() > 0 ? 10 : 0;
    bc[id] =    (blueprint){{12, AMULET_LEVEL},{60, 90},   freq,     8,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_PURGE_INTERIOR), {
        {ndf,   DOOR,       DUNGEON,        {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {DF_BONES,  ntt,        ndl,        {3,4},      2,          0,          -1,         0,              1,              0,          0,          0},
        {DF_ASH,    ntt,        ndl,        {3,4},      2,          0,          -1,         0,              1,              0,          0,          0},
        {DF_AMBIENT_BLOOD,ntt,  ndl,        {1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {DF_AMBIENT_BLOOD,ntt,  ndl,        {1,2},      1,          0,          -1,         0,              1,              0,          0,          0},
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   BRAZIER,    DUNGEON,        {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_NEAR_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   COFFIN_CLOSED, DUNGEON,     {6,8},      1,          0,          0,          getUndeadMonsterId(), 2,        0,          0,          (MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY | MF_MONSTERS_DORMANT)}}};
    // Haunted house -- key on an altar; take key to cause the room to darken, ectoplasm to cover everything and phantoms to appear
    id = MT_KEY_PHANTOM_TRAP_ROOM;
    freq = getInvisibleMonsterId() > 0 ? 10 : 0;
    bc[id] =    (blueprint){{16, AMULET_LEVEL},{45, 150},  freq,        4,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS), {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   DARK_FLOOR_DORMANT,DUNGEON, {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   DARK_FLOOR_DORMANT,DUNGEON, {4,5},      4,          0,          -1,         getInvisibleMonsterId(), 1,     0,          0,          (MF_MONSTERS_DORMANT)},
        {ndf,   HAUNTED_TORCH_DORMANT,DUNGEON,{5,10},   3,          0,          -1,         0,              2,              0,          0,          (MF_BUILD_IN_WALLS)}}};
    // Worm tunnels -- hidden lever causes tunnels to open up revealing worm areas and a key
    id = MT_KEY_WORM_TUNNEL_ROOM;
    freq = getDiggerMonsterId() > 0 ? 10 : 0;
    bc[id] =    (blueprint){{8, AMULET_LEVEL},{80, 175},    freq,       6,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_PURGE_INTERIOR | BP_MAXIMIZE_INTERIOR | BP_SURROUND_WITH_WALLS),  {
        {ndf,   ALTAR_INERT,DUNGEON,        {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING)},
        {ndf,   ntt,        ndl,            {3,6},      3,          0,          -1,         getDiggerMonsterId(), 1,        0,          0,          0},
        {ndf,   GRANITE,    DUNGEON,        {150,150},  1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_WORM_TUNNEL_MARKER_DORMANT,GRANITE,DUNGEON,{0,0},0,         0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE | MF_PERMIT_BLOCKING)},
        {DF_TUNNELIZE,WORM_TUNNEL_OUTER_WALL,DUNGEON,{1,1}, 1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_PERMIT_BLOCKING)},
        {ndf,   WALL_LEVER_HIDDEN,DUNGEON,  {1,1},      1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_IN_WALLS | MF_IN_PASSABLE_VIEW_OF_ORIGIN | MF_BUILD_ANYWHERE_ON_LEVEL)}}};
    // Gauntlet -- key on an altar; take key to cause turrets to emerge
    id = MT_KEY_TURRET_TRAP_ROOM;
    bc[id] =    (blueprint){{5, 24},           {35, 90},   10,     2,          0,                  (BP_ADOPT_ITEM | BP_NO_INTERIOR_FLAG),  {
        {ndf,   ALTAR_SWITCH,DUNGEON,       {1,1},      1,          0,          -1,         0,              2,              0,          0,          (MF_ADOPT_ITEM | MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING)},
        {ndf,   TURRET_DORMANT,DUNGEON,     {4,6},      4,          0,          -1,         0,              2,              HORDE_MACHINE_TURRET,0, (MF_TREAT_AS_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_IN_VIEW_OF_ORIGIN)}}};
    // Boss -- key is held by a boss atop a pile of bones in a secret room. A few fungus patches light up the area.
    id = MT_KEY_BOSS_ROOM;
    bc[id] =    (blueprint){{5, AMULET_LEVEL}, {40, 100},  18,     3,          0,                  (BP_ROOM | BP_ADOPT_ITEM | BP_SURROUND_WITH_WALLS | BP_PURGE_LIQUIDS), {
        {DF_BONES,  SECRET_DOOR,DUNGEON,        {1,1},      1,          0,          0,          0,              3,              0,          0,          (MF_PERMIT_BLOCKING | MF_BUILD_AT_ORIGIN)},
        {DF_LUMINESCENT_FUNGUS, STATUE_INERT,DUNGEON,{7,7}, 0,          0,          -1,         0,              2,              0,          0,          (MF_TREAT_AS_BLOCKING)},
        {DF_BONES,  ntt,        ndl,            {1,1},      1,          0,          -1,         0,              1,              HORDE_MACHINE_BOSS, 0,  (MF_ADOPT_ITEM | MF_FAR_FROM_ORIGIN | MF_MONSTER_TAKE_ITEM | MF_GENERATE_HORDE | MF_MONSTER_SLEEPING)}}};
    
    // -- FLAVOR MACHINES --
    
    // Bloodwort -- bloodwort stalk, some pods, and surrounding grass
    id = MT_BLOODFLOWER_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {5, 5},     0,          2,      0,                  (BP_TREAT_AS_BLOCKING), {
        {DF_GRASS,  BLOODFLOWER_STALK, SURFACE, {1, 1},     1,          0,          -1,         0,              0,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_NOT_IN_HALLWAY)},
        {DF_BLOODFLOWER_PODS_GROW_INITIAL,ntt, ndl,{1, 1},     1,          0,          -1,         0,              1,              0,          0,          (MF_BUILD_AT_ORIGIN | MF_TREAT_AS_BLOCKING)}}};
    // Shrine -- safe haven constructed and abandoned by a past adventurer
    id = MT_SHRINE_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {15, 25},   0,          3,      0,                  (BP_ROOM | BP_PURGE_INTERIOR | BP_SURROUND_WITH_WALLS | BP_OPEN_INTERIOR), {
        {ndf,   SACRED_GLYPH,  DUNGEON,     {1, 1},     1,          0,          -1,         0,              3,              0,          0,          (MF_BUILD_AT_ORIGIN)},
        {ndf,   HAVEN_BEDROLL, SURFACE,     {1, 1},     1,          0,          -1,         0,              2,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)},
        {ndf,   BONES,      SURFACE,        {1, 1},     1,          (POTION|SCROLL|WEAPON|ARMOR|RING),-1,0, 2,              0,          0,          (MF_GENERATE_ITEM | MF_TREAT_AS_BLOCKING | MF_NOT_IN_HALLWAY)}}};
    // Idyll -- ponds and some grass and forest
    id = MT_IDYLL_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {80, 120},  0,      2,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_GRASS,  FOLIAGE,    SURFACE,        {3, 4},     3,          0,          -1,         0,              1,              0,          0,          0},
        {DF_DEEP_WATER_POOL,ntt,ndl,            {2, 3},     2,          0,          -1,         0,              5,              0,          0,          (MF_NOT_IN_HALLWAY)}}};
    // Swamp -- mud, grass and some shallow water
    id = MT_SWAMP_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {50, 65},   0,      2,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_SWAMP,  ntt,        ndl,            {6, 8},     3,          0,          -1,         0,              1,              0,          0,          0},
        {DF_DEEP_WATER_POOL,ntt,ndl,            {0 ,1},     0,          0,          -1,         0,              3,              0,          0,          (MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING)}}};
    // Camp -- hay, junk, urine, vomit
    id = MT_CAMP_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {40, 50},   0,      4,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_HAY,    ntt,        ndl,            {1, 3},     1,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN)},
        {DF_JUNK,   ntt,        ndl,            {1, 2},     1,          0,          -1,         0,              3,              0,          0,          (MF_NOT_IN_HALLWAY | MF_IN_VIEW_OF_ORIGIN)},
        {DF_URINE,  ntt,        ndl,            {3, 5},     1,          0,          -1,         0,              1,              0,          0,          MF_IN_VIEW_OF_ORIGIN},
        {DF_VOMIT,  ntt,        ndl,            {0 ,2},     0,          0,          -1,         0,              1,              0,          0,          MF_IN_VIEW_OF_ORIGIN}}};
    // Remnant -- carpet surrounded by ash and with some statues
    id = MT_REMNANT_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {80, 120},  0,      2,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_REMNANT, ntt,       ndl,            {6, 8},     3,          0,          -1,         0,              1,              0,          0,          0},
        {ndf,   STATUE_INERT,DUNGEON,       {3, 5},     2,          0,          -1,         0,              1,              0,          0,          (MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING)}}};
    // Dismal -- blood, bones, charcoal, some rubble
    id = MT_DISMAL_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {60, 70},   0,      3,          0,                  BP_NO_INTERIOR_FLAG, {
        {DF_AMBIENT_BLOOD, ntt, ndl,            {5,10},     3,          0,          -1,         0,              1,              0,          0,          MF_NOT_IN_HALLWAY},
        {DF_ASH,    ntt,        ndl,            {4, 8},     2,          0,          -1,         0,              1,              0,          0,          MF_NOT_IN_HALLWAY},
        {DF_BONES,  ntt,        ndl,            {3, 5},     2,          0,          -1,         0,              1,              0,          0,          MF_NOT_IN_HALLWAY}}};
    // Chasm catwalk -- narrow bridge over a chasm, possibly under fire from a turret or two
    id = MT_BRIDGE_TURRET_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL-1},{40, 80},  0,      4,          0,                  (BP_REQUIRE_BLOCKING | BP_OPEN_INTERIOR | BP_NO_INTERIOR_FLAG), {
        {DF_CHASM_HOLE, ntt,    ndl,            {80, 80},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {DF_CATWALK_BRIDGE,ntt, ndl,            {0,0},      0,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)},
        {ndf,   MACHINE_TRIGGER_FLOOR, DUNGEON, {0,1},  0,          0,          0,          0,              1,              0,          0,          (MF_NEAR_ORIGIN | MF_PERMIT_BLOCKING)},
        {ndf,   TURRET_DORMANT,DUNGEON,     {1, 2},     1,          0,          -1,         0,              2,              HORDE_MACHINE_TURRET,0, (MF_TREAT_AS_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_IN_VIEW_OF_ORIGIN)}}};
    // Lake walk -- narrow bridge of shallow water through a lake, possibly under fire from a turret or two
    id = MT_LAKE_PATH_TURRET_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {40, 80},   0,      3,          0,                  (BP_REQUIRE_BLOCKING | BP_OPEN_INTERIOR | BP_NO_INTERIOR_FLAG), {
        {DF_LAKE_CELL,  ntt,    ndl,            {80, 80},   1,          0,          -1,         0,              1,              0,          0,          (MF_TREAT_AS_BLOCKING | MF_REPEAT_UNTIL_NO_PROGRESS)},
        {ndf,   MACHINE_TRIGGER_FLOOR, DUNGEON, {0,1},  0,          0,          0,          0,              1,              0,          0,          (MF_NEAR_ORIGIN | MF_PERMIT_BLOCKING)},
        {ndf,   TURRET_DORMANT,DUNGEON,     {1, 2},     1,          0,          -1,         0,              2,              HORDE_MACHINE_TURRET,0, (MF_TREAT_AS_BLOCKING | MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_IN_VIEW_OF_ORIGIN)}}};
    // Paralysis trap -- already-revealed pressure plate with a few hidden vents nearby.
    id = MT_PARALYSIS_TRAP_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {35, 40},   0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {ndf,   GAS_TRAP_PARALYSIS, DUNGEON, {1,2},     1,          0,          0,          0,              3,              0,          0,          (MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY)},
        {ndf,   MACHINE_PARALYSIS_VENT_HIDDEN,DUNGEON,{3, 4},2,     0,          0,          0,              3,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)}}};
    // Paralysis trap -- hidden pressure plate with a few vents nearby.
    id = MT_PARALYSIS_TRAP_HIDDEN_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {35, 40},   0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {ndf,   GAS_TRAP_PARALYSIS_HIDDEN, DUNGEON, {1,2},1,        0,          0,          0,              3,              0,          0,          (MF_NEAR_ORIGIN | MF_NOT_IN_HALLWAY)},
        {ndf,   MACHINE_PARALYSIS_VENT_HIDDEN,DUNGEON,{3, 4},2,     0,          0,          0,              3,              0,          0,          (MF_FAR_FROM_ORIGIN | MF_NOT_IN_HALLWAY)}}};
    // Statue comes alive -- innocent-looking statue that bursts to reveal a monster when the player approaches
    id = MT_TRICK_STATUE_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {5, 5},     0,      3,          0,                  (BP_NO_INTERIOR_FLAG), {
        {ndf,   STATUE_DORMANT,DUNGEON,     {1, 1},     1,          0,          -1,         0,              1,              HORDE_MACHINE_STATUE,0, (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_AT_ORIGIN | MF_ALTERNATIVE)},
        {ndf,   STATUE_DORMANT,DUNGEON,     {1, 1},     1,          0,          -1,         0,              1,              HORDE_MACHINE_STATUE,0, (MF_GENERATE_HORDE | MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_ALTERNATIVE | MF_NOT_ON_LEVEL_PERIMETER)},
        {ndf,   MACHINE_TRIGGER_FLOOR,DUNGEON,{0,0},    2,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)}}};
    // Worms in the walls -- step on trigger region to cause underworms to burst out of the walls
    id = MT_WORM_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {7, 7},     0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {ndf,   WALL_MONSTER_DORMANT,DUNGEON,{1, 3},    1,          0,          -1,         getDiggerMonsterId(), 1,        0,          0,          (MF_MONSTERS_DORMANT | MF_BUILD_IN_WALLS | MF_NOT_ON_LEVEL_PERIMETER)},
        {ndf,   MACHINE_TRIGGER_FLOOR,DUNGEON,{0,0},    2,          0,          -1,         0,              0,              0,          0,          (MF_EVERYWHERE)}}};
    // Sentinels
    id = MT_SENTINEL_AREA;
    bc[id] =    (blueprint){{1,DEEPEST_LEVEL}, {40, 40},   0,      2,          0,                  (BP_NO_INTERIOR_FLAG), {
        {ndf,   STATUE_DORMANT,DUNGEON,     {3, 3},     3,          0,          -1,         getSentinelMonsterId(), 2,      0,          0,          (MF_NOT_IN_HALLWAY | MF_TREAT_AS_BLOCKING | MF_IN_VIEW_OF_ORIGIN)},
        {DF_ASH,    ntt,        ndl,            {2, 3},     0,          0,          -1,         0,              0,              0,          0,          0}}};
    
}
