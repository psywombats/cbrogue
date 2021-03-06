/*
 *  Globals.c
 *  Brogue
 *
 *  Created by Brian Walker on 1/10/09.
 *  Copyright 2012. All rights reserved.
 *  
 *  This file is part of Brogue.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Rogue.h"
#include "MonsterGlobals.h"

tcell tmap[DCOLS][DROWS];                       // grids with info about the map
pcell pmap[DCOLS][DROWS];
short **scentMap;
cellDisplayBuffer displayBuffer[COLS][ROWS];    // used to optimize plotCharWithColor
short terrainRandomValues[DCOLS][DROWS][8];
short **safetyMap;                              // used to help monsters flee
short **allySafetyMap;                          // used to help allies flee
short **chokeMap;                               // used to assess the importance of the map's various chokepoints
short nbDirs[8][2] = {{0,-1}, {0,1}, {-1,0}, {1,0}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
short cDirs[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
short numberOfWaypoints;
levelData *levels;
creature player;
playerCharacter rogue;
creature *monsters;
creature *dormantMonsters;
creature *graveyard;
creature *purgatory;
item *floorItems;
item *packItems;
item *monsterItemsHopper;

char displayedMessage[MESSAGE_LINES][COLS*2];
bool messageConfirmed[MESSAGE_LINES];
char combatText[COLS * 2];
short messageArchivePosition;
char messageArchive[MESSAGE_ARCHIVE_LINES][COLS*2];

char currentFilePath[BROGUE_FILENAME_MAX];

char displayDetail[DCOLS][DROWS];       // used to make certain per-cell data accessible to external code (e.g. terminal adaptations)

#ifdef AUDIT_RNG
FILE *RNGLogFile;
#endif

unsigned char inputRecordBuffer[INPUT_RECORD_BUFFER + 100];
unsigned short locationInRecordingBuffer;
unsigned long randomNumbersGenerated;
unsigned long positionInPlaybackFile;
unsigned long lengthOfPlaybackFile;
unsigned long recordingLocation;
unsigned long maxLevelChanges;
char annotationPathname[BROGUE_FILENAME_MAX];   // pathname of annotation file
unsigned long previousGameSeed;

#pragma mark Colors
//                            Red     Green   Blue    RedRand GreenRand   BlueRand    Rand    Dances?
// basic colors
color white =                 {100,   100,    100,    0,      0,          0,          0,      false};
color gray =                  {50,    50,     50,     0,      0,          0,          0,      false};
color darkGray =              {30,    30,     30,     0,      0,          0,          0,      false};
color veryDarkGray =          {15,    15,     15,     0,      0,          0,          0,      false};
color black =                 {0,     0,      0,      0,      0,          0,          0,      false};
color yellow =                {100,   100,    0,      0,      0,          0,          0,      false};
color darkYellow =            {50,    50,     0,      0,      0,          0,          0,      false};
color teal =                  {30,    100,    100,    0,      0,          0,          0,      false};
color purple =                {100,   0,      100,    0,      0,          0,          0,      false};
color darkPurple =            {50,    0,      50,     0,      0,          0,          0,      false};
color brown =                 {60,    40,     0,      0,      0,          0,          0,      false};
color green =                 {0,     100,    0,      0,      0,          0,          0,      false};
color darkGreen =             {0,     50,     0,      0,      0,          0,          0,      false};
color orange =                {100,   50,     0,      0,      0,          0,          0,      false};
color darkOrange =            {50,    25,     0,      0,      0,          0,          0,      false};
color blue =                  {0,     0,      100,    0,      0,          0,          0,      false};
color darkBlue =              {0,     0,      50,     0,      0,          0,          0,      false};
color darkTurquoise =         {0,     40,     65,     0,      0,          0,          0,      false};
color lightBlue =             {40,    40,     100,    0,      0,          0,          0,      false};
color pink =                  {100,   60,     66,     0,      0,          0,          0,      false};
color red  =                  {100,   0,      0,      0,      0,          0,          0,      false};
color darkRed =               {50,    0,      0,      0,      0,          0,          0,      false};
color tanColor =              {80,    67,     15,     0,      0,          0,          0,      false};

// bolt colors
color rainbow =               {-70,   -70,    -70,    170,    170,        170,        0,      true};
color descentBoltColor =      {-40,   -40,    -40,    0,      0,          80,         80,     true};
color discordColor =          {25,    0,      25,     66,     0,          0,          0,      true};
color poisonColor =           {0,     0,      0,      10,     50,         10,         0,      true};
color beckonColor =           {10,    10,     10,     5,      5,          5,          50,     true};
color invulnerabilityColor =  {25,    0,      25,     0,      0,          66,         0,      true};
color dominationColor =       {0,     0,      100,    80,     25,         0,          0,      true};
color empowermentColor =      {30,    100,    40,     25,     80,         25,         0,      true};
color fireBoltColor =         {500,   150,    0,      45,     30,         0,          0,      true};
color yendorLightColor =      {50,    -100,    30,     0,      0,          0,          0,      true};
color dragonFireColor =       {500,   150,    0,      45,     30,         45,         0,      true};
color flamedancerCoronaColor ={500,   150,    100,    45,     30,         0,          0,      true};
//color shieldingColor =      {100,   50,     0,      0,      50,         100,        0,      true};
color shieldingColor =        {150,   75,     0,      0,      50,         175,        0,      true};

// tile colors
color undiscoveredColor =     {0,     0,      0,      0,      0,          0,          0,      false};

color wallForeColor =         {7,     7,      7,      3,      3,          3,          0,      false};

color wallBackColor;
color wallBackColorStart =    {45,    40,     40,     15,     0,          5,          20,     false};
color wallBackColorEnd =      {40,    30,     35,     0,      20,         30,         20,     false};

color mudWallForeColor =      {55,    45,     0,      5,      5,          5,          1,      false};
//color mudWallForeColor =      {40,  34,     7,      0,      3,          0,          3,      false};
color mudWallBackColor =      {20,    12,     3,      8,      4,          3,          0,      false};

color graniteBackColor =      {10,    10,     10,     0,      0,          0,          0,      false};

color floorForeColor =        {30,    30,     30,     0,      0,          0,          35,     false};

color floorBackColor;
color floorBackColorStart =   {2,     2,      10,     2,      2,          0,          0,      false};
color floorBackColorEnd =     {5,     5,      5,      2,      2,          0,          0,      false};

color stairsBackColor =       {15,    15,     5,      0,      0,          0,          0,      false};
color firstStairsBackColor =  {10,    10,     25,     0,      0,          0,          0,      false};

color refuseBackColor =       {6,     5,      3,      2,      2,          0,          0,      false};
color rubbleBackColor =       {7,     7,      8,      2,      2,          1,          0,      false};
color bloodflowerForeColor =  {30,    5,      40,     5,      1,          3,          0,      false};
color bloodflowerPodForeColor = {50,  5,      25,     5,      1,          3,          0,      false};
color bloodflowerBackColor =  {15,    3,      10,     3,      1,          3,          0,      false};
color bedrollBackColor =      {10,    8,      5,      1,      1,          0,          0,      false};

color obsidianBackColor =     {6,     0,      8,      2,      0,          3,          0,      false};
color carpetForeColor =       {23,    30,     38,     0,      0,          0,          0,      false};
color carpetBackColor =       {15,    8,      5,      0,      0,          0,          0,      false};
color marbleForeColor =       {30,    23,     38,     0,      0,          0,          0,      false};
color marbleBackColor =       {6,     5,      13,     1,      0,          1,          0,      false};
color doorForeColor =         {70,    35,     15,     0,      0,          0,          0,      false};
color doorBackColor =         {30,    10,     5,      0,      0,          0,          0,      false};
//color ironDoorForeColor =       {40,    40,     40,     0,      0,          0,          0,      false};
color ironDoorForeColor =     {500,   500,    500,    0,      0,          0,          0,      false};
color ironDoorBackColor =     {15,    15,     30,     0,      0,          0,          0,      false};
color bridgeFrontColor =      {33,    12,     12,     12,     7,          2,          0,      false};
color bridgeBackColor =       {12,    3,      2,      3,      2,          1,          0,      false};
color statueBackColor =       {20,    20,     20,     0,      0,          0,          0,      false};
color glyphColor =            {20,    5,      5,      50,     0,          0,          0,      true};
color glyphLightColor =       {150,   0,      0,      150,    0,          0,          0,      true};
color sacredGlyphColor =      {5,     20,     5,      0,      50,         0,          0,      true};
color sacredGlyphLightColor = {45,    150,    60,     25,     80,         25,         0,      true};

//color deepWaterForeColor =  {5,     5,      40,     0,      0,          10,         10,     true};
//color deepWaterBackColor;
//color deepWaterBackColorStart = {5, 5,      55,     5,      5,          10,         10,     true};
//color deepWaterBackColorEnd =   {5,     5,      45,     2,      2,          5,          5,      true};
//color shallowWaterForeColor =   {40,    40,     90,     0,      0,          10,         10,     true};
//color shallowWaterBackColor;
//color shallowWaterBackColorStart ={30,30,       80,     0,      0,          10,         10,     true};
//color shallowWaterBackColorEnd ={20,    20,     60,     0,      0,          5,          5,      true};

color deepWaterForeColor =    {5,     8,      20,     0,      4,          15,         10,     true};
color deepWaterBackColor;
color deepWaterBackColorStart = {5,   10,     31,     5,      5,          5,          6,      true};
color deepWaterBackColorEnd = {5,     8,      20,     2,      3,          5,          5,      true};
color shallowWaterForeColor = {28,    28,     60,     0,      0,          10,         10,     true};
color shallowWaterBackColor;
color shallowWaterBackColorStart ={20,20,     60,     0,      0,          10,         10,     true};
color shallowWaterBackColorEnd ={12,  15,     40,     0,      0,          5,          5,      true};

color mudForeColor =          {18,    14,     5,      5,      5,          0,          0,      false};
color mudBackColor =          {23,    17,     7,      5,      5,          0,          0,      false};
color chasmForeColor =        {7,     7,      15,     4,      4,          8,          0,      false};
color chasmEdgeBackColor;
color chasmEdgeBackColorStart ={5,    5,      25,     2,      2,          2,          0,      false};
color chasmEdgeBackColorEnd = {8,     8,      20,     2,      2,          2,          0,      false};
color fireForeColor =         {70,    20,     0,      15,     10,         0,          0,      true};
color lavaForeColor =         {20,    20,     20,     100,    10,         0,          0,      true};
color brimstoneForeColor =    {100,   50,     10,     0,      50,         40,         0,      true};
color brimstoneBackColor =    {18,    12,     9,      0,      0,          5,          0,      false};

color lavaBackColor =         {70,    20,     0,      15,     10,         0,          0,      true};
color acidBackColor =         {15,    80,     25,     5,      15,         10,         0,      true};

color lightningColor =        {100,   150,    500,    50,     50,         0,          50,     true};
color fungusLightColor =      {2,     11,     11,     4,      3,          3,          0,      true};
color lavaLightColor =        {47,    13,     0,      10,     7,          0,          0,      true};
color deepWaterLightColor =   {10,    30,     100,    0,      30,         100,        0,      true};

color grassColor =            {15,    40,     15,     15,     50,         15,         10,     false};
color deadGrassColor =        {20,    13,     0,      20,     10,         5,          10,     false};
color fungusColor =           {15,    50,     50,     0,      25,         0,          30,     true};
color grayFungusColor =       {30,    30,     30,     5,      5,          5,          10,     false};
color foliageColor =          {25,    100,    25,     15,     0,          15,         0,      false};
color deadFoliageColor =      {20,    13,     0,      30,     15,         0,          20,     false};
color lichenColor =           {50,    5,      25,     10,     0,          5,          0,      true};
color hayColor =              {70,    55,     5,      0,      20,         20,         0,      false};
color ashForeColor =          {20,    20,     20,     0,      0,          0,          20,     false};
color bonesForeColor =        {80,    80,     30,     5,      5,          35,         5,      false};
color ectoplasmColor =        {45,    20,     55,     25,     0,          25,         5,      false};
color forceFieldColor =       {0,     25,     25,     0,      25,         25,         0,      true};
color wallCrystalColor =      {40,    40,     60,     20,     20,         40,         0,      true};
color altarForeColor =        {5,     7,      9,      0,      0,          0,          0,      false};
color altarBackColor =        {35,    18,     18,     0,      0,          0,          0,      false};
color greenAltarBackColor =   {18,    25,     18,     0,      0,          0,          0,      false};
color goldAltarBackColor =    {25,    24,     12,     0,      0,          0,          0,      false};
color pedestalBackColor =     {10,    5,      20,     0,      0,          0,          0,      false};

// monster colors
color goblinColor =           {40,    30,     20,     0,      0,          0,          0,      false};
color jackalColor =           {60,    42,     27,     0,      0,          0,          0,      false};
color ogreColor =             {60,    25,     25,     0,      0,          0,          0,      false};
color eelColor =              {30,    12,     12,     0,      0,          0,          0,      false};
color goblinConjurerColor =   {67,    10,     100,    0,      0,          0,          0,      false};
color spectralBladeColor =    {15,    15,     60,     0,      0,          70,         50,     true};
color spectralImageColor =    {13,    0,      0,      25,     0,          0,          0,      true};
color toadColor =             {40,    65,     30,     0,      0,          0,          0,      false};
color trollColor =            {40,    60,     15,     0,      0,          0,          0,      false};
color centipedeColor =        {75,    25,     85,     0,      0,          0,          0,      false};
color dragonColor =           {20,    80,     15,     0,      0,          0,          0,      false};
color krakenColor =           {100,   55,     55,     0,      0,          0,          0,      false};
color salamanderColor =       {40,    10,     0,      8,      5,          0,          0,      true};
color pixieColor =            {60,    60,     60,     40,     40,         40,         0,      true};
color darPriestessColor =     {0,     50,     50,     0,      0,          0,          0,      false};
color darMageColor =          {50,    50,     0,      0,      0,          0,          0,      false};
color wraithColor =           {66,    66,     25,     0,      0,          0,          0,      false};
color pinkJellyColor =        {100,   40,     40,     5,      5,          5,          20,     true};
color wormColor =             {80,    60,     40,     0,      0,          0,          0,      false};
color sentinelColor =         {3,     3,      30,     0,      0,          10,         0,      true};
color goblinMysticColor =     {10,    67,     100,    0,      0,          0,          0,      false};
color ifritColor =            {50,    10,     100,    75,     0,          20,         0,      true};
color phoenixColor =          {100,   0,      0,      0,      100,        0,          0,      true};

// light colors
color minersLightColor;
color minersLightStartColor = {180,   180,    180,    0,      0,          0,          0,      false};
color minersLightEndColor =   {90,    90,     120,    0,      0,          0,          0,      false};
color torchColor =            {150,   75,     30,     0,      30,         20,         0,      true};
color torchLightColor =       {75,    38,     15,     0,      15,         7,          0,      true};
//color hauntedTorchColor =     {75,  30,     150,    30,     20,         0,          0,      true};
color hauntedTorchColor =     {75,    20,     40,     30,     10,         0,          0,      true};
//color hauntedTorchLightColor ={19,     7,       37,     8,      4,          0,          0,      true};
color hauntedTorchLightColor ={67,    10,     10,     20,     4,          0,          0,      true};
color ifritLightColor =       {0,     10,     150,    100,    0,          100,        0,      true};
//color unicornLightColor =       {-50,   -50,    -50,    200,    200,        200,        0,      true};
color unicornLightColor =     {-50,   -50,    -50,    250,    250,        250,        0,      true};
color wispLightColor =        {75,    100,    250,    33,     10,         0,          0,      true};
color summonedImageLightColor ={200,  0,      75,     0,      0,          0,          0,      true};
color spectralBladeLightColor ={40,   0,      230,    0,      0,          0,          0,      true};
color ectoplasmLightColor =   {23,    10,     28,     13,     0,          13,         3,      false};
color explosionColor =        {10,    8,      2,      0,      2,          2,          0,      true};
color explosiveAuraColor =    {2000,  0,      -1000,  200,    200,        0,          0,      true};
color dartFlashColor =        {500,   500,    500,    0,      2,          2,          0,      true};
color lichLightColor =        {-50,   80,     30,     0,      0,          20,         0,      true};
color forceFieldLightColor =  {10,    10,     10,     0,      50,         50,         0,      true};
color crystalWallLightColor = {10,    10,     10,     0,      0,          50,         0,      true};
color sunLightColor =         {100,   100,    75,     0,      0,          0,          0,      false};
color fungusForestLightColor ={30,    40,     60,     0,      0,          0,          40,     true};
color fungusTrampledLightColor ={10,  10,     10,     0,      50,         50,         0,      true};
color redFlashColor =         {100,   10,     10,     0,      0,          0,          0,      false};
color darknessPatchColor =    {-10,   -10,    -10,    0,      0,          0,          0,      false};
color darknessCloudColor =    {-20,   -20,    -20,    0,      0,          0,          0,      false};
color magicMapFlashColor =    {60,    20,     60,     0,      0,          0,          0,      false};
color sentinelLightColor =    {20,    20,     120,    10,     10,         60,         0,      true};
color telepathyColor =        {30,    30,     130,    0,      0,          0,          0,      false};
color confusionLightColor =   {10,    10,     10,     10,     10,         10,         0,      true};
color portalActivateLightColor ={300, 400,    500,    0,      0,          0,          0,      true};
color descentLightColor =     {20,    20,     70,     0,      0,          0,          0,      false};
color algaeBlueLightColor =   {20,    15,     50,     0,      0,          0,          0,      false};
color algaeGreenLightColor =  {15,    50,     20,     0,      0,          0,          0,      false};

// flare colors
color scrollProtectionColor = {375,   750,    0,      0,      0,          0,          0,      true};
color scrollEnchantmentColor ={250,   225,    300,    0,      0,          450,        0,      true};
color potionStrengthColor =   {1000,  0,      400,    600,    0,          0,          0,      true};
color empowermentFlashColor = {500,   1000,   600,    0,      500,        0,          0,      true};
color genericFlashColor =     {800,   800,    800,    0,      0,          0,          0,      false};
color summoningFlashColor =   {0,     0,      0,      600,    0,          1200,       0,      true};
color fireFlashColor =        {750,   225,    0,      100,    50,         0,          0,      true};
color explosionFlareColor =   {10000, 6000,   1000,   0,      0,          0,          0,      false};
color quietusFlashColor =     {0,     -1000,  -200,   0,      0,          0,          0,      true};
color slayingFlashColor =     {-1000, -200,   0,      0,      0,          0,          0,      true};

// color multipliers
color colorDim25 =            {25,    25,     25,     25,     25,         25,         25,     false};
color colorMultiplier100 =    {100,   100,    100,    100,    100,        100,        100,    false};
color memoryColor =           {25,    25,     50,     20,     20,         20,         0,      false};
color memoryOverlay =         {25,    25,     50,     0,      0,          0,          0,      false};
color magicMapColor =         {60,    20,     60,     60,     20,         60,         0,      false};
color clairvoyanceColor =     {50,    90,     50,     50,     90,         50,         66,     false};
color telepathyMultiplier =   {30,    30,     130,    30,     30,         130,        66,     false};
color omniscienceColor =      {140,   100,    60,     140,    100,        60,         90,     false};
color basicLightColor =       {180,   180,    180,    180,    180,        180,        180,    false};

// blood colors
color humanBloodColor =       {60,    20,     10,     15,     0,          0,          15,     false};
color insectBloodColor =      {10,    60,     20,     0,      15,         0,          15,     false};
color vomitColor =            {60,    50,     5,      0,      15,         15,         0,      false};
color urineColor =            {70,    70,     40,     0,      0,          0,          10,     false};
color methaneColor =          {45,    60,     15,     0,      0,          0,          0,      false};

// gas colors
color poisonGasColor =        {75,    25,     85,     0,      0,          0,          0,      false};
color confusionGasColor =     {60,    60,     60,     40,     40,         40,         0,      true};

// interface colors
color itemColor =             {100,   95,     -30,    0,      0,          0,          0,      false};
color blueBar =               {15,    10,     50,     0,      0,          0,          0,      false};
color redBar =                {45,    10,     15,     0,      0,          0,          0,      false};
color hiliteColor =           {100,   100,    0,      0,      0,          0,          0,      false};
color interfaceBoxColor =     {7,     6,      15,     0,      0,          0,          0,      false};
color interfaceButtonColor =  {18,    15,     38,     0,      0,          0,          0,      false};
color buttonHoverColor =      {100,   70,     40,     0,      0,          0,          0,      false};
color titleButtonColor =      {23,    15,     30,     0,      0,          0,          0,      false};

color playerInvisibleColor =  {20,    20,     30,     0,      0,          80,         0,      true};
color playerInLightColor =    {100,   90,     30,     0,      0,          0,          0,      false};
color playerInShadowColor =   {60,    60,     100,    0,      0,          0,          0,      false};
color playerInDarknessColor = {30,    30,     65,     0,      0,          0,          0,      false};

color inLightMultiplierColor ={150,   150,    75,     150,    150,        75,         100,    true};
color inDarknessMultiplierColor={66,  66,     120,    66,     66,         120,        66,     true};

color goodMessageColor =      {60,    50,     100,    0,      0,          0,          0,      false};
color badMessageColor =       {100,   50,     60,     0,      0,          0,          0,      false};
color advancementMessageColor ={50,   100,    60,     0,      0,          0,          0,      false};
color itemMessageColor =      {100,   100,    50,     0,      0,          0,          0,      false};
color flavorTextColor =       {50,    40,     90,     0,      0,          0,          0,      false};
color backgroundMessageColor ={60,    20,     70,     0,      0,          0,          0,      false};

color superVictoryColor =     {150,   100,    300,    0,      0,          0,          0,      false};

//color flameSourceColor = {0, 0, 0, 65, 40, 100, 0, true}; // 1
//color flameSourceColor = {0, 0, 0, 80, 50, 100, 0, true}; // 2
//color flameSourceColor = {25, 13, 25, 50, 25, 50, 0, true}; // 3
//color flameSourceColor = {20, 20, 20, 60, 20, 40, 0, true}; // 4
//color flameSourceColor = {30, 18, 18, 70, 36, 36, 0, true}; // 7**
color flameSourceColor = {20, 7, 7, 60, 40, 40, 0, true}; // 8
color flameSourceColorSecondary = {7, 2, 0, 10, 0, 0, 0, true};

//color flameTitleColor = {0, 0, 0, 17, 10, 6, 0, true}; // pale orange
//color flameTitleColor = {0, 0, 0, 7, 7, 10, 0, true}; // *pale blue*
color flameTitleColor = {0, 0, 0, 9, 9, 15, 0, true}; // *pale blue**
//color flameTitleColor = {0, 0, 0, 11, 11, 18, 0, true}; // *pale blue*
//color flameTitleColor = {0, 0, 0, 15, 15, 9, 0, true}; // pale yellow
//color flameTitleColor = {0, 0, 0, 15, 9, 15, 0, true}; // pale purple

#pragma mark Dynamic color references

color *dynamicColors[NUMBER_DYNAMIC_COLORS][3] = {
    // used color           shallow color               deep color
    {&minersLightColor,     &minersLightStartColor,     &minersLightEndColor},
    {&wallBackColor,        &wallBackColorStart,        &wallBackColorEnd},
    {&deepWaterBackColor,   &deepWaterBackColorStart,   &deepWaterBackColorEnd},
    {&shallowWaterBackColor,&shallowWaterBackColorStart,&shallowWaterBackColorEnd},
    {&floorBackColor,       &floorBackColorStart,       &floorBackColorEnd},
    {&chasmEdgeBackColor,   &chasmEdgeBackColorStart,   &chasmEdgeBackColorEnd},
};

#pragma mark Autogenerator definitions

autoGenerator autoGeneratorCatalog[NUMBER_AUTOGENERATORS] = {
//   terrain                    layer   DF                          Machine                     reqDungeon  reqLiquid   >Depth  <Depth          freq    minIncp minSlope    maxNumber
    // Ordinary features of the dungeon
    {NOTHING,                   DUNGEON,DF_GRANITE_COLUMN,          MT_NONE,                    FLOOR,      NOTHING,    1,      DEEPEST_LEVEL,  60,     100,    0,          4},
    {NOTHING,                   DUNGEON,DF_CRYSTAL_WALL,            MT_NONE,                    WALL,       NOTHING,    14,     DEEPEST_LEVEL,  15,     -325,   25,         5},
    {NOTHING,                   DUNGEON,DF_LUMINESCENT_FUNGUS,      MT_NONE,                    FLOOR,      NOTHING,    7,      DEEPEST_LEVEL,  15,     -300,   70,         14},
    {NOTHING,                   DUNGEON,DF_GRASS,                   MT_NONE,                    FLOOR,      NOTHING,    0,      10,             0,      1000,   -80,        10},
    {NOTHING,                   DUNGEON,DF_DEAD_GRASS,              MT_NONE,                    FLOOR,      NOTHING,    4,      9,              0,      -200,   80,         10},
    {NOTHING,                   DUNGEON,DF_DEAD_GRASS,              MT_NONE,                    FLOOR,      NOTHING,    9,      14,             0,      1200,   -80,        10},
    {NOTHING,                   DUNGEON,DF_BONES,                   MT_NONE,                    FLOOR,      NOTHING,    12,     DEEPEST_LEVEL-1,30,     0,      0,          4},
    {NOTHING,                   DUNGEON,DF_RUBBLE,                  MT_NONE,                    FLOOR,      NOTHING,    0,      DEEPEST_LEVEL-1,30,     0,      0,          4},
    {NOTHING,                   DUNGEON,DF_FOLIAGE,                 MT_NONE,                    FLOOR,      NOTHING,    0,      8,              15,     1000,   -333,       10},
    {NOTHING,                   DUNGEON,DF_FUNGUS_FOREST,           MT_NONE,                    FLOOR,      NOTHING,    13,     DEEPEST_LEVEL,  30,     -600,   50,         12},
    {NOTHING,                   DUNGEON,DF_BUILD_ALGAE_WELL,        MT_NONE,                    FLOOR,      DEEP_WATER, 10,     DEEPEST_LEVEL,  50,     0,      0,          2},
    {STATUE_INERT,              DUNGEON,DF_NONE,                    MT_NONE,                    WALL,       NOTHING,    6,      DEEPEST_LEVEL-1,5,      -100,   35,         3},
    {STATUE_INERT,              DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    10,     DEEPEST_LEVEL-1,50,     0,      0,          3},
    {TORCH_WALL,                DUNGEON,DF_NONE,                    MT_NONE,                    WALL,       NOTHING,    6,      DEEPEST_LEVEL-1,5,      -200,   70,         12},

    // Pre-revealed traps
    {GAS_TRAP_POISON,           DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    2,      4,              20,     0,      0,          1},
    {NET_TRAP,                  DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    2,      5,              20,     0,      0,          1},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_PARALYSIS_TRAP_AREA,     FLOOR,      NOTHING,    2,      6,              20,     0,      0,          1},
    {ALARM_TRAP,                DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    4,      7,              20,     0,      0,          1},
    {GAS_TRAP_CONFUSION,        DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    2,      10,             20,     0,      0,          1},
    {FLAMETHROWER,              DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    4,      12,             20,     0,      0,          1},
    {FLOOD_TRAP,                DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    10,     14,             20,     0,      0,          1},

    // Hidden traps
    {GAS_TRAP_POISON_HIDDEN,    DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    5,      DEEPEST_LEVEL-1,20,     100,    0,          3},
    {NET_TRAP_HIDDEN,           DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    6,      DEEPEST_LEVEL-1,20,     100,    0,          3},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_PARALYSIS_TRAP_HIDDEN_AREA, FLOOR,   NOTHING,    7,      DEEPEST_LEVEL-1,20,     100,    0,          3},
    {ALARM_TRAP_HIDDEN,         DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    8,      DEEPEST_LEVEL-1,20,     100,    0,          2},
    {TRAP_DOOR_HIDDEN,          DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    9,      DEEPEST_LEVEL-1,20,     100,    0,          2},
    {GAS_TRAP_CONFUSION_HIDDEN, DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    11,     DEEPEST_LEVEL-1,20,     100,    0,          3},
    {FLAMETHROWER_HIDDEN,       DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    13,     DEEPEST_LEVEL-1,20,     100,    0,          3},
    {FLOOD_TRAP_HIDDEN,         DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    15,     DEEPEST_LEVEL-1,20,     100,    0,          3},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_SWAMP_AREA,              FLOOR,      NOTHING,    1,      DEEPEST_LEVEL-1,30,     0,      0,          2},
    {NOTHING,                   DUNGEON,DF_SUNLIGHT,                MT_NONE,                    FLOOR,      NOTHING,    0,      5,              15,     500,    -150,       10},
    {NOTHING,                   DUNGEON,DF_DARKNESS,                MT_NONE,                    FLOOR,      NOTHING,    1,      15,             15,     500,    -50,        10},
    {STEAM_VENT,                DUNGEON,DF_NONE,                    MT_NONE,                    FLOOR,      NOTHING,    16,     DEEPEST_LEVEL-1,30,     100,    0,          3},
    {CRYSTAL_WALL,              DUNGEON,DF_NONE,                    MT_NONE,                    WALL,       NOTHING,    DEEPEST_LEVEL,DEEPEST_LEVEL,100,0,      0,          600},

    // Dewars
    {DEWAR_CAUSTIC_GAS,         DUNGEON,DF_CARPET_AREA,             MT_NONE,                    FLOOR,      NOTHING,    8,      DEEPEST_LEVEL-1,2,      0,      0,          2},
    {DEWAR_CONFUSION_GAS,       DUNGEON,DF_CARPET_AREA,             MT_NONE,                    FLOOR,      NOTHING,    8,      DEEPEST_LEVEL-1,2,      0,      0,          2},
    {DEWAR_PARALYSIS_GAS,       DUNGEON,DF_CARPET_AREA,             MT_NONE,                    FLOOR,      NOTHING,    8,      DEEPEST_LEVEL-1,2,      0,      0,          2},
    {DEWAR_METHANE_GAS,         DUNGEON,DF_CARPET_AREA,             MT_NONE,                    FLOOR,      NOTHING,    8,      DEEPEST_LEVEL-1,2,      0,      0,          2},
    
    // Flavor machines
    {NOTHING,                   DUNGEON,DF_LUMINESCENT_FUNGUS,      MT_NONE,                    FLOOR,      NOTHING,    DEEPEST_LEVEL,DEEPEST_LEVEL,100,0,      0,          200},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_BLOODFLOWER_AREA,        FLOOR,      NOTHING,    1,      30,             25,     140,    -10,        3},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_SHRINE_AREA,             FLOOR,      NOTHING,    5,      AMULET_LEVEL,   7,      0,      0,          1},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_IDYLL_AREA,              FLOOR,      NOTHING,    1,      5,              15,     0,      0,          1},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_REMNANT_AREA,            FLOOR,      NOTHING,    10,     DEEPEST_LEVEL,  15,     0,      0,          2},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_DISMAL_AREA,             FLOOR,      NOTHING,    7,      DEEPEST_LEVEL,  12,     0,      0,          5},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_BRIDGE_TURRET_AREA,      FLOOR,      NOTHING,    5,      DEEPEST_LEVEL-1,6,      0,      0,          2},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_LAKE_PATH_TURRET_AREA,   FLOOR,      NOTHING,    5,      DEEPEST_LEVEL-1,6,      0,      0,          2},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_TRICK_STATUE_AREA,       FLOOR,      NOTHING,    6,      DEEPEST_LEVEL-1,15,     0,      0,          3},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_SENTINEL_AREA,           FLOOR,      NOTHING,    12,     DEEPEST_LEVEL-1,10,     0,      0,          2},
    {NOTHING,                   DUNGEON,DF_NONE,                    MT_WORM_AREA,               FLOOR,      NOTHING,    12,     DEEPEST_LEVEL-1,12,     0,      0,          3},
};

#pragma mark Terrain definitions

floorTileType tileCatalog[NUMBER_TILETYPES] = {
    
    // promoteChance is in hundredths of a percent per turn
    
    //  char        fore color              back color      priority    ignit   fireType    discovType  promoteType     promoteChance   glowLight       flags                                                                                               description         flavorText
    
    // dungeon layer (this layer must have all of fore color, back color and char)
    {   ' ',        &black,                 &black,                 100,0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "a chilly void",        ""},
    {WALL_CHAR,     &wallBackColor,         &graniteBackColor,      0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                       "a rough granite wall", "The granite is split open with splinters of rock jutting out at odd angles."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "the ground",           ""},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "the ground",           ""},
    {FLOOR_CHAR,    &carpetForeColor,       &carpetBackColor,       85, 0,  DF_EMBERS,      DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "the carpet",           "Ornate carpeting fills this room, a relic of ages past."},
    {FLOOR_CHAR,    &marbleForeColor,       &marbleBackColor,       85, 0,  DF_EMBERS,      DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "the marble ground",    "Light from the nearby crystals catches the grain of the lavish marble floor."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                       "a stone wall",         "The rough stone wall is firm and unyielding."},
    {DOOR_CHAR,     &doorForeColor,         &doorBackColor,         25, 50, DF_EMBERS,      DF_NONE,    DF_OPEN_DOOR,   0,              NO_LIGHT,       (T_OBSTRUCTS_VISION | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP | TM_VISUALLY_DISTINCT), "a wooden door", "you pass through the doorway."},
    {OPEN_DOOR_CHAR,&doorForeColor,         &doorBackColor,         25, 50, DF_EMBERS,      DF_NONE,    DF_CLOSED_DOOR, 10000,          NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),           "an open door",         "you pass through the doorway."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  50, DF_EMBERS,      DF_SHOW_DOOR,DF_NONE,       0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET),  "a stone wall",     "The rough stone wall is firm and unyielding."},
    {DOOR_CHAR,     &ironDoorForeColor,     &ironDoorBackColor,     15, 50, DF_EMBERS,      DF_NONE,    DF_OPEN_IRON_DOOR_INERT,0,      NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_WITH_KEY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED),  "a locked iron door",   "you search your pack but do not have a matching key."},
    {OPEN_DOOR_CHAR,&white,                 &ironDoorBackColor,     90, 50, DF_EMBERS,      DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VISUALLY_DISTINCT),                           "an open iron door",    "you pass through the doorway."},
    {DESCEND_CHAR,  &itemColor,             &stairsBackColor,       30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_REPEL_CREATURES, 0,          NO_LIGHT,       (T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "a downward staircase",   "stairs spiral downward into the depths."},
    {ASCEND_CHAR,   &itemColor,             &stairsBackColor,       30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_REPEL_CREATURES, 0,          NO_LIGHT,       (T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "an upward staircase",    "stairs spiral upward."},
    {OMEGA_CHAR,    &lightBlue,             &firstStairsBackColor,  30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_REPEL_CREATURES, 0,          NO_LIGHT,       (T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "the dungeon exit",       "the gilded doors leading out of the dungeon are sealed by an invisible force."},
    {OMEGA_CHAR,    &wallCrystalColor,      &firstStairsBackColor,  30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_REPEL_CREATURES, 0,          INCENDIARY_DART_LIGHT,      (T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_ON_STEP | TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_BRIGHT_MEMORY | TM_INTERRUPT_EXPLORATION_WHEN_SEEN | TM_INVERT_WHEN_HIGHLIGHTED), "a crystal portal",       "dancing lights play across the plane of this sparkling crystal portal."},
    {WALL_CHAR,     &torchColor,            &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              TORCH_LIGHT,    (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                       "a wall-mounted torch", "The torch is anchored firmly to the wall and sputters quietly in the gloom."},
    {WALL_CHAR,     &wallCrystalColor,      &wallCrystalColor,      0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              CRYSTAL_WALL_LIGHT,(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_DIAGONAL_MOVEMENT), (TM_STAND_IN_TILE | TM_REFLECTS_BOLTS),"a crystal formation", "You feel the crystal's glossy surface and admire the dancing lights beneath."},
    {WALL_CHAR,     &gray,                  &floorBackColor,        10, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_OPEN_PORTCULLIS, 0,          NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_CONNECTS_LEVEL), "a heavy portcullis", "The iron bars rattle but will not budge; they are firmly locked in place."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_ACTIVATE_PORTCULLIS,0,       NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "the ground",           ""},
    {WALL_CHAR,     &doorForeColor,         &floorBackColor,        10, 100,DF_WOODEN_BARRICADE_BURN,DF_NONE,DF_NONE,   0,              NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_CONNECTS_LEVEL),"a dry wooden barricade","The wooden barricade is firmly set but has dried over the years. Might it burn?"},
    {WALL_CHAR,     &torchLightColor,       &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_PILOT_LIGHT, 0,              TORCH_LIGHT,    (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),            "a wall-mounted torch", "The torch is anchored firmly to the wall, and sputters quietly in the gloom."},
    {FIRE_CHAR,     &fireForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              TORCH_LIGHT,    (T_OBSTRUCTS_EVERYTHING | T_IS_FIRE), (TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR),                      "a fallen torch",       "The torch lies at the foot of the wall, spouting gouts of flame haphazardly."},
    {WALL_CHAR,     &torchColor,            &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_HAUNTED_TORCH_TRANSITION,0,  TORCH_LIGHT,    (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),            "a wall-mounted torch", "The torch is anchored firmly to the wall and sputters quietly in the gloom."},
    {WALL_CHAR,     &torchColor,            &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_HAUNTED_TORCH,2000,          TORCH_LIGHT,    (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                          "a wall-mounted torch", "The torch is anchored firmly to the wall and sputters quietly in the gloom."},
    {WALL_CHAR,     &hauntedTorchColor,     &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              HAUNTED_TORCH_LIGHT,(T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                   "a sputtering torch",   "A dim purple flame sputters and spits atop this wall-mounted torch."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_REVEAL_LEVER,DF_NONE,    0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET),           "a stone wall",         "The rough stone wall is firm and unyielding."},
    {LEVER_CHAR,    &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_PULL_LEVER,  0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_INVERT_WHEN_HIGHLIGHTED),"a lever", "The lever moves."},
    {LEVER_PULLED_CHAR,&wallForeColor,      &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                       "an inactive lever",    "The lever won't budge."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_CREATE_LEVER,0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_IS_WIRED),                                         "a stone wall",         "The rough stone wall is firm and unyielding."},
    {STATUE_CHAR,   &wallBackColor,         &statueBackColor,       0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE),  "a marble statue",  "The cold marble statue has weathered the years with grace."},
    {STATUE_CHAR,   &wallBackColor,         &statueBackColor,       0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_CRACKING_STATUE,0,           NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),"a marble statue", "The cold marble statue has weathered the years with grace."},
    {STATUE_CHAR,   &wallBackColor,         &statueBackColor,       0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_STATUE_SHATTER,3500,         NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),"a cracking statue",    "Deep cracks ramble down the side of the statue even as you watch."},
    {STATUE_CHAR,   &wallBackColor,         &statueBackColor,       0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_STATUE_SHATTER,0,            NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),"a marble statue", "The cold marble statue has weathered the years with grace."},
    {OMEGA_CHAR,    &wallBackColor,         &floorBackColor,        17, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_PORTAL_ACTIVATE,0,           NO_LIGHT,       (T_OBSTRUCTS_ITEMS), (TM_STAND_IN_TILE | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),  "a stone archway",      "This ancient moss-covered stone archway radiates a strange, alien energy."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_TURRET_EMERGE,0,             NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),            "a stone wall",         "The rough stone wall is firm and unyielding."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_WALL_SHATTER,0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),            "a stone wall",         "The rough stone wall is firm and unyielding."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_DARKENING_FLOOR, 0,          NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "the ground",           ""},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_DARK_FLOOR,  1500,           NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION),                                                                  "the ground",           ""},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              DARKNESS_CLOUD_LIGHT, 0, 0,                                                                                         "the ground",           ""},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY),                      "the ground",           ""},
    {ALTAR_CHAR,    &altarForeColor,        &altarBackColor,        17, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                         "a candle-lit altar",   "a gilded altar is adorned with candles that flicker in the breeze."},
    {GEM_CHAR,      &altarForeColor,        &altarBackColor,        17, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_PROMOTES_WITH_KEY | TM_IS_WIRED | TM_LIST_IN_SIDEBAR),           "a candle-lit altar",   "ornate gilding spirals around a spherical depression in the top of the altar."},
    {ALTAR_CHAR,    &altarForeColor,        &altarBackColor,        17, 0,  DF_NONE,        DF_NONE,    DF_ITEM_CAGE_CLOSE, 0,          CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_WITHOUT_KEY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"a candle-lit altar",   "a cage, open on the bottom, hangs over this altar on a retractable chain."},
    {WALL_CHAR,     &altarBackColor,        &veryDarkGray,          17, 0,  DF_NONE,        DF_NONE,    DF_ITEM_CAGE_OPEN,  0,          CANDLE_LIGHT,   (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_WITH_KEY | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"an iron cage","the missing item must be replaced before you can access the remaining items."},
    {ALTAR_CHAR,    &altarForeColor,        &altarBackColor,        17, 0,  DF_NONE,        DF_NONE,    DF_ALTAR_INERT, 0,              CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_ITEM_PICKUP | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT), "a candle-lit altar",   "a weathered stone altar is adorned with candles that flicker in the breeze."},
    {ALTAR_CHAR,    &altarForeColor,        &altarBackColor,        17, 0,  DF_NONE,        DF_NONE,    DF_ALTAR_RETRACT,0,             CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_ITEM_PICKUP | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT), "a candle-lit altar",   "a weathered stone altar is adorned with candles that flicker in the breeze."},
    {WALL_CHAR,     &altarBackColor,        &veryDarkGray,          17, 0,  DF_NONE,        DF_NONE,    DF_CAGE_DISAPPEARS, 0,          CANDLE_LIGHT,   (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"an iron cage","the cage won't budge. Perhaps there is a way to raise it nearby..."},
    {ALTAR_CHAR,    &altarForeColor,        &pedestalBackColor,     17, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), 0,                                                                   "a stone pedestal",     "elaborate carvings wind around this ancient pedestal."},
    {ALTAR_CHAR,    &floorBackColor,        &veryDarkGray,          17, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "an open cage",         "the interior of the cage is filthy and reeks of decay."},
    {WALL_CHAR,     &gray,                  &darkGray,              17, 0,  DF_NONE,        DF_NONE,    DF_MONSTER_CAGE_OPENS,  0,      NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_WITH_KEY | TM_LIST_IN_SIDEBAR | TM_INTERRUPT_EXPLORATION_WHEN_SEEN),"a locked iron cage","the bars of the cage are firmly set and will not budge."},
    {ALTAR_CHAR,    &bridgeFrontColor,      &bridgeBackColor,       17, 20, DF_COFFIN_BURNS,DF_NONE,    DF_COFFIN_BURSTS,0,             NO_LIGHT,       (T_IS_FLAMMABLE), (TM_IS_WIRED | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),                  "a sealed coffin",      "a coffin made from thick wooden planks rests in a bed of moss."},
    {ALTAR_CHAR,    &black,                 &bridgeBackColor,       17, 20, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),             "an empty coffin",      "an open wooden coffin rests in a bed of moss."},
    
    // traps (part of dungeon layer):
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_POISON_GAS_CLOUD, DF_SHOW_POISON_GAS_TRAP, DF_NONE, 0,   NO_LIGHT,       (T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",           ""},
    {TRAP_CHAR,     &poisonGasColor,        0,                      30, 0,  DF_POISON_GAS_CLOUD, DF_NONE, DF_NONE,      0,              NO_LIGHT,       (T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a caustic gas trap",   "there is a hidden pressure plate in the floor above a reserve of caustic gas."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_POISON_GAS_CLOUD, DF_SHOW_TRAPDOOR,DF_NONE, 0,           NO_LIGHT,       (T_AUTO_DESCENT), (TM_IS_SECRET),                                                                   "the ground",           "you plunge through a hidden trap door!"},
    {CHASM_CHAR,    &chasmForeColor,        &black,                 30, 0,  DF_POISON_GAS_CLOUD, DF_NONE, DF_NONE,      0,              NO_LIGHT,       (T_AUTO_DESCENT), 0,                                                                                "a hole",               "you plunge through a hole in the ground!"},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_NONE,        DF_SHOW_PARALYSIS_GAS_TRAP, DF_NONE, 0,     NO_LIGHT,       (T_IS_DF_TRAP), (TM_IS_SECRET | TM_IS_WIRED),                                                       "the ground",           ""},
    {TRAP_CHAR,     &pink,                  0,                      30, 0,  DF_NONE,        DF_NONE,    DF_NONE,        DF_NONE,        NO_LIGHT,       (T_IS_DF_TRAP), (TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                          "a paralysis trigger",  "there is a hidden pressure plate in the floor."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_DISCOVER_PARALYSIS_VENT, DF_PARALYSIS_VENT_SPEW,0,NO_LIGHT,  (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET | TM_IS_WIRED),                                 "the ground",           ""},
    {VENT_CHAR,     &pink,                  0,                      30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_PARALYSIS_VENT_SPEW,0,       NO_LIGHT,       (0), (TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                     "an inactive gas vent", "A dormant gas vent is connected to a reserve of paralytic gas."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_CONFUSION_GAS_TRAP_CLOUD,DF_SHOW_CONFUSION_GAS_TRAP,DF_NONE,0,NO_LIGHT,  (T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",           ""},
    {TRAP_CHAR,     &confusionGasColor,     0,                      30, 0,  DF_CONFUSION_GAS_TRAP_CLOUD,DF_NONE, DF_NONE,0,             NO_LIGHT,       (T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a confusion trap",     "A hidden pressure plate accompanies a reserve of psychotropic gas."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_FLAMETHROWER,    DF_SHOW_FLAMETHROWER_TRAP, DF_NONE,0,   NO_LIGHT,       (T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",           ""},
    {TRAP_CHAR,     &red,                   0,                      30, 0,  DF_FLAMETHROWER,    DF_NONE,     DF_NONE,   0,              NO_LIGHT,       (T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a fire trap",          "A hidden pressure plate is connected to a crude flamethrower mechanism."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_FLOOD,       DF_SHOW_FLOOD_TRAP, DF_NONE,0,              NO_LIGHT,       (T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",           ""},
    {TRAP_CHAR,     &blue,                  0,                      58, 0,  DF_FLOOD,       DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a flood trap",         "A hidden pressure plate is connected to floodgates in the walls and ceiling."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_NET,         DF_SHOW_NET_TRAP, DF_NONE,  0,              NO_LIGHT,       (T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",           ""},
    {TRAP_CHAR,     &tanColor,              0,                      30, 0,  DF_NET,         DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "a net trap",           "you see netting subtly concealed in the ceiling over a hidden pressure plate."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_AGGRAVATE_TRAP, DF_SHOW_ALARM_TRAP, DF_NONE, 0,          NO_LIGHT,       (T_IS_DF_TRAP), (TM_IS_SECRET),                                                                     "the ground",           ""},
    {TRAP_CHAR,     &gray,                  0,                      30, 0,  DF_AGGRAVATE_TRAP, DF_NONE, DF_NONE,        0,              NO_LIGHT,       (T_IS_DF_TRAP), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                        "an alarm trap",        "a hidden pressure plate is connected to a loud alarm mechanism."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_SHOW_POISON_GAS_VENT, DF_POISON_GAS_VENT_OPEN, 0, NO_LIGHT, (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET | TM_IS_WIRED),                                  "the ground",           ""},
    {VENT_CHAR,     &floorForeColor,        0,                      30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_POISON_GAS_VENT_OPEN,0,      NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),        "an inactive gas vent", "An inactive gas vent is hidden in a crevice in the ground."},
    {VENT_CHAR,     &floorForeColor,        0,                      30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_VENT_SPEW_POISON_GAS,10000,  NO_LIGHT,       0, (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                                     "a gas vent",           "Clouds of caustic gas are wafting out of a hidden vent in the floor."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_SHOW_METHANE_VENT, DF_METHANE_VENT_OPEN,0,NO_LIGHT,      (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_SECRET | TM_IS_WIRED),                                     "the ground",           ""},
    {VENT_CHAR,     &floorForeColor,        0,                      30, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_METHANE_VENT_OPEN,0,         NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),        "an inactive gas vent", "An inactive gas vent is hidden in a crevice in the ground."},
    {VENT_CHAR,     &floorForeColor,        0,                      30, 15, DF_EMBERS,      DF_NONE,    DF_VENT_SPEW_METHANE,5000,      NO_LIGHT,       (T_IS_FLAMMABLE), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                      "a gas vent",           "Clouds of explosive gas are wafting out of a hidden vent in the floor."},
    {VENT_CHAR,     &gray,                  0,                      15, 15, DF_EMBERS,      DF_NONE,    DF_STEAM_PUFF,  250,            NO_LIGHT,       T_OBSTRUCTS_ITEMS, (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                                     "a steam vent",         "A natural crevice in the floor periodically vents scalding gouts of steam."},
    {TRAP_CHAR,     &white,                 &chasmEdgeBackColor,    15, 0,  DF_NONE,        DF_NONE,    DF_MACHINE_PRESSURE_PLATE_USED,0,NO_LIGHT,      (T_IS_DF_TRAP), (TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP | TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),"a pressure plate",        "There is an exposed pressure plate here. A thrown item might trigger it."},
    {TRAP_CHAR,     &darkGray,              &chasmEdgeBackColor,    15, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, (TM_LIST_IN_SIDEBAR),                                                                            "an inactive pressure plate", "This pressure plate has already been depressed."},
    {CHASM_CHAR,    &glyphColor,            0,                      42, 0,  DF_NONE,        DF_NONE,    DF_INACTIVE_GLYPH,0,            GLYPH_LIGHT_DIM,(0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY | TM_VISUALLY_DISTINCT),"a magical glyph",      "A strange glyph, engraved into the floor, flickers with magical light."},
    {CHASM_CHAR,    &glyphColor,            0,                      42, 0,  DF_NONE,        DF_NONE,    DF_ACTIVE_GLYPH,10000,          GLYPH_LIGHT_BRIGHT,(0), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),                                        "a glowing glyph",      "A strange glyph, engraved into the floor, radiates magical light."},
    {DEWAR_CHAR,    &poisonGasColor,        &darkGray,              10, 20, DF_DEWAR_CAUSTIC,DF_NONE,   DF_DEWAR_CAUSTIC,0,             NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of caustic gas", ""},
    {DEWAR_CHAR,    &confusionGasColor,     &darkGray,              10, 20, DF_DEWAR_CONFUSION,DF_NONE, DF_DEWAR_CONFUSION,0,           NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of confusion gas", ""},
    {DEWAR_CHAR,    &pink,                  &darkGray,              10, 20, DF_DEWAR_PARALYSIS,DF_NONE, DF_DEWAR_PARALYSIS,0,           NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of paralytic gas", ""},
    {DEWAR_CHAR,    &methaneColor,          &darkGray,              10, 20, DF_DEWAR_METHANE,DF_NONE,   DF_DEWAR_METHANE,0,             NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_SURFACE_EFFECTS | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_LIST_IN_SIDEBAR | TM_PROMOTES_ON_PLAYER_ENTRY | TM_INVERT_WHEN_HIGHLIGHTED),"a glass dewar of methane gas", ""},
    
    // liquid layer
    {LIQUID_CHAR,   &deepWaterForeColor,    &deepWaterBackColor,    40, 100,DF_STEAM_ACCUMULATION,DF_NONE,DF_NONE,      0,              NO_LIGHT,       (T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_ALLOWS_SUBMERGING | TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE),"the murky waters",    "the current tugs you in all directions."},
    {0,             &shallowWaterForeColor, &shallowWaterBackColor, 55, 0,  DF_STEAM_ACCUMULATION,DF_NONE,DF_NONE,      0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),                              "shallow water",        "the water is cold and reaches your knees."},
    {MUD_CHAR,      &mudForeColor,          &mudBackColor,          55, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_METHANE_GAS_PUFF, 100,       NO_LIGHT,       (0), (TM_STAND_IN_TILE | TM_ALLOWS_SUBMERGING),                                                     "a bog",                "you are knee-deep in thick, foul-smelling mud."},
    {CHASM_CHAR,    &chasmForeColor,        &black,                 40, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_AUTO_DESCENT), (TM_STAND_IN_TILE),                                                               "a chasm",              "you plunge downward into the chasm!"},
    {FLOOR_CHAR,    &white,                 &chasmEdgeBackColor,    80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "the brink of a chasm", "chilly winds blow upward from the stygian depths."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_SPREADABLE_COLLAPSE,0,       NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "the ground",           ""},
    {FLOOR_CHAR,    &white,                 &chasmEdgeBackColor,    45, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_COLLAPSE_SPREADS,2500,       NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION),                                                                  "the crumbling ground", "cracks are appearing in the ground beneath your feet!"},
    {LIQUID_CHAR,   &fireForeColor,         &lavaBackColor,         40, 0,  DF_OBSIDIAN,    DF_NONE,    DF_NONE,        0,              LAVA_LIGHT,     (T_LAVA_INSTA_DEATH), (TM_STAND_IN_TILE | TM_ALLOWS_SUBMERGING),                                    "lava",                 "searing heat rises from the lava."},
    {LIQUID_CHAR,   &fireForeColor,         &lavaBackColor,         40, 0,  DF_OBSIDIAN,    DF_NONE,    DF_RETRACTING_LAVA, 0,          LAVA_LIGHT,     (T_LAVA_INSTA_DEATH), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_ALLOWS_SUBMERGING),"lava","searing heat rises from the lava."},
    {LIQUID_CHAR,   &fireForeColor,         &lavaBackColor,         40, 0,  DF_OBSIDIAN,    DF_NONE,    DF_OBSIDIAN_WITH_STEAM, -1500,  LAVA_LIGHT,     (T_LAVA_INSTA_DEATH), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_ALLOWS_SUBMERGING),       "cooling lava",         "searing heat rises from the lava."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        90, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              SUN_LIGHT,      (0), (TM_STAND_IN_TILE),                                                                            "a patch of sunlight",  "sunlight streams through cracks in the ceiling."},
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        90, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              DARKNESS_PATCH_LIGHT,   (0), 0,                                                                                     "a patch of shadows",   "this area happens to be cloaked in shadows -- perhaps a safe place to hide."},
    {ASH_CHAR,      &brimstoneForeColor,    &brimstoneBackColor,    40, 100,DF_INERT_BRIMSTONE,DF_NONE, DF_INERT_BRIMSTONE, 10,         NO_LIGHT,       (T_IS_FLAMMABLE | T_SPONTANEOUSLY_IGNITES), 0,                                                      "hissing brimstone",    "the jagged brimstone hisses and spits ominously as it crunches under your feet."},
    {ASH_CHAR,      &brimstoneForeColor,    &brimstoneBackColor,    40, 0,  DF_INERT_BRIMSTONE,DF_NONE, DF_ACTIVE_BRIMSTONE, 800,       NO_LIGHT,       (T_SPONTANEOUSLY_IGNITES), 0,                                                                       "hissing brimstone",    "the jagged brimstone hisses and spits ominously as it crunches under your feet."},
    {FLOOR_CHAR,    &darkGray,              &obsidianBackColor,     50, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "the obsidian ground",  "the ground has fused into obsidian."},
    {BRIDGE_CHAR,   &bridgeFrontColor,      &bridgeBackColor,       45, 50, DF_BRIDGE_FIRE, DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "a rickety rope bridge","the rickety rope bridge creaks underfoot."},
    {BRIDGE_CHAR,   &bridgeFrontColor,      &bridgeBackColor,       45, 50, DF_BRIDGE_FALL, DF_NONE,    DF_BRIDGE_FALL, 10000,          NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "a plummeting bridge",  "the bridge is plunging into the chasm before your eyes!"},
    {BRIDGE_CHAR,   &bridgeFrontColor,      &bridgeBackColor,       45, 50, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "a rickety rope bridge","the rickety rope bridge is staked to the edge of the chasm."},
    {FLOOR_CHAR,    &white,                 &chasmEdgeBackColor,    20, 50, DF_BRIDGE_FIRE, DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "a stone bridge",       "the narrow stone bridge winds precariously across the chasm."},
    {0,             &shallowWaterForeColor, &shallowWaterBackColor, 60, 0,  DF_STEAM_ACCUMULATION,DF_NONE,DF_SPREADABLE_WATER,0,        NO_LIGHT,       (0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),   "shallow water",    "the water is cold and reaches your knees."},
    {0,             &shallowWaterForeColor, &shallowWaterBackColor, 60, 0,  DF_STEAM_ACCUMULATION,DF_NONE,DF_WATER_SPREADS,2500,        NO_LIGHT,       (0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING), "shallow water",        "the water is cold and reaches your knees."},
    {MUD_CHAR,      &mudForeColor,          &mudBackColor,          55, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_MUD_ACTIVATE,0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_ALLOWS_SUBMERGING),          "a bog",                "you are knee-deep in thick, foul-smelling mud."},
        
    // surface layer
    {CHASM_CHAR,    &chasmForeColor,        &black,                 9,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_HOLE_DRAIN,  -1000,          NO_LIGHT,       (T_AUTO_DESCENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "a hole",               "you plunge downward into the hole!"},
    {CHASM_CHAR,    &chasmForeColor,        &black,                 9,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_HOLE_DRAIN,  -1000,          DESCENT_LIGHT,  (T_AUTO_DESCENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "a hole",               "you plunge downward into the hole!"},
    {FLOOR_CHAR,    &white,                 &chasmEdgeBackColor,    50, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        -500,           NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION),                                                                  "translucent ground",   "chilly gusts of air blow upward through the translucent floor."},
    {LIQUID_CHAR,   &deepWaterForeColor,    &deepWaterBackColor,    41, 100,DF_STEAM_ACCUMULATION,DF_NONE,DF_FLOOD_DRAIN,-200,          NO_LIGHT,       (T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING), "sloshing water", "roiling water floods the room."},
    {0,             &shallowWaterForeColor, &shallowWaterBackColor, 50, 0,  DF_STEAM_ACCUMULATION,DF_NONE,DF_PUDDLE,    -100,           NO_LIGHT,       (0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING), "shallow water",        "knee-deep water drains slowly into holes in the floor."},
    {GRASS_CHAR,    &grassColor,            0,                      60, 15, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "grass-like fungus",    "grass-like fungus crunches underfoot."},
    {GRASS_CHAR,    &deadGrassColor,        0,                      60, 40, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "withered fungus",      "dead fungus covers the ground."},
    {GRASS_CHAR,    &grayFungusColor,       0,                      51, 10, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "withered fungus",      "groping tendrils of pale fungus rise from the muck."},
    {GRASS_CHAR,    &fungusColor,           0,                      60, 10, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              FUNGUS_LIGHT,   (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "luminescent fungus",   "luminescent fungus casts a pale, eerie glow."},
    {GRASS_CHAR,    &lichenColor,           0,                      60, 50, DF_PLAIN_FIRE,  DF_NONE,    DF_LICHEN_GROW, 10000,          NO_LIGHT,       (T_CAUSES_POISON | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                "deadly lichen",        "venomous barbs cover the quivering tendrils of this fast-growing lichen."},
    {GRASS_CHAR,    &hayColor,              &refuseBackColor,       57, 50, DF_STENCH_BURN, DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "filthy hay",           "a pile of hay, matted with filth, has been arranged here as a makeshift bed."},
    {FLOOR_CHAR,    &humanBloodColor,       0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pool of blood",      "the floor is splattered with blood."},
    {FLOOR_CHAR,    &insectBloodColor,      0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pool of green blood", "the floor is splattered with green blood."},
    {FLOOR_CHAR,    &poisonGasColor,        0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pool of purple blood", "the floor is splattered with purple blood."},
    {FLOOR_CHAR,    &acidBackColor,         0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "the acid-flecked ground", "the floor is splattered with acid."},
    {FLOOR_CHAR,    &vomitColor,            0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a puddle of vomit",    "the floor is caked with vomit."},
    {FLOOR_CHAR,    &urineColor,            0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        100,            NO_LIGHT,       (0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                               "a puddle of urine",    "a puddle of urine covers the ground."},
    {FLOOR_CHAR,    &white,                 0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              UNICORN_POOP_LIGHT,(0), (TM_STAND_IN_TILE),                                                                         "unicorn poop",         "a pile of lavender-scented unicorn poop sparkles with rainbow light."},
    {FLOOR_CHAR,    &wormColor,             0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pool of worm entrails", "worm viscera cover the ground."},
    {ASH_CHAR,      &ashForeColor,          0,                      80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pile of ashes",      "charcoal and ash crunch underfoot."},
    {ASH_CHAR,      &ashForeColor,          0,                      87, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "burned carpet",        "the carpet has been scorched by an ancient fire."},
    {FLOOR_CHAR,    &shallowWaterBackColor, 0,                      80, 20, DF_NONE,        DF_NONE,    DF_NONE,        100,            NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                  "a puddle of water",    "a puddle of water covers the ground."},
    {BONES_CHAR,    &bonesForeColor,        0,                      70, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pile of bones",      "unidentifiable bones, yellowed with age, litter the ground."},
    {BONES_CHAR,    &gray,                  0,                      70, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pile of rubble",     "rocky rubble covers the ground."},
    {BONES_CHAR,    &mudBackColor,          &refuseBackColor,       50, 20, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "a pile of filthy effects","primitive tools, carvings and trinkets are strewn about the area."},
    {BONES_CHAR,    &white,                 0,                      70, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (TM_STAND_IN_TILE),                                                                            "shattered glass",      "jagged chunks of glass from the broken dewar litter the ground."},
    {FLOOR_CHAR,    &ectoplasmColor,        0,                      70, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              ECTOPLASM_LIGHT,(0), (TM_STAND_IN_TILE),                                                                            "ectoplasmic residue",  "a thick, glowing substance has congealed on the ground."},
    {ASH_CHAR,      &fireForeColor,         0,                      70, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_ASH,         300,            EMBER_LIGHT,    (0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                               "sputtering embers",    "sputtering embers cover the ground."},
    {WEB_CHAR,      &white,                 0,                      19, 100,DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_ENTANGLES | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),"a spiderweb",       "thick, sticky spiderwebs fill the area."},
    {WEB_CHAR,      &brown,                 0,                      19, 40, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_ENTANGLES | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),"a net",             "a dense tangle of netting fills the area."},
    {FOLIAGE_CHAR,  &foliageColor,          0,                      45, 15, DF_PLAIN_FIRE,  DF_NONE,    DF_TRAMPLED_FOLIAGE, 0,         NO_LIGHT,       (T_OBSTRUCTS_VISION | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP), "dense foliage",   "dense foliage fills the area, thriving on what sunlight trickles in."},
    {FOLIAGE_CHAR,  &deadFoliageColor,      0,                      45, 80, DF_PLAIN_FIRE,  DF_NONE,    DF_SMALL_DEAD_GRASS, 0,         NO_LIGHT,       (T_OBSTRUCTS_VISION | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP), "dead foliage",    "the decaying husk of a fungal growth fills the area."},
    {TRAMPLED_FOLIAGE_CHAR,&foliageColor,   0,                      60, 15, DF_PLAIN_FIRE,  DF_NONE,    DF_FOLIAGE_REGROW, 100,         NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "trampled foliage",     "dense foliage fills the area, thriving on what sunlight trickles in."},
    {FOLIAGE_CHAR,  &fungusForestLightColor,0,                      45, 15, DF_PLAIN_FIRE,  DF_NONE,    DF_TRAMPLED_FUNGUS_FOREST, 0,   FUNGUS_FOREST_LIGHT,(T_OBSTRUCTS_VISION | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP),"a luminescent fungal forest", "luminescent fungal growth fills the area, groping upward from the rich soil."},
    {TRAMPLED_FOLIAGE_CHAR,&fungusForestLightColor,0,               60, 15, DF_PLAIN_FIRE,  DF_NONE,    DF_FUNGUS_FOREST_REGROW, 100,   FUNGUS_LIGHT,   (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "trampled fungal foliage", "luminescent fungal growth fills the area, groping upward from the rich soil."},
    {WALL_CHAR,     &forceFieldColor,       &forceFieldColor,       0,  0,  DF_NONE,        DF_NONE,    DF_FORCEFIELD_MELT, -200,       FORCEFIELD_LIGHT, (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_GAS | T_OBSTRUCTS_DIAGONAL_MOVEMENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_STEP),       "a green crystal",      "The translucent green crystal is melting away in front of your eyes."},
    {WALL_CHAR,     &black,                 &forceFieldColor,       0,  0,  DF_NONE,        DF_NONE,    DF_NONE,        -10000,         FORCEFIELD_LIGHT, (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_GAS | T_OBSTRUCTS_DIAGONAL_MOVEMENT), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),     "a dissolving crystal",     "The translucent green crystal is melting away in front of your eyes."},
    {CHASM_CHAR,    &sacredGlyphColor,      0,                      57, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              SACRED_GLYPH_LIGHT, (T_SACRED), 0,                                                                                  "a sacred glyph",       "a sacred glyph adorns the floor, glowing with a powerful warding enchantment."},
    {CHAIN_TOP_LEFT,&gray,                  0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the ceiling."},
    {CHAIN_BOTTOM_RIGHT, &gray,             0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the floor."},
    {CHAIN_TOP_RIGHT, &gray,                0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the ceiling."},
    {CHAIN_BOTTOM_LEFT, &gray,              0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the floor."},
    {CHAIN_TOP,     &gray,                  0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the wall."},
    {CHAIN_BOTTOM,  &gray,                  0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the wall."},
    {CHAIN_LEFT,    &gray,                  0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the wall."},
    {CHAIN_RIGHT,   &gray,                  0,                      20, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, 0,                                                                                               "an iron manacle",      "a thick iron manacle is anchored to the wall."},
    {0,             0,                      0,                      1,  0,  DF_NONE,        DF_NONE,    DF_NONE,        10000,          PORTAL_ACTIVATE_LIGHT,(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                         "blinding light",       "blinding light streams out of the archway."},
    {0,             0,                      0,                      100,0,  DF_NONE,        DF_NONE,    DF_NONE,        10000,          GLYPH_LIGHT_BRIGHT,(0), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION),                                            "a red glow",           "a red glow fills the area."},
    
    // fire tiles
    {FIRE_CHAR,     &fireForeColor,         0,                      10, 0,  DF_NONE,        DF_NONE,    DF_EMBERS,      500,            FIRE_LIGHT,     (T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),                "billowing flames",     "flames billow upward."},
    {FIRE_CHAR,     &fireForeColor,         0,                      10, 0,  DF_NONE,        DF_NONE,    DF_NONE,        2500,           BRIMSTONE_FIRE_LIGHT,(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),           "sulfurous flames",     "sulfurous flames leap from the unstable bed of brimstone."},
    {FIRE_CHAR,     &fireForeColor,         0,                      10, 0,  DF_NONE,        DF_NONE,    DF_OBSIDIAN,    5000,           FIRE_LIGHT,     (T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),                "clouds of infernal flame", "billowing infernal flames eat at the floor."},
    {FIRE_CHAR,     &fireForeColor,         0,                      10, 0,  DF_NONE,        DF_NONE,    DF_NONE,        8000,           FIRE_LIGHT,     (T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),                "a cloud of burning gas", "burning gas fills the air with flame."},
    {FIRE_CHAR,     &yellow,                0,                      10, 0,  DF_NONE,        DF_NONE,    DF_NONE,        10000,          EXPLOSION_LIGHT,(T_IS_FIRE | T_CAUSES_EXPLOSIVE_DAMAGE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT), "a violent explosion", "the force of the explosion slams into you."},
    {FIRE_CHAR,     &white,                 0,                      10, 0,  DF_NONE,        DF_NONE,    DF_NONE,        10000,          INCENDIARY_DART_LIGHT ,(T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),         "a flash of fire",      "flames burst out of the incendiary dart."},
    {FIRE_CHAR,     &white,                 0,                      10, 0,  DF_NONE,        DF_NONE,    DF_EMBERS,      3000,           FIRE_LIGHT,     (T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),                "crackling flames",     "crackling flames rise from the blackened item."},
    {FIRE_CHAR,     &white,                 0,                      10, 0,  DF_NONE,        DF_NONE,    DF_EMBERS,      3000,           FIRE_LIGHT,     (T_IS_FIRE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT),                "greasy flames",        "greasy flames rise from the corpse."},
    
    // gas layer
    {   ' ',        0,                      &poisonGasColor,        35, 100,DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE | T_CAUSES_DAMAGE), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES),                         "a cloud of caustic gas", "you can feel the purple gas eating at your flesh."},
    {   ' ',        0,                      &confusionGasColor,     35, 100,DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              CONFUSION_GAS_LIGHT,(T_IS_FLAMMABLE | T_CAUSES_CONFUSION), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),          "a cloud of confusion gas", "the rainbow-colored gas tickles your brain."},
    {   ' ',        0,                      &vomitColor,            35, 100,DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE | T_CAUSES_NAUSEA), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),                 "a cloud of putrescence", "the stench of rotting flesh is overpowering."},
    {   ' ',        0,                      &vomitColor,            35, 0,  DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_CAUSES_NAUSEA), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),                                  "a cloud of putrid smoke", "you retch violently at the smell of the greasy smoke."},
    {   ' ',        0,                      &pink,                  35, 100,DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE | T_CAUSES_PARALYSIS), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),              "a cloud of paralytic gas", "the pale gas causes your muscles to stiffen."},
    {   ' ',        0,                      &methaneColor,          35, 100,DF_GAS_FIRE,    DF_NONE,    DF_EXPLOSION_FIRE, 0,           NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_EXPLOSIVE_PROMOTE),                                        "a cloud of explosive gas", "the smell of explosive swamp gas fills the air."},
    {   ' ',        0,                      &white,                 35, 0,  DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_CAUSES_DAMAGE), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),                                  "a cloud of scalding steam", "scalding steam fills the air!"},
    {   ' ',        0,                      0,                      35, 0,  DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              DARKNESS_CLOUD_LIGHT,   (0), (TM_STAND_IN_TILE),                                                                    "a cloud of supernatural darkness", "everything is obscured by an aura of supernatural darkness."},
    {   ' ',        0,                      &darkRed,               35, 0,  DF_GAS_FIRE,    DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_CAUSES_HEALING), (TM_STAND_IN_TILE | TM_GAS_DISSIPATES_QUICKLY),                                 "a cloud of healing spores", "bloodwort spores, renowned for their healing properties, fill the air."},
    
    // bloodwort pods
    {FOLIAGE_CHAR,  &bloodflowerForeColor,  &bloodflowerBackColor,  10, 20, DF_PLAIN_FIRE,  DF_NONE,    DF_BLOODFLOWER_PODS_GROW, 100,  NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FLAMMABLE), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT), "a bloodwort stalk", "this spindly plant grows seed pods famous for their healing properties."},
    {GOLD_CHAR,     &bloodflowerPodForeColor, 0,                    11, 20, DF_BLOODFLOWER_POD_BURST,DF_NONE,DF_BLOODFLOWER_POD_BURST,0,NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_PLAYER_ENTRY | TM_VISUALLY_DISTINCT | TM_INVERT_WHEN_HIGHLIGHTED), "a bloodwort pod", "the bloodwort seed pod bursts, releasing a cloud of healing spores."},
    
    // shrine accoutrements
    {BRIDGE_CHAR,   &black,                 &bedrollBackColor,      57, 50, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "an abandoned bedroll", "a bedroll lies in the corner, disintegrating with age."},
    
    // algae
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_ALGAE_1,     100,            NO_LIGHT,       0, 0,                                                                                               "the ground",           ""},
    {LIQUID_CHAR,   &deepWaterForeColor,    &deepWaterBackColor,    40, 100,DF_STEAM_ACCUMULATION,DF_NONE,DF_ALGAE_1,   500,            LUMINESCENT_ALGAE_BLUE_LIGHT,(T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING),  "luminescent waters",   "blooming algae fills the waters with a swirling luminescence."},
    {LIQUID_CHAR,   &deepWaterForeColor,    &deepWaterBackColor,    39, 100,DF_STEAM_ACCUMULATION,DF_NONE,DF_ALGAE_REVERT,300,          LUMINESCENT_ALGAE_GREEN_LIGHT,(T_IS_FLAMMABLE | T_IS_DEEP_WATER), (TM_STAND_IN_TILE | TM_EXTINGUISHES_FIRE | TM_ALLOWS_SUBMERGING), "luminescent waters",   "blooming algae fills the waters with a swirling luminescence."},
    
    // ancient spirit terrain
    {VINE_CHAR,     &lichenColor,           0,                      19, 100,DF_PLAIN_FIRE,  DF_NONE,    DF_ANCIENT_SPIRIT_GRASS,1000,   NO_LIGHT,       (T_ENTANGLES | T_CAUSES_DAMAGE | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_VISUALLY_DISTINCT | TM_PROMOTES_ON_PLAYER_ENTRY),"thorned vines",       "thorned vines make a rustling noise as they quiver restlessly."},
    {GRASS_CHAR,    &grassColor,            0,                      60, 15, DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_STAND_IN_TILE),                                                               "a tuft of grass",      "tufts of lush grass have improbably pushed upward through the stone ground."},
    
    // Yendor amulet floor tile
    {FLOOR_CHAR,    &floorForeColor,        &floorBackColor,        95, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       0, (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_PROMOTES_ON_ITEM_PICKUP),                         "the ground",           ""},
    
    // commutation device
    {ALTAR_CHAR,    &altarForeColor,        &greenAltarBackColor,   17, 0,  DF_NONE,        DF_NONE,    DF_ALTAR_COMMUTE,0,             NO_LIGHT,       (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_SWAP_ENCHANTS_ACTIVATION | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),    "a commutation altar",  "crude diagrams on this altar and its twin invite you to place items upon them."},
    {ALTAR_CHAR,    &black,                 &greenAltarBackColor,   17, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                         "a scorched altar",     "scorch marks cover the surface of the altar, but it is cold to the touch."},
    {'+',           &veryDarkGray,          0,                      45, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_INERT_PIPE,  0,              CONFUSION_GAS_LIGHT, (0), (TM_IS_WIRED | TM_VANISHES_UPON_PROMOTION),                                               "glowing glass pipes",  "glass pipes are set into the floor and emit a soft glow of shifting color."},
    {'+',           &black,                 0,                      45, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (0), (0),                                                                                           "charred glass pipes",  "the inside of the glass pipes are charred."},

    // resurrection altar
    {ALTAR_CHAR,    &altarForeColor,        &goldAltarBackColor,    17, 0,  DF_NONE,        DF_NONE,    DF_ALTAR_RESURRECT,0,           CANDLE_LIGHT,   (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_IS_WIRED | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),           "a resurrection altar", "the souls of the dead surround you. A deceased ally might be called back."},
    {ALTAR_CHAR,    &black,                 &goldAltarBackColor,    16, 0,  DF_NONE,        DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_SURFACE_EFFECTS), (TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT),                         "a scorched altar",     "scorch marks cover the surface of the altar, but it is cold to the touch."},
    {0,             0,                      0,                      95, 0,  DF_NONE,        DF_NONE,    DF_NONE,         0,              NO_LIGHT,       (0), (TM_IS_WIRED | TM_PROMOTES_ON_PLAYER_ENTRY),                                                   "the ground",           ""},
    
    // doorway statues
    {STATUE_CHAR,   &wallBackColor,         &statueBackColor,       0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_CONNECTS_LEVEL),  "a marble statue",  "The cold marble statue has weathered the years with grace."},
    {STATUE_CHAR,   &wallBackColor,         &statueBackColor,       0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_CRACKING_STATUE,0,           NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_CONNECTS_LEVEL),"a marble statue", "The cold marble statue has weathered the years with grace."},
    
    // extensible stone bridge    
    {CHASM_CHAR,    &chasmForeColor,        &black,                 40, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_AUTO_DESCENT), (TM_STAND_IN_TILE),                                                               "a chasm",              "you plunge downward into the chasm!"},
    {FLOOR_CHAR,    &white,                 &chasmEdgeBackColor,    40, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_BRIDGE_ACTIVATE,6000,        NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION),                                                                  "a stone bridge",       "the narrow stone bridge is extending across the chasm."},
    {FLOOR_CHAR,    &white,                 &chasmEdgeBackColor,    80, 0,  DF_PLAIN_FIRE,  DF_NONE,    DF_BRIDGE_ACTIVATE_ANNOUNCE,0,  NO_LIGHT,       (0), (TM_IS_WIRED),                                                                                 "the brink of a chasm", "chilly winds blow upward from the stygian depths."},
    
    // rat trap
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_WALL_CRACK,  0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),            "a stone wall",         "The rough stone wall is firm and unyielding."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_WALL_SHATTER,500,            NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_LIST_IN_SIDEBAR),     "a cracking wall",      "Cracks are running ominously across the base of this rough stone wall."},
    
    // electric crystals
    {ELECTRIC_CRYSTAL_CHAR, &wallCrystalColor, &graniteBackColor,   0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_ELECTRIC_CRYSTAL_ON, 0,      NO_LIGHT,       (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_PROMOTES_ON_ELECTRICITY | TM_IS_CIRCUIT_BREAKER | TM_IS_WIRED | TM_LIST_IN_SIDEBAR), "a darkened crystal globe", "A slight electric shock startles you when you touch the inert crystal globe."},
    {ELECTRIC_CRYSTAL_CHAR, &white,         &wallCrystalColor,      0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              CRYSTAL_WALL_LIGHT,(T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_OBSTRUCTS_GAS | T_OBSTRUCTS_SURFACE_EFFECTS), (TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR), "a shining crystal globe", "Crackling light streams out of the crystal globe."},
    {LEVER_CHAR,    &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_TURRET_LEVER,0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_PROMOTES_ON_PLAYER_ENTRY | TM_LIST_IN_SIDEBAR | TM_VISUALLY_DISTINCT | TM_INVERT_WHEN_HIGHLIGHTED),"a lever", "The lever moves."},
    
    // worm tunnels
    {0,             0,                      0,                      100,0,  DF_NONE,        DF_NONE,    DF_WORM_TUNNEL_MARKER_ACTIVE,0, NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED),                                                    "",                     ""},
    {0,             0,                      0,                      100,0,  DF_NONE,        DF_NONE,    DF_GRANITE_CRUMBLES,-2000,      NO_LIGHT,       (0), (TM_VANISHES_UPON_PROMOTION),                                                                  "a rough granite wall", "The granite is split open with splinters of rock jutting out at odd angles."},
    {WALL_CHAR,     &wallForeColor,         &wallBackColor,         0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_WALL_SHATTER,0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE | TM_VANISHES_UPON_PROMOTION | TM_IS_WIRED | TM_CONNECTS_LEVEL),"a stone wall", "The rough stone wall is firm and unyielding."},
    
    // zombie crypt
    {FIRE_CHAR,     &fireForeColor,         &statueBackColor,       0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              BURNING_CREATURE_LIGHT, (T_OBSTRUCTS_PASSABILITY | T_OBSTRUCTS_ITEMS | T_IS_FIRE), (TM_STAND_IN_TILE | TM_LIST_IN_SIDEBAR),"a ceremonial brazier",      "The ancient brazier smolders with a deep crimson flame."},
    
    // goblin warren
    {FLOOR_CHAR,    &mudBackColor,          &refuseBackColor,       85, 0,  DF_STENCH_SMOLDER,DF_NONE,  DF_NONE,        0,              NO_LIGHT,       (T_IS_FLAMMABLE), (TM_VANISHES_UPON_PROMOTION),                                                     "the mud floor",        "Rotting animal matter has been ground into the mud floor; the stench is awful."},
    {WALL_CHAR,     &mudWallForeColor,      &mudWallBackColor,      0,  0,  DF_PLAIN_FIRE,  DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_EVERYTHING), (TM_STAND_IN_TILE),                                                       "a mud-covered wall",   "A malodorous layer of clay and fecal matter coats the wall."},
    {OMEGA_CHAR,    &mudWallForeColor,      &refuseBackColor,       25, 50, DF_EMBERS,      DF_NONE,    DF_NONE,        0,              NO_LIGHT,       (T_OBSTRUCTS_VISION | T_OBSTRUCTS_GAS | T_IS_FLAMMABLE), (TM_STAND_IN_TILE | TM_VISUALLY_DISTINCT), "hanging animal skins", "you push through the animal skins that hang across the threshold."},
};

#pragma mark Dungeon Feature definitions

// Features in the gas layer use the startprob as volume, ignore probdecr, and spawn in only a single point.
// Intercepts and slopes are in units of 0.01.
dungeonFeature dungeonFeatureCatalog[NUMBER_DUNGEON_FEATURES] = {
    // tileType                 layer       start   decr    fl  txt  flare   fCol fRad  propTerrain subseqDF
    {NOTHING}, // nothing
    {GRANITE,                   DUNGEON,    80,     70,     DFF_CLEAR_OTHER_TERRAIN},
    {CRYSTAL_WALL,              DUNGEON,    200,    50,     DFF_CLEAR_OTHER_TERRAIN},
    {LUMINESCENT_FUNGUS,        SURFACE,    60,     8,      DFF_BLOCKED_BY_OTHER_LAYERS},
    {GRASS,                     SURFACE,    75,     5,      DFF_BLOCKED_BY_OTHER_LAYERS},
    {DEAD_GRASS,                SURFACE,    75,     5,      DFF_BLOCKED_BY_OTHER_LAYERS,    "", NO_LIGHT,  0,  0,      NOTHING,          DF_DEAD_FOLIAGE},
    {BONES,                     SURFACE,    75,     23,     0},
    {RUBBLE,                    SURFACE,    45,     23,     0},
    {FOLIAGE,                   SURFACE,    100,    33,     (DFF_BLOCKED_BY_OTHER_LAYERS)},
    {FUNGUS_FOREST,             SURFACE,    100,    45,     (DFF_BLOCKED_BY_OTHER_LAYERS)},
    {DEAD_FOLIAGE,              SURFACE,    50,     30,     (DFF_BLOCKED_BY_OTHER_LAYERS)},
    
    // misc. liquids
    {SUNLIGHT_POOL,             LIQUID,     65,     6,      0},
    {DARKNESS_PATCH,            LIQUID,     65,     11,     0},
    
    // Dungeon features spawned during gameplay:
    
    // revealed secrets
    {DOOR,                      DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    {GAS_TRAP_POISON,           DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    {GAS_TRAP_PARALYSIS,        DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    {CHASM_EDGE,                LIQUID,     100,    100,    0, "", GENERIC_FLASH_LIGHT},
    {TRAP_DOOR,                 LIQUID,     0,      0,      DFF_CLEAR_OTHER_TERRAIN, "", GENERIC_FLASH_LIGHT, 0, 0, NOTHING, DF_SHOW_TRAPDOOR_HALO},
    {GAS_TRAP_CONFUSION,        DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    {FLAMETHROWER,              DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    {FLOOD_TRAP,                DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    {NET_TRAP,                  DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    {ALARM_TRAP,                DUNGEON,    0,      0,      0, "", GENERIC_FLASH_LIGHT},
    
    // bloods
    // Start probability is actually a percentage for bloods.
    // Base probability is 15 + (damage * 2/3), and then take the given percentage of that.
    // If it's a gas, we multiply the base by an additional 100.
    // Thus to get a starting gas volume of a poison potion (1000), with a hit for 10 damage, use a starting probability of 48.
    {RED_BLOOD,                 SURFACE,    100,    25,     0},
    {GREEN_BLOOD,               SURFACE,    100,    25,     0},
    {PURPLE_BLOOD,              SURFACE,    100,    25,     0},
    {WORM_BLOOD,                SURFACE,    100,    25,     0},
    {ACID_SPLATTER,             SURFACE,    200,    25,     0},
    {ASH,                       SURFACE,    50,     25,     0},
    {EMBERS,                    SURFACE,    125,    25,     0},
    {ECTOPLASM,                 SURFACE,    110,    25,     0},
    {RUBBLE,                    SURFACE,    33,     25,     0},
    {ROT_GAS,                   GAS,        12,     0,      0},
    
    // monster effects
    {VOMIT,                     SURFACE,    30,     10,     0},
    {POISON_GAS,                GAS,        2000,   0,      0},
    {GAS_EXPLOSION,             SURFACE,    350,    100,    0,  "", EXPLOSION_FLARE_LIGHT},
    {RED_BLOOD,                 SURFACE,    150,    30,     0},
    {FLAMEDANCER_FIRE,          SURFACE,    200,    75,     0},
    
    // mutation effects
    {GAS_EXPLOSION,             SURFACE,    350,    100,    0,  "The corpse detonates with terrifying force!", EXPLOSION_FLARE_LIGHT},
    {LICHEN,                    SURFACE,    70,     60,     0,  "Poisonous spores burst from the corpse!"},
    
    // misc
    {NOTHING,                   GAS,        0,      0,      DFF_EVACUATE_CREATURES_FIRST},
    {ROT_GAS,                   GAS,        15,     0,      0},
    {STEAM,                     GAS,        325,    0,      0},
    {STEAM,                     GAS,        15,     0,      0},
    {METHANE_GAS,               GAS,        2,      0,      0},
    {EMBERS,                    SURFACE,    0,      0,      0},
    {URINE,                     SURFACE,    65,     25,     0},
    {UNICORN_POOP,              SURFACE,    65,     40,     0},
    {PUDDLE,                    SURFACE,    13,     25,     0},
    {ASH,                       SURFACE,    0,      0,      0},
    {ECTOPLASM,                 SURFACE,    0,      0,      0},
    {FORCEFIELD,                SURFACE,    100,    50,     0},
    {FORCEFIELD_MELT,           SURFACE,    0,      0,      0},
    {SACRED_GLYPH,              LIQUID,     100,    100,    0,  "", EMPOWERMENT_LIGHT},
    {LICHEN,                    SURFACE,    2,      100,    (DFF_BLOCKED_BY_OTHER_LAYERS)}, // Lichen won't spread through lava.
    {RUBBLE,                    SURFACE,    45,     23,     (DFF_ACTIVATE_DORMANT_MONSTER)},
    {RUBBLE,                    SURFACE,    0,      0,      (DFF_ACTIVATE_DORMANT_MONSTER)},
    
    {SPIDERWEB,                 SURFACE,    15,     12,     0},
    {SPIDERWEB,                 SURFACE,    100,    39,     0},
    
    {ANCIENT_SPIRIT_VINES,      SURFACE,    75,     70,     0},
    {ANCIENT_SPIRIT_GRASS,      SURFACE,    50,     47,     0},
    
    // foliage
    {TRAMPLED_FOLIAGE,          SURFACE,    0,      0,      0},
    {DEAD_GRASS,                SURFACE,    75,     75,     0},
    {FOLIAGE,                   SURFACE,    0,      0,      (DFF_BLOCKED_BY_OTHER_LAYERS)},
    {TRAMPLED_FUNGUS_FOREST,    SURFACE,    0,      0,      0},
    {FUNGUS_FOREST,             SURFACE,    0,      0,      (DFF_BLOCKED_BY_OTHER_LAYERS)},
    
    // brimstone
    {ACTIVE_BRIMSTONE,          LIQUID,     0,      0,      0},
    {INERT_BRIMSTONE,           LIQUID,     0,      0,      0,  "", NO_LIGHT,  0,  0,      NOTHING,          DF_BRIMSTONE_FIRE},
    
    // bloodwort
    {BLOODFLOWER_POD,           SURFACE,    60,     60,     DFF_EVACUATE_CREATURES_FIRST},
    {BLOODFLOWER_POD,           SURFACE,    10,     10,     DFF_EVACUATE_CREATURES_FIRST},
    {HEALING_CLOUD,             GAS,        350,    0,      0},
    
    // dewars
    {POISON_GAS,                GAS,        20000,  0,      0, "the dewar shatters and pressurized caustic gas explodes outward!", NO_LIGHT, &poisonGasColor, 4, NOTHING, DF_DEWAR_GLASS},
    {CONFUSION_GAS,             GAS,        20000,  0,      0, "the dewar shatters and pressurized confusion gas explodes outward!", NO_LIGHT, &confusionGasColor, 4, NOTHING, DF_DEWAR_GLASS},
    {PARALYSIS_GAS,             GAS,        20000,  0,      0, "the dewar shatters and pressurized paralytic gas explodes outward!", NO_LIGHT, &pink, 4, NOTHING, DF_DEWAR_GLASS},
    {METHANE_GAS,               GAS,        20000,  0,      0, "the dewar shatters and pressurized methane gas explodes outward!", NO_LIGHT, &methaneColor, 4, NOTHING, DF_DEWAR_GLASS},
    {BROKEN_GLASS,              SURFACE,    100,    70,     0},
    {CARPET,                    DUNGEON,    120,    20,     0},
    
    // algae
    {DEEP_WATER_ALGAE_WELL,     DUNGEON,    0,      0,      DFF_SUPERPRIORITY},
    {DEEP_WATER_ALGAE_1,        LIQUID,     50,     100,    0,  "", NO_LIGHT,  0,   0,     DEEP_WATER, DF_ALGAE_2},
    {DEEP_WATER_ALGAE_2,        LIQUID,     0,      0,      0},
    {DEEP_WATER,                LIQUID,     0,      0,      DFF_SUPERPRIORITY},
    
    // doors, item cages, altars, glyphs, guardians -- reusable machine components
    {OPEN_DOOR,                 DUNGEON,    0,      0,      0},
    {DOOR,                      DUNGEON,    0,      0,      0},
    {OPEN_IRON_DOOR_INERT,      DUNGEON,    0,      0,      0,  "", GENERIC_FLASH_LIGHT},
    {ALTAR_CAGE_OPEN,           DUNGEON,    0,      0,      0,  "the cages lift off of the altars as you approach.", GENERIC_FLASH_LIGHT},
    {ALTAR_CAGE_CLOSED,         DUNGEON,    0,      0,      (DFF_EVACUATE_CREATURES_FIRST), "the cages lower to cover the altars.", GENERIC_FLASH_LIGHT},
    {ALTAR_INERT,               DUNGEON,    0,      0,      0},
    {FLOOR_FLOODABLE,           DUNGEON,    0,      0,      0,  "the altar retracts into the ground with a grinding sound.", GENERIC_FLASH_LIGHT},
    {PORTAL_LIGHT,              SURFACE,    0,      0,      (DFF_EVACUATE_CREATURES_FIRST | DFF_ACTIVATE_DORMANT_MONSTER), "the archway flashes, and you catch a glimpse of another world!"},
    {MACHINE_GLYPH_INACTIVE,    DUNGEON,    0,      0,      0},
    {MACHINE_GLYPH,             DUNGEON,    0,      0,      0},
    {GUARDIAN_GLOW,             SURFACE,    0,      0,      0,  ""},
    {GUARDIAN_GLOW,             SURFACE,    0,      0,      0,  "the glyph beneath you glows, and the guardians take a step!"},
    {GUARDIAN_GLOW,             SURFACE,    0,      0,      0,  "the mirrored totem flashes, reflecting the red glow of the glyph beneath you."},
    {MACHINE_GLYPH,             DUNGEON,    200,    95,     DFF_BLOCKED_BY_OTHER_LAYERS},
    {WALL_LEVER,                DUNGEON,    0,      0,      0,  "you notice a lever hidden behind a loose stone in the wall.", GENERIC_FLASH_LIGHT},
    {WALL_LEVER_PULLED,         DUNGEON,    0,      0,      0},
    {WALL_LEVER_HIDDEN,         DUNGEON,    0,      0,      0},
    
    {BRIDGE_FALLING,            LIQUID,     200,    100,    0, "", NO_LIGHT, 0, 0, BRIDGE},
    {CHASM,                     LIQUID,     0,      0,      0, "", GENERIC_FLASH_LIGHT, 0, 0, NOTHING, DF_BRIDGE_FALL_PREP},
    
    // fire
    {PLAIN_FIRE,                SURFACE,    0,      0,      0},
    {GAS_FIRE,                  SURFACE,    0,      0,      0},
    {GAS_EXPLOSION,             SURFACE,    60,     17,     0},
    {DART_EXPLOSION,            SURFACE,    0,      0,      0},
    {BRIMSTONE_FIRE,            SURFACE,    0,      0,      0},
    {NOTHING,                   NO_LAYER,   0,      0,      0,  "the rope bridge snaps from the heat and plunges into the chasm!", FALLEN_TORCH_FLASH_LIGHT,    0,  0,      NOTHING,      DF_BRIDGE_FALL},
    {PLAIN_FIRE,                SURFACE,    100,    37,     0},
    {EMBERS,                    SURFACE,    0,      0,      0},
    {EMBERS,                    SURFACE,    100,    94,     0},
    {OBSIDIAN,                  SURFACE,    0,      0,      0},
    {ITEM_FIRE,                 SURFACE,    0,      0,      0,  "", FALLEN_TORCH_FLASH_LIGHT},
    {CREATURE_FIRE,             SURFACE,    0,      0,      0,  "", FALLEN_TORCH_FLASH_LIGHT},
    
    {FLOOD_WATER_SHALLOW,       SURFACE,    225,    37,     0,  "", NO_LIGHT,  0,  0,      NOTHING,          DF_FLOOD_2},
    {FLOOD_WATER_DEEP,          SURFACE,    175,    37,     0,  "the area is flooded as water rises through imperceptible holes in the ground."},
    {FLOOD_WATER_SHALLOW,       SURFACE,    10,     25,     0},
    {HOLE,                      SURFACE,    200,    100,    0},
    {HOLE_EDGE,                 SURFACE,    0,      0,      0},
    
    // gas trap effects
    {POISON_GAS,                GAS,        1000,   0,      0,  "a cloud of caustic gas sprays upward from the floor!"},
    {CONFUSION_GAS,             GAS,        300,    0,      0,  "a sparkling cloud of confusion gas sprays upward from the floor!"},
    {NETTING,                   SURFACE,    300,    90,     0,  "a net falls from the ceiling!"},
    {NOTHING,                   NO_LAYER,   0,      0,      DFF_AGGRAVATES_MONSTERS, "a piercing shriek echoes through the nearby rooms!", NO_LIGHT, 0, DCOLS/2},
    {METHANE_GAS,               GAS,        10000,  0,      0}, // debugging toy
    
    // potions
    {POISON_GAS,                GAS,        1000,   0,      0,  "", NO_LIGHT,  &poisonGasColor,4},
    {PARALYSIS_GAS,             GAS,        1000,   0,      0,  "", NO_LIGHT,  &pink,4},
    {CONFUSION_GAS,             GAS,        1000,   0,      0,  "", NO_LIGHT,  &confusionGasColor, 4},
    {PLAIN_FIRE,                SURFACE,    100,    37,     0,  "", EXPLOSION_FLARE_LIGHT},
    {DARKNESS_CLOUD,            GAS,        200,    0,      0},
    {HOLE_EDGE,                 SURFACE,    300,    100,    0,  "", NO_LIGHT,  &darkBlue,3,NOTHING,          DF_HOLE_2},
    {LICHEN,                    SURFACE,    70,     60,     0},
    
    // other items
    {PLAIN_FIRE,                SURFACE,    100,    45,     0,  "", NO_LIGHT,  &yellow,3},
    {HOLE_GLOW,                 SURFACE,    200,    100,    DFF_SUBSEQ_EVERYWHERE,  "", NO_LIGHT,  &darkBlue,3,NOTHING,          DF_STAFF_HOLE_EDGE},
    {HOLE_EDGE,                 SURFACE,    100,    100,    0},
    
    // machine components
    
    // commutation altars
    {COMMUTATION_ALTAR_INERT,   DUNGEON,    0,      0,      0,  "the items on the two altars flash with a brilliant light!", SCROLL_ENCHANTMENT_LIGHT},
    {PIPE_GLOWING,              SURFACE,    90,     60,     0},
    {PIPE_INERT,                SURFACE,    0,      0,      0,  "", SCROLL_ENCHANTMENT_LIGHT},
    
    // resurrection altars
    {RESURRECTION_ALTAR_INERT,  DUNGEON,    0,      0,      DFF_RESURRECT_ALLY, "An old friend emerges from a bloom of sacred light!", EMPOWERMENT_LIGHT},
    {MACHINE_TRIGGER_FLOOR_REPEATING, LIQUID, 300,  100,    DFF_SUPERPRIORITY},
    
    // coffin bursts open to reveal vampire:
    {COFFIN_OPEN,               DUNGEON,    0,      0,      DFF_ACTIVATE_DORMANT_MONSTER,   "the coffin opens and a dark figure rises!", NO_LIGHT, &darkGray, 3},
    {PLAIN_FIRE,                SURFACE,    0,      0,      DFF_ACTIVATE_DORMANT_MONSTER,   "as flames begin to lick the coffin, its tenant bursts forth!", NO_LIGHT, 0, 0, NOTHING, DF_EMBERS_PATCH},
    {MACHINE_TRIGGER_FLOOR,     DUNGEON,    200,    100,    0},
    
    // throwing tutorial:
    {ALTAR_INERT,               DUNGEON,    0,      0,      0,  "the cage lifts off of the altar.", GENERIC_FLASH_LIGHT},
    {TRAP_DOOR,                 LIQUID,     225,    100,    (DFF_CLEAR_OTHER_TERRAIN | DFF_SUBSEQ_EVERYWHERE), "", NO_LIGHT, 0, 0, NOTHING, DF_SHOW_TRAPDOOR_HALO},
    {LAVA,                      LIQUID,     225,    100,    (DFF_CLEAR_OTHER_TERRAIN)},
    {MACHINE_PRESSURE_PLATE_USED,DUNGEON,   0,      0,      0},
    
    // rat trap:
    {RAT_TRAP_WALL_CRACKING,    DUNGEON,    0,      0,      0,  "a scratching sound emanates from the nearby walls!", NO_LIGHT, 0, 0, NOTHING, DF_RUBBLE},
    
    // wooden barricade at entrance:
    {PLAIN_FIRE,                SURFACE,    0,      0,      0,  "flames quickly consume the wooden barricade."},
    
    // wooden barricade around altar:
    {WOODEN_BARRICADE,          DUNGEON,    220,    100,    (DFF_TREAT_AS_BLOCKING | DFF_SUBSEQ_EVERYWHERE), "", NO_LIGHT, 0, 0, NOTHING, DF_SMALL_DEAD_GRASS},
    
    // shallow water flood machine:
    {MACHINE_FLOOD_WATER_SPREADING, LIQUID, 0,      0,      0,  "you hear a heavy click, and the nearby water begins flooding the area!"},
    {SHALLOW_WATER,             LIQUID,     0,      0,      0},
    {MACHINE_FLOOD_WATER_SPREADING,LIQUID,  100,    100,    0,  "", NO_LIGHT,  0,  0,      FLOOR_FLOODABLE,            DF_SHALLOW_WATER},
    {MACHINE_FLOOD_WATER_DORMANT,LIQUID,    250,    100,    (DFF_TREAT_AS_BLOCKING), "", NO_LIGHT, 0, 0, NOTHING,            DF_SPREADABLE_DEEP_WATER_POOL},
    {DEEP_WATER,                LIQUID,     90,     100,    (DFF_CLEAR_OTHER_TERRAIN | DFF_PERMIT_BLOCKING)},
    
    // unstable floor machine:
    {MACHINE_COLLAPSE_EDGE_SPREADING,LIQUID,0,      0,      0,  "you hear a deep rumbling noise as the floor begins to collapse!"},
    {CHASM,                     LIQUID,     0,      0,      DFF_CLEAR_OTHER_TERRAIN, "", NO_LIGHT, 0, 0, NOTHING, DF_SHOW_TRAPDOOR_HALO},
    {MACHINE_COLLAPSE_EDGE_SPREADING,LIQUID,100,    100,    0,  "", NO_LIGHT,  0,  0,  FLOOR_FLOODABLE,    DF_COLLAPSE},
    {MACHINE_COLLAPSE_EDGE_DORMANT,LIQUID,  0,      0,      0},
    
    // levitation bridge machine:
    {CHASM_WITH_HIDDEN_BRIDGE_ACTIVE,LIQUID,100,    100,    0,  "", NO_LIGHT, 0,  0,  CHASM_WITH_HIDDEN_BRIDGE,  DF_BRIDGE_APPEARS},
    {CHASM_WITH_HIDDEN_BRIDGE_ACTIVE,LIQUID,100,    100,    0,  "a stone bridge extends from the floor with a grinding sound.", NO_LIGHT, 0,  0,  CHASM_WITH_HIDDEN_BRIDGE,  DF_BRIDGE_APPEARS},
    {STONE_BRIDGE,              LIQUID,     0,      0,      0},
    {MACHINE_CHASM_EDGE,        LIQUID,     100,    100,    0},
    
    // retracting lava pool:
    {LAVA_RETRACTABLE,          LIQUID,     100,    100,    0,  "", NO_LIGHT, 0,  0,  LAVA},
    {LAVA_RETRACTING,           LIQUID,     0,      0,      0,  "hissing fills the air as the lava begins to cool."},
    {OBSIDIAN,                  SURFACE,    0,      0,      0,  "", NO_LIGHT,  0,  0,      NOTHING,          DF_STEAM_ACCUMULATION},
    
    // hidden poison vent machine:
    {MACHINE_POISON_GAS_VENT_DORMANT,DUNGEON,0,     0,      0,  "you notice an inactive gas vent hidden in a crevice of the floor.", GENERIC_FLASH_LIGHT},
    {MACHINE_POISON_GAS_VENT,   DUNGEON,    0,      0,      0,  "deadly purple gas starts wafting out of hidden vents in the floor!"},
    {PORTCULLIS_CLOSED,         DUNGEON,    0,      0,      DFF_EVACUATE_CREATURES_FIRST,   "with a heavy mechanical sound, an iron portcullis falls from the ceiling!", GENERIC_FLASH_LIGHT},
    {PORTCULLIS_DORMANT,        DUNGEON,    0,      0,      0,  "the portcullis slowly rises from the ground into a slot in the ceiling.", GENERIC_FLASH_LIGHT},
    {POISON_GAS,                GAS,        25,     0,      0},
    
    // hidden methane vent machine:
    {MACHINE_METHANE_VENT_DORMANT,DUNGEON,0,        0,      0,  "you notice an inactive gas vent hidden in a crevice of the floor.", GENERIC_FLASH_LIGHT},
    {MACHINE_METHANE_VENT,      DUNGEON,    0,      0,      0,  "explosive methane gas starts wafting out of hidden vents in the floor!", NO_LIGHT, 0, 0, NOTHING, DF_VENT_SPEW_METHANE},
    {METHANE_GAS,               GAS,        60,     0,      0},
    {PILOT_LIGHT,               DUNGEON,    0,      0,      0,  "a torch falls from its mount and lies sputtering on the floor.", FALLEN_TORCH_FLASH_LIGHT},
    
    // paralysis trap:
    {MACHINE_PARALYSIS_VENT,    DUNGEON,    0,      0,      0,  "you notice an inactive gas vent hidden in a crevice of the floor.", GENERIC_FLASH_LIGHT},
    {PARALYSIS_GAS,             GAS,        350,    0,      0,  "paralytic gas sprays upward from hidden vents in the floor!", NO_LIGHT, 0, 0, NOTHING, DF_REVEAL_PARALYSIS_VENT_SILENTLY},
    {MACHINE_PARALYSIS_VENT,    DUNGEON,    0,      0,      0},
    
    // thematic dungeon:
    {RED_BLOOD,                 SURFACE,    75,     25,     0},
    
    // statuary:
    {STATUE_CRACKING,           DUNGEON,    0,      0,      0,  "cracks begin snaking across the marble surface of the statue!", NO_LIGHT, 0, 0, NOTHING, DF_RUBBLE},
    {RUBBLE,                    SURFACE,    120,    100,    DFF_ACTIVATE_DORMANT_MONSTER,   "the statue shatters!", NO_LIGHT, &darkGray, 3, NOTHING, DF_RUBBLE},
    
    // hidden turrets:
    {WALL,                      DUNGEON,    0,      0,      DFF_ACTIVATE_DORMANT_MONSTER,   "you hear a click, and the stones in the wall shift to reveal turrets!", NO_LIGHT, 0, 0, NOTHING, DF_RUBBLE},
    
    // worm tunnels:
    {WORM_TUNNEL_MARKER_DORMANT,LIQUID,     5,      5,      0,  "", NO_LIGHT,  0,  GRANITE},
    {WORM_TUNNEL_MARKER_ACTIVE, LIQUID,     0,      0,      0},
    {FLOOR,                     DUNGEON,    0,      0,      (DFF_SUPERPRIORITY | DFF_ACTIVATE_DORMANT_MONSTER),  "", NO_LIGHT, 0,  0,  NOTHING,  DF_TUNNELIZE},
    {FLOOR,                     DUNGEON,    0,      0,      0,  "the nearby wall cracks and collapses in a cloud of dust!", NO_LIGHT, &darkGray,  5,  NOTHING,  DF_TUNNELIZE},
    
    // haunted room:
    {DARK_FLOOR_DARKENING,      DUNGEON,    0,      0,      0,  "the light in the room flickers and you feel a chill in the air."},
    {DARK_FLOOR,                DUNGEON,    0,      0,      DFF_ACTIVATE_DORMANT_MONSTER,   "", NO_LIGHT, 0, 0, NOTHING, DF_ECTOPLASM_DROPLET},
    {HAUNTED_TORCH_TRANSITIONING,DUNGEON,   0,      0,      0},
    {HAUNTED_TORCH,             DUNGEON,    0,      0,      0},
    
    // mud pit:
    {MACHINE_MUD_DORMANT,       LIQUID,     100,    100,    0},
    {MUD,                       LIQUID,     0,      0,      DFF_ACTIVATE_DORMANT_MONSTER,   "across the bog, bubbles rise ominously from the mud."},
    
    // electric crystals:
    {ELECTRIC_CRYSTAL_ON,       DUNGEON,    0,      0,      0, "the crystal absorbs the electricity and begins to glow.", CHARGE_FLASH_LIGHT},
    {WALL,                      DUNGEON,    0,      0,      DFF_ACTIVATE_DORMANT_MONSTER,   "the wall above the lever shifts to reveal a spark turret!"},
    
    // idyll:
    {SHALLOW_WATER,             LIQUID,     150,    100,    (DFF_PERMIT_BLOCKING)},
    {DEEP_WATER,                LIQUID,     90,     100,    (DFF_TREAT_AS_BLOCKING | DFF_CLEAR_OTHER_TERRAIN | DFF_SUBSEQ_EVERYWHERE), "", NO_LIGHT, 0, 0, NOTHING, DF_SHALLOW_WATER_POOL},
    
    // swamp:
    {SHALLOW_WATER,             LIQUID,     30,     100,    0},
    {GRAY_FUNGUS,               SURFACE,    80,     50,     0,  "", NO_LIGHT, 0, 0, NOTHING, DF_SWAMP_MUD},
    {MUD,                       LIQUID,     75,     5,      0,  "", NO_LIGHT, 0, 0, NOTHING, DF_SWAMP_WATER},
    
    // camp:
    {HAY,                       SURFACE,    90,     87,     0},
    {JUNK,                      SURFACE,    20,     20,     0},
    
    // remnants:
    {CARPET,                    DUNGEON,    110,    20,     DFF_SUBSEQ_EVERYWHERE,  "", NO_LIGHT, 0, 0, NOTHING, DF_REMNANT_ASH},
    {BURNED_CARPET,             SURFACE,    120,    100,    0},
    
    // chasm catwalk:
    {CHASM,                     LIQUID,     0,      0,      DFF_CLEAR_OTHER_TERRAIN, "", NO_LIGHT, 0, 0, NOTHING, DF_SHOW_TRAPDOOR_HALO},
    {STONE_BRIDGE,              LIQUID,     0,      0,      DFF_CLEAR_OTHER_TERRAIN},

    // lake catwalk:
    {DEEP_WATER,                LIQUID,     0,      0,      DFF_CLEAR_OTHER_TERRAIN, "", NO_LIGHT, 0, 0, NOTHING, DF_LAKE_HALO},
    {SHALLOW_WATER,             LIQUID,     160,    100,    0},
    
    // worms pop out of walls:
    {RUBBLE,                    SURFACE,    120,    100,    DFF_ACTIVATE_DORMANT_MONSTER,   "the nearby wall explodes in a shower of stone fragments!", NO_LIGHT, &darkGray, 3, NOTHING, DF_RUBBLE},
    
    // monster cages open:
    {MONSTER_CAGE_OPEN,         DUNGEON,    0,      0,      0},
    
    // goblin warren:
    {STENCH_SMOKE_GAS,          GAS,        50,     0,      0, "", NO_LIGHT, 0, 0, NOTHING, DF_PLAIN_FIRE},
    {STENCH_SMOKE_GAS,          GAS,        50,     0,      0, "", NO_LIGHT, 0, 0, NOTHING, DF_EMBERS},
    
    // chimeric:
    {RUBBLE,                    SURFACE,    0,      0,      (DFF_ACTIVATE_DORMANT_MONSTER)},
};

#pragma mark Dungeon Profiles

dungeonProfile dungeonProfileCatalog[NUMBER_DUNGEON_PROFILES] = {
    // Room frequencies:
    //      0. Cross room
    //      1. Small symmetrical cross room
    //      2. Small room
    //      3. Circular room
    //      4. Chunky room
    //      5. Cave
    //      6. Cavern (the kind that fills a level)
    //      7. Entrance room (the big upside-down T room at the start of depth 1)
    
    // Room frequencies
    // 0    1   2   3   4   5   6   7   Corridor chance
    {{2,    1,  1,  1,  7,  1,  0,  0}, 10},    // Basic dungeon generation (further adjusted by depth)
    {{10,   0,  0,  3,  7,  10, 10, 0}, 0},     // First room for basic dungeon generation (further adjusted by depth)
    
    {{0,    0,  1,  0,  0,  0,  0,  0}, 0},     // Goblin warrens
    {{0,    5,  0,  1,  0,  0,  0,  0}, 0},     // Sentinel sanctuaries
};

#pragma mark Lights

// radius is in units of 0.01
lightSource lightCatalog[NUMBER_LIGHT_KINDS] = {
    //color                 radius range            fade%   passThroughCreatures
    {0},                                                                // NO_LIGHT
    {&minersLightColor,     {0, 0, 1},              35,     true},      // miners light
    {&fireBoltColor,        {300, 400, 1},          0,      false},     // burning creature light
    {&wispLightColor,       {400, 800, 1},          0,      false},     // will-o'-the-wisp light
    {&fireBoltColor,        {300, 400, 1},          0,      false},     // salamander glow
    {&pink,                 {600, 600, 1},          0,      true},      // imp light
    {&pixieColor,           {400, 600, 1},          50,     false},     // pixie light
    {&lichLightColor,       {1500, 1500, 1},        0,      false},     // lich light
    {&flamedancerCoronaColor,{1000, 2000, 1},       0,      false},     // flamedancer light
    {&sentinelLightColor,   {300, 500, 1},          0,      false},     // sentinel light
    {&unicornLightColor,    {300, 400, 1},          0,      false},     // unicorn light
    {&ifritLightColor,      {300, 600, 1},          0,      false},     // ifrit light
    {&fireBoltColor,        {400, 600, 1},          0,      false},     // phoenix light
    {&fireBoltColor,        {150, 300, 1},          0,      false},     // phoenix egg light
    {&yendorLightColor,     {1500, 1500, 1},        0,      false},     // Yendorian light
    {&spectralBladeLightColor,{350, 350, 1},        0,      false},     // spectral blades
    {&summonedImageLightColor,{350, 350, 1},        0,      false},     // weapon images
    {&lightningColor,       {250, 250, 1},          35,     false},     // lightning turret light
    {&explosiveAuraColor,   {150, 200, 1},          0,      true},      // explosive bloat light
    {&lightningColor,       {300, 300, 1},          0,      false},     // bolt glow
    {&telepathyColor,       {200, 200, 1},          0,      true},      // telepathy light
    
    // flares:
    {&scrollProtectionColor,{600, 600, 1},          0,      true},      // scroll of protection flare
    {&scrollEnchantmentColor,{600, 600, 1},         0,      true},      // scroll of enchantment flare
    {&potionStrengthColor,  {600, 600, 1},          0,      true},      // potion of strength flare
    {&empowermentFlashColor,{600, 600, 1},          0,      true},      // empowerment flare
    {&genericFlashColor,    {300, 300, 1},          0,      true},      // generic flash flare
    {&fireFlashColor,       {800, 800, 1},          0,      false},     // fallen torch flare
    {&summoningFlashColor,  {600, 600, 1},          0,      true},      // summoning flare
    {&explosionFlareColor,  {5000, 5000, 1},        0,      true},      // explosion (explosive bloat or incineration potion)
    {&quietusFlashColor,    {300, 300, 1},          0,      true},      // quietus activation flare
    {&slayingFlashColor,    {300, 300, 1},          0,      true},      // slaying activation flare
    {&lightningColor,       {800, 800, 1},          0,      false},     // electric crystal activates
    
    // glowing terrain:
    {&torchLightColor,      {1000, 1000, 1},        50,     false},     // torch
    {&lavaLightColor,       {300, 300, 1},          50,     false},     // lava
    {&sunLightColor,        {200, 200, 1},          25,     true},      // sunlight
    {&darknessPatchColor,   {400, 400, 1},          0,      true},      // darkness patch
    {&fungusLightColor,     {300, 300, 1},          50,     false},     // luminescent fungus
    {&fungusForestLightColor,{500, 500, 1},         0,      false},     // luminescent forest
    {&algaeBlueLightColor,  {300, 300, 1},          0,      false},     // luminescent algae blue
    {&algaeGreenLightColor, {300, 300, 1},          0,      false},     // luminescent algae green
    {&ectoplasmColor,       {200, 200, 1},          50,     false},     // ectoplasm
    {&unicornLightColor,    {200, 200, 1},          0,      false},     // unicorn poop light
    {&lavaLightColor,       {200, 200, 1},          50,     false},     // embers
    {&lavaLightColor,       {500, 1000, 1},         0,      false},     // fire
    {&lavaLightColor,       {200, 300, 1},          0,      false},     // brimstone fire
    {&explosionColor,       {DCOLS*100,DCOLS*100,1},100,    false},     // explosions
    {&dartFlashColor,       {15*100,15*100,1},      0,      false},     // incendiary darts
    {&portalActivateLightColor, {DCOLS*100,DCOLS*100,1},0,  false},     // portal activation
    {&confusionLightColor,  {300, 300, 1},          100,    false},     // confusion gas
    {&darknessCloudColor,   {500, 500, 1},          0,      true},      // darkness cloud
    {&forceFieldLightColor, {200, 200, 1},          50,     false},     // forcefield
    {&crystalWallLightColor,{300, 500, 1},          50,     false},     // crystal wall
    {&torchLightColor,      {200, 400, 1},          0,      false},     // candle light
    {&hauntedTorchColor,    {400, 600, 1},          0,      false},     // haunted torch
    {&glyphLightColor,      {100, 100, 1},          0,      false},     // glyph dim light
    {&glyphLightColor,      {300, 300, 1},          0,      false},     // glyph bright light
    {&sacredGlyphColor,     {300, 300, 1},          0,      false},     // sacred glyph light
    {&descentLightColor,    {600, 600, 1},          0,      false},     // magical pit light
};

#pragma mark Mutation definitions

mutation mutationCatalog[NUMBER_MUTATORS] = {
    //Title         textColor       healthFactor    moveSpdMult attackSpdMult   defMult damMult DF% DFtype  light       monstFlags  abilityFlags    forbiddenFlags      forbiddenAbilities
    {"explosive",   &orange,        50,             100,        100,            50,     100,    0,  DF_MUTATION_EXPLOSION, EXPLOSIVE_BLOAT_LIGHT, 0, MA_DF_ON_DEATH, MONST_SUBMERGES, 0,
        "A rare mutation will cause $HIMHER to explode violently when $HESHE dies."},
    {"infested",    &lichenColor,   50,             100,        100,            50,     100,    0,  DF_MUTATION_LICHEN, NO_LIGHT, 0,MA_DF_ON_DEATH, 0,               0,
        "$HESHE has been infested by deadly lichen spores; poisonous fungus will spread from $HISHER corpse when $HESHE dies."},
    {"agile",       &lightBlue,     100,            50,         100,            150,    100,    -1, DF_NONE,NO_LIGHT,   MONST_FLEES_NEAR_DEATH, 0, MONST_FLEES_NEAR_DEATH, 0,
        "A rare mutation greatly enhances $HISHER mobility."},
    {"juggernaut",  &brown,         300,            200,        200,            75,     200,    -1, DF_NONE,NO_LIGHT,   0,          0,              MONST_MAINTAINS_DISTANCE, 0,
        "A rare mutation has hardened $HISHER flesh, increasing $HISHER health and power but compromising $HISHER speed."},
    {"grappling",   &tanColor,      150,            100,        100,            50,     100,    -1, DF_NONE,NO_LIGHT,   0,          MA_SEIZES,      MONST_MAINTAINS_DISTANCE, MA_SEIZES,
        "A rare mutation has caused suckered tentacles to sprout from $HISHER frame, increasing $HISHER health and allowing $HIMHER to grapple with $HISHER prey."},
    {"vampiric",    &red,           100,            100,        100,            100,    100,    -1, DF_NONE,NO_LIGHT,   0,          MA_TRANSFERENCE, MONST_MAINTAINS_DISTANCE, MA_TRANSFERENCE,
        "A rare mutation allows $HIMHER to heal $HIMSELFHERSELF with every attack."},
    {"toxic",       &green,         100,            100,        200,            100,    20,     -1, DF_NONE,NO_LIGHT,   0,          (MA_CAUSES_WEAKNESS | MA_POISONS), MONST_MAINTAINS_DISTANCE, (MA_CAUSES_WEAKNESS | MA_POISONS),
        "A rare mutation causes $HIMHER to poison $HISHER victims and sap their strength with every attack."},
    {"reflective",  &darkTurquoise, 100,            100,        100,            100,    100,    -1, DF_NONE,NO_LIGHT,   MONST_REFLECT_4, 0,         (MONST_REFLECT_4 | MONST_ALWAYS_USE_ABILITY), 0,
        "A rare mutation has coated $HISHER flesh with reflective scales."},
};

// ITEMS

#pragma mark Item flavors

char itemTitles[NUMBER_SCROLL_KINDS][30];

char titlePhonemes[NUMBER_TITLE_PHONEMES][30] = {
    "glorp",
    "snarg",
    "gana",
    "flin",
    "herba",
    "pora",
    "nuglo",
    "greep",
    "nur",
    "lofa",
    "poder",
    "nidge",
    "pus",
    "wooz",
    "flem",
    "bloto",
    "porta",
    "ermah",
    "gerd",
    "nurt",
    "flurx",
};

char itemColors[NUMBER_ITEM_COLORS][30];

char itemColorsRef[NUMBER_ITEM_COLORS][30] = {
    "crimson",
    "scarlet",
    "orange",
    "yellow",
    "green",
    "blue",
    "indigo",
    "violet",
    "puce",
    "mauve",
    "burgundy",
    "turquoise",
    "aquamarine",
    "gray",
    "pink",
    "white",
    "lavender",
    "tan",
    "brown",
    "cyan",
    "black"
};

char itemWoods[NUMBER_ITEM_WOODS][30];

char itemWoodsRef[NUMBER_ITEM_WOODS][30] = {
    "teak",
    "oak",
    "redwood",
    "rowan",
    "willow",
    "mahogany",
    "pinewood",
    "maple",
    "bamboo",
    "ironwood",
    "pearwood",
    "birch",
    "cherry",
    "eucalyptus",
    "walnut",
    "cedar",
    "rosewood",
    "yew",
    "sandalwood",
    "hickory",
    "hemlock",
};

char itemMetals[NUMBER_ITEM_METALS][30];

char itemMetalsRef[NUMBER_ITEM_METALS][30] = {
    "bronze",
    "steel",
    "brass",
    "pewter",
    "nickel",
    "copper",
    "aluminum",
    "tungsten",
    "titanium",
    "cobalt",
    "chromium",
    "silver",
};

char itemGems[NUMBER_ITEM_GEMS][30];

char itemGemsRef[NUMBER_ITEM_GEMS][30] = {
    "diamond",
    "opal",
    "garnet",
    "ruby",
    "amethyst",
    "topaz",
    "onyx",
    "tourmaline",
    "sapphire",
    "obsidian",
    "malachite",
    "aquamarine",
    "emerald",
    "jade",
    "alexandrite",
    "agate",
    "bloodstone",
    "jasper"
};

#pragma mark Item definitions

//typedef struct itemTable {
//  char *name;
//  char *flavor;
//  short frequency;
//  short marketValue;
//  short number;
//  randomRange range;
//} itemTable;

itemTable keyTable[NUMBER_KEY_TYPES] = {
    {"door key",            "", "", 1, 0,   0, {0,0,0}, true, false, "The notches on this ancient iron key are well worn; its leather lanyard is battered by age. What door might it open?"},
    {"cage key",            "", "", 1, 0,   0, {0,0,0}, true, false, "The rust accreted on this iron key has been stained with flecks of blood; it must have been used recently. What cage might it open?"},
    {"crystal orb",         "", "", 1, 0,   0, {0,0,0}, true, false, "A faceted orb, seemingly cut from a single crystal, sparkling and perpetually warm to the touch. What manner of device might such an object activate?"},
};

itemTable foodTable[NUMBER_FOOD_KINDS] = {
    {"ration of food",      "", "", 3, 25,  1800, {0,0,0}, true, false, "A ration of food. Was it left by former adventurers? Is it a curious byproduct of the subterranean ecosystem?"},
    {"mango",               "", "", 1, 15,  1550, {0,0,0}, true, false, "An odd fruit to be found so deep beneath the surface of the earth, but only slightly less filling than a ration of food."}
};

itemTable weaponTable[NUMBER_WEAPON_KINDS] = {
    {"dagger",              "", "", 10, 190,        12, {3, 4,  1},     true, false, "A simple iron dagger with a well-worn wooden handle. Daggers will deal quintuple damage upon a succesful sneak attack instead of triple damage."},
    {"sword",               "", "", 10, 440,        14, {7, 9,  1},     true, false, "The razor-sharp length of steel blade shines reassuringly."},
    {"broadsword",          "", "", 10, 990,        19, {14, 22, 1},    true, false, "This towering blade inflicts heavy damage by investing its heft into every cut."},
    
    {"whip",                "", "", 10, 440,        14, {3, 5,  1},     true, false, "This lash from this coil of braided leather can tear bark from trees, and it will reach opponents up to five spaces away."},
    {"rapier",              "", "", 10, 440,        15, {3, 5,  1},     true, false, "This blade is thin and flexible, designed for deft and rapid maneuvers. It inflicts less damage than comparable weapons, but permits you to attack twice as quickly. If there is one space between you and an enemy and you step directly toward it, you will perform a devastating lunge attack, which deals treble damage and never misses."},
    {"flail",               "", "", 10, 440,        17, {10,13, 1},     true, false, "The spiked iron ball can be whirled at the end of its chain in synchronicity with your movement, allowing you a free attack whenever moving between two spaces that are adjacent to an enemy."},
    
    {"mace",                "", "", 10, 660,        16, {16, 20, 1},    true, false, "The iron flanges at the head of this weapon inflict substantial damage with every weighty blow. Because of its heft, it takes two turns when it hits."},
    {"war hammer",          "", "", 10, 1100,       20, {25, 35, 1},    true, false, "Few creatures can withstand the crushing blow of this towering mass of lead and steel, but only the strongest of adventurers can effectively wield it. Because of its heft, it takes two turns when it hits."},
    
    {"spear",               "", "", 10, 330,        13, {4, 5, 1},      true, false, "A slender wooden rod tipped with sharpened iron. The reach of the spear permits you to simultaneously attack an adjacent enemy and the enemy directly behind it."},
    {"war pike",            "", "", 10, 880,        18, {11, 15, 1},    true, false, "A long steel pole ending in a razor-sharp point. The reach of the pike permits you to simultaneously attack an adjacent enemy and the enemy directly behind it."},
    
    {"axe",                 "", "", 10, 550,        15, {7, 9, 1},      true, false, "The blunt iron edge on this axe glints in the darkness. The arc of its swing permits you to attack all adjacent enemies simultaneously."},
    {"war axe",             "", "", 10, 990,        19, {12, 17, 1},    true, false, "The enormous steel head of this war axe puts considerable heft behind each stroke. The arc of its swing permits you to attack all adjacent enemies simultaneously."},

    {"dart",                "", "", 0,  15,         10, {2, 4,  1},     true, false, "These simple metal spikes are weighted to fly true and sting their prey with a flick of the wrist."},
    {"incendiary dart",     "", "", 10, 25,         12, {1, 2,  1},     true, false, "The barbed spike on each of these darts is designed to stick to its target while the compounds strapped to its length explode into flame."},
    {"javelin",             "", "", 10, 40,         15, {3, 11, 3},     true, false, "This length of metal is weighted to keep the spike at its tip foremost as it sails through the air."},
};

itemTable armorTable[NUMBER_ARMOR_KINDS] = {
    {"leather armor",   "", "", 10, 250,        10, {30,30,0},      true, false, "This lightweight armor offers basic protection."},
    {"scale mail",      "", "", 10, 350,        12, {40,40,0},      true, false, "Bronze scales cover the surface of treated leather, offering greater protection than plain leather with minimal additional weight."},
    {"chain mail",      "", "", 10, 500,        13, {50,50,0},      true, false, "Interlocking metal links make for a tough but flexible suit of armor."},
    {"banded mail",     "", "", 10, 800,        15, {70,70,0},      true, false, "Overlapping strips of metal horizontally encircle a chain mail base, offering an additional layer of protection at the cost of greater weight."},
    {"splint mail",     "", "", 10, 1000,       17, {90,90,0},      true, false, "Thick plates of metal are embedded into a chain mail base, providing the wearer with substantial protection."},
    {"plate armor",     "", "", 10, 1300,       19, {110,110,0},    true, false, "Enormous plates of metal are joined together into a suit that provides unmatched protection to any adventurer strong enough to bear its staggering weight."}
};

char weaponRunicNames[NUMBER_WEAPON_RUNIC_KINDS][30] = {
    "speed",
    "quietus",
    "paralysis",
    "multiplicity",
    "slowing",
    "confusion",
    "force",
    "slaying",
    "mercy",
    "plenty"
};

char armorRunicNames[NUMBER_ARMOR_ENCHANT_KINDS][30] = {
    "multiplicity",
    "mutuality",
    "absorption",
    "reprisal",
    "immunity",
    "reflection",
    "respiration",
    "dampening",
    "burden",
    "vulnerability",
    "immolation",
};

itemTable scrollTable[NUMBER_SCROLL_KINDS] = {
    {"enchanting",          itemTitles[0], "",  0,  550,    0,{0,0,0}, false, false, "This indispensable scroll will imbue a single item with a powerful and permanent magical charge. A staff will increase in power and in number of charges; a weapon will inflict more damage or find its mark more frequently; a suit of armor will deflect additional blows; the effect of a ring on its wearer will intensify; and a wand will gain expendable charges in the least amount that such a wand can be found with. Weapons and armor will also require less strength to use, and any curses on the item will be lifted."}, // frequency is dynamically adjusted
    {"identify",            itemTitles[1], "",  30, 300,    0,{0,0,0}, false, false, "The scrying magic on this parchment will permanently reveal all of the secrets of a single item."},
    {"teleportation",       itemTitles[2], "",  10, 500,    0,{0,0,0}, false, false, "The spell on this parchment instantly transports the reader to a random location on the dungeon level. It can be used to escape a dangerous situation, but the unlucky reader might find himself in an even more dangerous place."},
    {"remove curse",        itemTitles[3], "",  15, 150,    0,{0,0,0}, false, false, "The incantation on this scroll will instantly strip from the reader's weapon, armor, rings and carried items any evil enchantments that might prevent the wearer from removing them."},
    {"recharging",          itemTitles[4], "",  12, 375,    0,{0,0,0}, false, false, "The power bound up in this parchment will instantly recharge all of your staffs and charms."},
    {"protect armor",       itemTitles[5], "",  10, 400,    0,{0,0,0}, false, false, "The armor worn by the reader of this scroll will be permanently proofed against degradation from acid."},
    {"protect weapon",      itemTitles[6], "",  10, 400,    0,{0,0,0}, false, false, "The weapon held by the reader of this scroll will be permanently proofed against degradation from acid."},
    {"sanctuary",           itemTitles[7], "",  10, 500,    0,{0,0,0}, false, false, "When recited over plain ground, the sacred rite of protection memorialized on this scroll will imbue the area with powerful warding glyphs. Monsters will not willingly set foot on the affected area."},
    {"magic mapping",       itemTitles[8], "",  12, 500,    0,{0,0,0}, false, false, "When this scroll is read, a purple-hued image of crystal clarity will be etched into your memory, alerting you to the precise layout of the level and revealing all hidden secrets. The locations of items and creatures will remain unknown."},
    {"negation",            itemTitles[9], "",  8,  400,    0,{0,0,0}, false, false, "This scroll contains a powerful anti-magic. When it is released, all creatures (including yourself) and all items lying on the ground within your field of view will be exposed to its blast and stripped of magic -- and creatures animated purely by magic will die. Potions, scrolls, items being held by other creatures and items in your inventory will not be affected."},
    {"shattering",          itemTitles[10],"",  8,  500,    0,{0,0,0}, false, false, "The blast of sorcery unleashed by this scroll will alter the physical structure of nearby stone, causing it to dissolve away over the ensuing minutes."},
    {"discord",             itemTitles[11], "", 8,  400,    0,{0,0,0}, false, false, "Reading this scroll will unleash a powerful blast of mind magic. Any creatures within line of sight will turn against their companions and attack indiscriminately for 30 turns."},
    {"aggravate monsters",  itemTitles[12], "", 15, 50,     0,{0,0,0}, false, false, "When read aloud, this scroll will unleash a piercing shriek that will awaken all monsters and alert them to the reader's location."},
    {"summon monsters",     itemTitles[13], "", 10, 50,     0,{0,0,0}, false, false, "The incantation on this scroll will call out to creatures in other planes of existence, drawing them through the fabric of reality to confront the reader."},
};

itemTable potionTable[NUMBER_POTION_KINDS] = {
    {"life",                itemColors[1], "",  0,  500,    0,{0,0,0}, false, false, "A swirling elixir that will instantly heal you, cure you of ailments, and permanently increase your maximum health."}, // frequency is dynamically adjusted
    {"strength",            itemColors[2], "",  0,  400,    0,{0,0,0}, false, false, "This powerful medicine will course through your muscles, permanently increasing your strength by one point."}, // frequency is dynamically adjusted
    {"telepathy",           itemColors[3], "",  20, 350,    0,{0,0,0}, false, false, "After drinking this, your mind will become attuned to the psychic signature of distant creatures, enabling you to sense biological presences through walls. Its effects will not reveal inanimate objects, such as totems, turrets and traps."},
    {"levitation",          itemColors[4], "",  15, 250,    0,{0,0,0}, false, false, "Drinking this curious liquid will cause you to hover in the air, able to drift effortlessly over lava, water, chasms and traps. Flames, gases and spiderwebs fill the air, however, and cannot be bypassed while airborne. Creatures that dwell in water or mud will be unable to attack you while you levitate."},
    {"detect magic",        itemColors[5], "",  20, 500,    0,{0,0,0}, false, false, "This drink will sensitize your mind to the radiance of magic. Items imbued with helpful enchantments will be marked on the map with a full magical sigil; items corrupted by curses or intended to inflict harm on the bearer will be marked on the map with a hollow sigil. The Amulet of Yendor, if in the vicinity, will be revealed by its unique aura."},
    {"speed",               itemColors[6], "",  10, 500,    0,{0,0,0}, false, false, "Quaffing the contents of this flask will enable you to move at blinding speed for several minutes."},
    {"fire immunity",       itemColors[7], "",  15, 500,    0,{0,0,0}, false, false, "This potion will render you impervious to heat and permit you to wander through fire and lava and ignore otherwise deadly bolts of flame. It will not guard against the concussive impact of an explosion, however."},
    {"invisibility",        itemColors[8], "",  15, 400,    0,{0,0,0}, false, false, "Drinking this potion will render you temporarily invisible. Enemies more than two spaces away will be unable to track you."},
    {"caustic gas",         itemColors[9], "",  15, 200,    0,{0,0,0}, false, false, "Uncorking or shattering this pressurized glass will cause its contents to explode into a deadly cloud of caustic purple gas. You might choose to fling this potion at distant enemies instead of uncorking it by hand."},
    {"paralysis",           itemColors[10], "", 10, 250,    0,{0,0,0}, false, false, "Upon exposure to open air, the liquid in this flask will vaporize into a numbing pink haze. Anyone who inhales the cloud will be paralyzed instantly, unable to move for some time after the cloud dissipates. This item can be thrown at distant enemies to catch them within the effect of the gas."},
    {"hallucination",       itemColors[11], "", 10, 500,    0,{0,0,0}, false, false, "This flask contains a vicious and long-lasting hallucinogen. Under its dazzling effect, you will wander through a rainbow wonderland, unable to discern the form of any creatures or items you see."},
    {"confusion",           itemColors[12], "", 15, 450,    0,{0,0,0}, false, false, "This unstable chemical will quickly vaporize into a glittering cloud upon contact with open air, causing any creature that inhales it to lose control of the direction of its movements until the effect wears off (although its ability to aim projectile attacks will not be affected). Its vertiginous intoxication can cause creatures and adventurers to careen into one another or into chasms or lava pits, so extreme care should be taken when under its effect. Its contents can be weaponized by throwing the flask at distant enemies."},
    {"incineration",        itemColors[13], "", 15, 500,    0,{0,0,0}, false, false, "This flask contains an unstable compound which will burst violently into flame upon exposure to open air. You might throw the flask at distant enemies -- or into a deep lake, to cleanse the cavern with scalding steam."},
    {"darkness",            itemColors[14], "", 7,  150,    0,{0,0,0}, false, false, "Drinking this potion will plunge you into darkness. At first, you will be completely blind to anything not illuminated by an independent light source, but over time your vision will regain its former strength. Throwing the potion will create a cloud of supernatural darkness, and enemies will have difficulty seeing or following you if you take refuge under its cover."},
    {"descent",             itemColors[15], "", 15, 500,    0,{0,0,0}, false, false, "When this flask is uncorked by hand or shattered by being thrown, the fog that seeps out will temporarily cause the ground in the vicinity to vanish."},
    {"creeping death",      itemColors[16], "", 7,  450,    0,{0,0,0}, false, false, "When the cork is popped or the flask is thrown, tiny spores will spill across the ground and begin to grow a deadly lichen. Anything that touches the lichen will be poisoned by its clinging tendrils, and the lichen will slowly grow to fill the area. Fire will purge the infestation."},
};

itemTable wandTable[NUMBER_WAND_KINDS] = {
    {"teleportation",   itemMetals[0], "",  3,  800,    BOLT_TELEPORT,      {3,5,1}, false, false, "A blast from this wand will teleport a creature to a random place on the level. This can be particularly effective against aquatic or mud-bound creatures, which are helpless on dry land."},
    {"slowness",        itemMetals[1], "",  3,  800,    BOLT_SLOW,          {2,5,1}, false, false, "This wand will cause a creature to move at half its ordinary speed for 30 turns."},
    {"polymorphism",    itemMetals[2], "",  3,  700,    BOLT_POLYMORPH,     {3,5,1}, false, false, "This mischievous magic can transform any creature into another creature at random. Beware: the tamest of creatures might turn into the most fearsome. The horror of the transformation will turn any affected allies against you."},
    {"negation",        itemMetals[3], "",  3,  550,    BOLT_NEGATION,      {4,6,1}, false, false, "This powerful anti-magic will strip a creature of a host of magical traits, including flight, invisibility, acidic corrosiveness, telepathy, magical speed or slowness, hypnosis, magical fear, immunity to physical attack, fire resistance and the ability to blink. Spellcasters will lose their magical abilities and magical totems will be rendered inert. Creatures animated purely by magic will die."},
    {"domination",      itemMetals[4], "",  1,  1000,   BOLT_DOMINATION,    {1,2,1}, false, false, "This wand can forever bind an enemy to the caster's will, turning it into a steadfast ally. However, the magic works only against enemies that are near death."},
    {"beckoning",       itemMetals[5], "",  3,  500,    BOLT_BECKONING,     {2,4,1}, false, false, "The force of this wand will yank the targeted creature into direct proximity."},
    {"plenty",          itemMetals[6], "",  2,  700,    BOLT_PLENTY,        {1,2,1}, false, false, "The creature at the other end of this mischievous bit of metal will be beside itself -- literally! Cloning an enemy is ill-advised, but the effect can be invaluable on a powerful ally."},
    {"invisibility",    itemMetals[7], "",  3,  100,    BOLT_INVISIBILITY,  {3,5,1}, false, false, "A charge from this wand will render a creature temporarily invisible to the naked eye. Only with telepathy or in the silhouette of a thick gas will an observer discern the creature's hazy outline."},
    {"empowerment",     itemMetals[8], "",  2,  100,    BOLT_EMPOWERMENT,   {1,1,1}, false, false, "This sacred magic will permanently improve the mind and body of any monster it hits. A wise adventurer will use it on allies, making them stronger in combat and able to learn a new talent from a fallen foe. If the bolt is reflected back at you, it will have no effect."},
};

itemTable staffTable[NUMBER_STAFF_KINDS] = {
    {"lightning",       itemWoods[0], "",   15, 1300,   BOLT_LIGHTNING,     {2,4,1}, false, false, "This staff conjures forth deadly arcs of electricity, which deal damage to any number of creatures in a straight line."},
    {"firebolt",        itemWoods[1], "",   15, 1300,   BOLT_FIRE,          {2,4,1}, false, false, "This staff unleashes bursts of magical fire. It will ignite flammable terrain, and will damage and burn a creature that it hits. Creatures with an immunity to fire will be unaffected by the bolt."},
    {"poison",          itemWoods[2], "",   10, 1200,   BOLT_POISON,        {2,4,1}, false, false, "The vile blast of this twisted bit of wood will imbue its target with a deadly venom. Each turn, a creature that is poisoned will suffer one point of damage per dose of poison it has received, and poisoned creatures will not regenerate lost health until the poison clears."},
    {"tunneling",       itemWoods[3], "",   10, 1000,   BOLT_TUNNELING,     {2,4,1}, false, false, "Bursts of magic from this staff will pass harmlessly through creatures but will reduce walls and other inanimate obstructions to rubble."},
    {"blinking",        itemWoods[4], "",   11, 1200,   BOLT_BLINKING,      {2,4,1}, false, false, "This staff will allow you to teleport in the chosen direction. Creatures and inanimate obstructions will block the teleportation. Be careful around dangerous terrain, as nothing will prevent you from teleporting to a fiery death in a lake of lava."},
    {"entrancement",    itemWoods[5], "",   6,  1000,   BOLT_ENTRANCEMENT,  {2,4,1}, false, false, "This curious staff will send creatures into a deep but temporary trance, in which they will mindlessly mirror your movements. You can use the effect to cause one creature to attack another or to step into hazardous terrain, but the spell will be broken if you attack the creature under the effect."},
    {"obstruction",     itemWoods[6], "",   10, 1000,   BOLT_OBSTRUCTION,   {2,4,1}, false, false, "A mass of impenetrable green crystal will spring forth from the point at which this staff is aimed, obstructing any who wish to move through the affected area and temporarily entombing any who are already there. The crystal will dissolve into the air as time passes. Higher level staffs will create larger obstructions."},
    {"discord",         itemWoods[7], "",   10, 1000,   BOLT_DISCORD,       {2,4,1}, false, false, "The purple light from this staff will alter the perception of a creature to lash out indiscriminately. Strangers and allies alike will turn on the victim."},
    {"conjuration",     itemWoods[8], "",   8,  1000,   BOLT_CONJURATION,   {2,4,1}, false, false, "A flick of this staff summons a number of phantom blades to fight on your behalf."},
    {"healing",         itemWoods[9], "",   5,  1100,   BOLT_HEALING,       {2,4,1}, false, false, "The crimson bolt from this staff will heal the injuries of any creature it touches. This can be counterproductive against enemies but can prove useful when aimed at your allies. Unfortunately, you cannot use this or any staff on yourself."},
    {"haste",           itemWoods[10], "",  5,  900,    BOLT_HASTE,         {2,4,1}, false, false, "The magical bolt from this staff will temporarily double the speed of any monster it hits. This can be counterproductive against enemies but can prove useful when aimed at your allies. Unfortunately, you cannot use this or any staff on yourself."},
    {"protection",      itemWoods[11], "",  5,  900,    BOLT_SHIELDING,     {2,4,1}, false, false, "A charge from this staff will bathe a creature in protective light, absorbing all damage until depleted. This can be counterproductive against enemies but can prove useful when aimed at your allies. Unfortunately, you cannot use this or any staff on yourself."},
};

itemTable ringTable[NUMBER_RING_KINDS] = {
    {"clairvoyance",    itemGems[0], "",    1,  900,    0,{1,3,1}, false, false, "Wearing this ring will permit you to see through nearby walls and doors, within a radius determined by the level of the ring. A cursed ring of clairvoyance will blind you to your immediate surroundings."},
    {"stealth",         itemGems[1], "",    1,  800,    0,{1,3,1}, false, false, "This ring will reduce your stealth range, making enemies less likely to notice you and more likely to lose your trail. Staying motionless and lurking in the shadows will make you even harder to spot. Cursed rings of stealth will increase your stealth range, making you easier to spot and to track."},
    {"regeneration",    itemGems[2], "",    1,  750,    0,{1,3,1}, false, false, "This ring increases the body's regenerative properties, allowing one to recover lost health at an accelerated rate. Cursed rings will decrease or even halt one's natural regeneration."},
    {"transference",    itemGems[3], "",    1,  750,    0,{1,3,1}, false, false, "Landing a melee attack while wearing this ring will heal you in proportion to the damage inflicted. Cursed rings will cause you to lose health with each attack you land."},
    {"light",           itemGems[4], "",    1,  600,    0,{1,3,1}, false, false, "This ring subtly enhances your vision, enabling you to see farther in the dimming light of the deeper dungeon levels. It will not make you more visible to enemies."},
    {"awareness",       itemGems[5], "",    1,  700,    0,{1,3,1}, false, false, "Wearing this ring will enable you to notice hidden secrets (traps, secret doors and hidden levers) more often and from a greater distance. Cursed rings of awareness will dull your senses, making it harder to notice secrets even when actively searching for them."},
    {"wisdom",          itemGems[6], "",    1,  700,    0,{1,3,1}, false, false, "Your staffs will recharge at an accelerated rate in the energy field that radiates from this ring. Cursed rings of wisdom will instead cause your staffs to recharge more slowly."},
    {"reaping",         itemGems[7], "",    1,  700,    0,{1,3,1}, false, false, "The blood magic in this ring will recharge your staffs and charms in proportion to the damage you inflict directly. Cursed rings of reaping will drain your staffs and charms when you inflict damage directly."},
};

itemTable charmTable[NUMBER_CHARM_KINDS] = {
    {"health",          "", "", 5,  900,    0,{1,2,1}, true, false, "This handful of dried bloodwort and mandrake root has been bound together with leather cord and imbued with a powerful healing magic."},
    {"protection",      "", "", 5,  800,    0,{1,2,1}, true, false, "Four copper rings have been joined into a tetrahedron. The construct is oddly warm to the touch."},
    {"haste",           "", "", 5,  750,    0,{1,2,1}, true, false, "Various animals have been etched into the surface of this brass bangle. It emits a barely audible hum."},
    {"fire immunity",   "", "", 3,  750,    0,{1,2,1}, true, false, "Eldritch flames flicker within this polished crystal bauble."},
    {"invisibility",    "", "", 5,  700,    0,{1,2,1}, true, false, "This intricate figurine depicts a strange humanoid creature. It has a face on both sides of its head, but all four eyes are closed."},
    {"telepathy",       "", "", 3,  700,    0,{1,2,1}, true, false, "Seven tiny glass eyes roll freely within this glass sphere. Somehow, they always come to rest facing outward."},
    {"levitation",      "", "", 1,  700,    0,{1,2,1}, true, false, "Sparkling dust and fragments of feather waft and swirl endlessly inside this small glass sphere."},
    {"shattering",      "", "", 1,  700,    0,{1,2,1}, true, false, "This turquoise crystal, fixed to a leather lanyard, hums with an arcane energy that sets your teeth on edge."},
    {"guardian",        "", "", 5,  700,    0,{1,2,1}, true, false, "When you touch this tiny granite statue, a rhythmic booming sound echoes in your head."},
//    {"fear",            "", "",   3,  700,    0,{1,2,1}, true, false, "When you gaze into the murky interior of this obsidian cube, you feel as though something predatory is watching you."},
    {"teleportation",   "", "", 4,  700,    0,{1,2,1}, true, false, "The surface of this nickel sphere has been etched with a perfect grid pattern. Somehow, the squares of the grid are all exactly the same size."},
    {"recharging",      "", "", 5,  700,    0,{1,2,1}, true, false, "A strip of bronze has been wound around a rough wooden sphere. Each time you touch it, you feel a tiny electric shock."},
    {"negation",        "", "", 5,  700,    0,{1,2,1}, true, false, "A featureless gray disc hangs from a leather lanyard. When you touch it, your hand briefly goes numb."},
};

#pragma mark Bolt definitions

bolt boltCatalog[NUMBER_BOLT_KINDS] = {
    {{0}},
    //name                      bolt description                ability description                         char    foreColor       backColor           boltEffect      magnitude       pathDF      targetDF    forbiddenMonsterFlags       flags
    {"teleportation spell",     "casts a teleport spell",       "can teleport other creatures",             0,      NULL,           &blue,              BE_TELEPORT,    10,             0,          0,          MONST_IMMOBILE,             (BF_TARGET_ENEMIES)},
    {"slowing spell",           "casts a slowing spell",        "can slow $HISHER enemies",                 0,      NULL,           &green,             BE_SLOW,        10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ENEMIES)},
    {"polymorph spell",         "casts a polymorphism spell",   "can polymorph other creatures",            0,      NULL,           &purple,            BE_POLYMORPH,   10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ENEMIES)},
    {"negation magic",          "casts a negation spell",       "can cast negation",                        0,      NULL,           &pink,              BE_NEGATION,    10,             0,          0,          0,                          (BF_TARGET_ENEMIES)},
    {"domination spell",        "casts a domination spell",     "can dominate other creatures",             0,      NULL,           &dominationColor,   BE_DOMINATION,  10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ENEMIES)},
    {"beckoning spell",         "casts a beckoning spell",      "can cast beckoning",                       0,      NULL,           &beckonColor,       BE_BECKONING,   10,             0,          0,          MONST_IMMOBILE,             (BF_TARGET_ENEMIES)},
    {"spell of plenty",         "casts a spell of plenty",      "can duplicate other creatures",            0,      NULL,           &rainbow,           BE_PLENTY,      10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ALLIES | BF_NOT_LEARNABLE)},
    {"invisibility magic",      "casts invisibility magic",     "can turn creatures invisible",             0,      NULL,           &darkBlue,          BE_INVISIBILITY, 10,            0,          0,          MONST_INANIMATE,            (BF_TARGET_ALLIES)},
    {"empowerment sorcery",     "casts empowerment",            "can cast empowerment",                     0,      NULL,           &empowermentColor,  BE_EMPOWERMENT, 10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ALLIES | BF_NOT_LEARNABLE)},
    {"lightning",               "casts lightning",              "can hurl lightning bolts",                 0,      NULL,           &lightningColor,    BE_DAMAGE,      10,             0,          0,          0,                          (BF_PASSES_THRU_CREATURES | BF_TARGET_ENEMIES | BF_ELECTRIC)},
    {"flame",                   "casts a gout of flame",        "can hurl gouts of flame",                  0,      NULL,           &fireBoltColor,     BE_DAMAGE,      4,              0,          0,          MONST_IMMUNE_TO_FIRE,       (BF_TARGET_ENEMIES | BF_FIERY)},
    {"poison ray",              "casts a poison ray",           "can cast poisonous bolts",                 0,      NULL,           &poisonColor,       BE_POISON,      10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ENEMIES)},
    {"tunneling magic",         "casts tunneling",              "can tunnel",                               0,      NULL,           &brown,             BE_TUNNELING,   10,             0,          0,          0,                          (BF_PASSES_THRU_CREATURES)},
    {"blink trajectory",        "blinks",                       "can blink",                                0,      NULL,           &white,             BE_BLINKING,    5,              0,          0,          0,                          (BF_HALTS_BEFORE_OBSTRUCTION)},
    {"entrancement ray",        "casts entrancement",           "can cast entrancement",                    0,      NULL,           &yellow,            BE_ENTRANCEMENT,10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ENEMIES)},
    {"obstruction magic",       "casts obstruction",            "can cast obstruction",                     0,      NULL,           &forceFieldColor,   BE_OBSTRUCTION, 10,             0,          0,          0,                          (BF_HALTS_BEFORE_OBSTRUCTION)},
    {"spell of discord",        "casts a spell of discord",     "can cast discord",                         0,      NULL,           &discordColor,      BE_DISCORD,     10,             0,          0,          MONST_INANIMATE,            (BF_TARGET_ENEMIES)},
    {"conjuration magic",       "casts a conjuration bolt",     "can cast conjuration",                     0,      NULL,           &spectralBladeColor, BE_CONJURATION,10,             0,          0,          MONST_IMMUNE_TO_WEAPONS,    (BF_HALTS_BEFORE_OBSTRUCTION | BF_TARGET_ENEMIES)},
    {"healing magic",           "casts healing",                "can heal $HISHER allies",                  0,      NULL,           &darkRed,           BE_HEALING,     5,              0,          0,          0,                          (BF_TARGET_ALLIES)},
    {"haste spell",             "casts a haste spell",          "can haste $HISHER allies",                 0,      NULL,           &orange,            BE_HASTE,       2,              0,          0,          MONST_INANIMATE,            (BF_TARGET_ALLIES)},
    {"slowing spell",           "casts a slowing spell",        "can slow $HISHER enemies",                 0,      NULL,           &green,             BE_SLOW,        2,              0,          0,          MONST_INANIMATE,            (BF_TARGET_ENEMIES)},
    {"protection magic",        "casts protection",             "can cast protection",                      0,      NULL,           &shieldingColor,    BE_SHIELDING,   5,              0,          0,          MONST_INANIMATE,            (BF_TARGET_ALLIES)},
    {"spiderweb",               "launches a sticky web",        "can launch sticky webs",                   '*',    &white,         NULL,               BE_NONE,        10,             DF_WEB_SMALL, DF_WEB_LARGE, (MONST_IMMOBILE | MONST_IMMUNE_TO_WEBS),   (BF_TARGET_ENEMIES | BF_NEVER_REFLECTS | BF_NOT_LEARNABLE)},
    {"spark",                   "shoots a spark",               "can throw sparks of lightning",            0,      NULL,           &lightningColor,    BE_DAMAGE,      1,              0,          0,          0,                          (BF_PASSES_THRU_CREATURES | BF_TARGET_ENEMIES | BF_ELECTRIC)},
    {"dragonfire",              "breathes a gout of white-hot flame", "can breathe gouts of white-hot flame", 0,    NULL,           &dragonFireColor,   BE_DAMAGE,      18,             DF_OBSIDIAN, 0,         MONST_IMMUNE_TO_FIRE,       (BF_TARGET_ENEMIES | BF_FIERY | BF_NOT_LEARNABLE)},
    {"arrow",                   "shoots an arrow",              "attacks from a distance",                  WEAPON_CHAR, &gray,     NULL,               BE_ATTACK,      1,              0,          0,          MONST_IMMUNE_TO_WEAPONS,    (BF_TARGET_ENEMIES | BF_NEVER_REFLECTS | BF_NOT_LEARNABLE)},
    {"poisoned dart",           "fires a dart",                 "fires strength-sapping darts",             WEAPON_CHAR, &centipedeColor, NULL,         BE_ATTACK,      1,              0,          0,          0,                          (BF_TARGET_ENEMIES | BF_NEVER_REFLECTS | BF_NOT_LEARNABLE)},
    {"acid spray",              "sprays a stream of acid",      "sprays streams of acid",                   '*',    &acidBackColor, NULL,               BE_ATTACK,      1,              0,          0,          0,                          (BF_TARGET_ENEMIES | BF_NEVER_REFLECTS | BF_NOT_LEARNABLE)},
    {"growing vines",           "releases carnivorous vines into the ground", "conjures carnivorous vines", GRASS_CHAR, &tanColor,  NULL,               BE_NONE,        5,              DF_ANCIENT_SPIRIT_GRASS, DF_ANCIENT_SPIRIT_VINES, (MONST_INANIMATE | MONST_IMMUNE_TO_WEBS),   (BF_TARGET_ENEMIES | BF_NEVER_REFLECTS)},
    {"whip",                    "whips",                        "wields a whip",                            '*',    &tanColor,      NULL,               BE_ATTACK,      1,              0,          0,          MONST_IMMUNE_TO_WEAPONS,    (BF_TARGET_ENEMIES | BF_NEVER_REFLECTS | BF_NOT_LEARNABLE | BF_DISPLAY_CHAR_ALONG_LENGTH)},
};

#pragma mark Feat definitions

feat featTable[FEAT_COUNT] = {
    {"Pure Mage",       "Ascend without using fists or a weapon.", true},
    {"Pure Warrior",    "Ascend without using a staff, wand or charm.", true},
    {"Pacifist",        "Ascend without attacking a creature.", true},
    {"Archivist",       "Ascend without drinking a potion or reading a scroll.", true},
    {"Companion",       "Journey with an ally through 20 depths.", false},
    {"Specialist",      "Enchant an item up to or above +16.", false},
    {"Jellymancer",     "Obtain at least 90 jelly allies simultaneously.", false},
    {"Indomitable",     "Ascend without taking damage.", true},
    {"Mystic",          "Ascend without eating.", true},
    {"Dragonslayer",    "Kill a dragon with a melee attack.", false},
    {"Paladin",         "Ascend without attacking an unaware or fleeing creature.", true},
};

#pragma mark Miscellaneous definitions

char monsterBehaviorFlagDescriptions[32][COLS] = {
    "is invisible",                             // MONST_INVISIBLE
    "is an inanimate object",                   // MONST_INANIMATE
    "cannot move",                              // MONST_IMMOBILE
    "",                                         // MONST_CARRY_ITEM_100
    "",                                         // MONST_CARRY_ITEM_25
    "",                                         // MONST_ALWAYS_HUNTING
    "flees at low health",                      // MONST_FLEES_NEAR_DEATH
    "",                                         // MONST_ATTACKABLE_THRU_WALLS
    "corrodes weapons when hit",                // MONST_DEFEND_DEGRADE_WEAPON
    "is immune to weapon damage",               // MONST_IMMUNE_TO_WEAPONS
    "flies",                                    // MONST_FLIES
    "moves erratically",                        // MONST_FLITS
    "is immune to fire",                        // MONST_IMMUNE_TO_FIRE
    "",                                         // MONST_CAST_SPELLS_SLOWLY
    "cannot be entangled",                      // MONST_IMMUNE_TO_WEBS
    "can reflect magic spells",                 // MONST_REFLECT_4
    "never sleeps",                             // MONST_NEVER_SLEEPS
    "burns unceasingly",                        // MONST_FIERY
    "is invulnerable",                          // MONST_INVULNERABLE
    "is at home in water",                      // MONST_IMMUNE_TO_WATER
    "cannot venture onto dry land",             // MONST_RESTRICTED_TO_LIQUID
    "submerges",                                // MONST_SUBMERGES
    "keeps $HISHER distance",                   // MONST_MAINTAINS_DISTANCE
    "",                                         // MONST_WILL_NOT_USE_STAIRS
    "is animated purely by magic",              // MONST_DIES_IF_NEGATED
    "",                                         // MONST_MALE
    "",                                         // MONST_FEMALE
    "",                                         // MONST_NOT_LISTED_IN_SIDEBAR
    "moves only when activated",                // MONST_GETS_TURN_ON_ACTIVATION
};

char monsterAbilityFlagDescriptions[32][COLS] = {
    "can induce hallucinations",                // MA_HIT_HALLUCINATE
    "can steal items",                          // MA_HIT_STEAL_FLEE
    "can possess $HISHER summoned allies",      // MA_ENTER_SUMMONS
    "corrodes armor when $HESHE hits",          // MA_HIT_DEGRADE_ARMOR
    "can summon allies",                        // MA_CAST_SUMMON
    "immobilizes $HISHER prey",                 // MA_SEIZES
    "injects poison when $HESHE hits",          // MA_POISONS
    "",                                         // MA_DF_ON_DEATH
    "divides in two when struck",               // MA_CLONE_SELF_ON_DEFEND
    "dies when $HESHE attacks",                 // MA_KAMIKAZE
    "recovers health when $HESHE inflicts damage",// MA_TRANSFERENCE
    "saps strength when $HESHE inflicts damage",// MA_CAUSE_WEAKNESS
    
    "attacks up to two opponents in a line",    // MA_ATTACKS_PENETRATE
    "attacks all adjacent opponents at once",   // MA_ATTACKS_ALL_ADJACENT
    "attacks enemies at a distance",            // MA_ATTACKS_EXTEND
    "avoids attacking in corridors in a group", // MA_AVOID_CORRIDORS
};

char monsterBookkeepingFlagDescriptions[32][COLS] = {
    "",                                         // MB_WAS_VISIBLE
    "is telepathically bonded with you",        // MB_TELEPATHICALLY_REVEALED
    "",                                         // MB_PREPLACED
    "",                                         // MB_APPROACHING_UPSTAIRS
    "",                                         // MB_APPROACHING_DOWNSTAIRS
    "",                                         // MB_APPROACHING_PIT
    "",                                         // MB_LEADER
    "",                                         // MB_FOLLOWER
    "",                                         // MB_CAPTIVE
    "has been immobilized",                     // MB_SEIZED
    "is currently holding $HISHER prey immobile",// MB_SEIZING
    "is submerged",                             // MB_SUBMERGED
    "",                                         // MB_JUST_SUMMONED
    "",                                         // MB_WILL_FLASH
    "is anchored to reality by $HISHER summoner",// MB_BOUND_TO_LEADER
};
