/*
 * ChimeraMonster.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "ChimeraMonster.h"
#include "Body.h"
#include "IncludeGlobals.h"
#include "MonsterGlobals.h"
#include "Ability.h"
#include <stdlib.h>

int ChimeraMonster::nextChimeraId = 0;
std::map<std::reference_wrapper<uchar>, std::string> ChimeraMonster::usedChars;

static std::vector<std::string> flavorWeaponNames = {   "greatsword", "battleaxe", "broadsword", "lance", "flail", "warhammer", "waraxe", "saber", "scythe",
                                                        "claymore", "zweihander", "bardiche", "poleaxe", "mace", "morning star", "pike", "trident", "glaive",
                                                        "halberd", "partisan", "voulge", "broad-bladed sword", "dull broadsword", "broken greatsword",
                                                        "ancient claymore", "stone sword", "iron blade", "two-handed scimitar", "chipped longsword" };
static std::vector<std::string> flavorMagicNames = {    "orb", "sphere", "ring", "ankh", "cross", "sceptre", "wand", "staff", "book", "tome", "iron tome",
                                                        "metal skull", "wooden skull", "candelabra", "incense burner", "iron rod", "strand of beads",
                                                        "clay icon", "statuette", "golden chalice", "single golden coin", "faded scroll", "silver shortsword",
                                                        "silver dagger", "ox skull", "ceremonial mace", "torch", "torque", "stone codex" };
static std::vector<std::string> flavorSwordNames = {    "rapier", "blade", "sword", "shortsword", "longsword", "dirk", "scimitar", "saber" };
static std::vector<std::string> fodderNames =      {    "magpie", "rat", "songbird", "ferret", "jay", "crow", "spider", "hand", "wisp", "globe", "sphere" };

ChimeraMonster::ChimeraMonster(Body &newBody) :
        hp(0),
        genFlags(0),
        abilFlags(0),
        flags(0),
        baseMook(NULL),
        baseMookName(""),
        displayName(""),
        nameSuffix(""),
        namePrefix(""),
        baseFlavor(""),
        summonMessage(""),
        displayChar('?'),
        baseDisplayChar('?'),
        accuracy(AccuracyType::NORMAL),
        defense(DefenseType::NORMAL),
        physique(PhysiqueType::NORMAL),
        bloodType(DF_NONE),
        feature(DF_NONE),
        summon(SummonType::NONE),
        featureKamikaze(false),
        featurePeriodicPercent(0),
        featureMessage(""),
        damage({0, 0, 0}),
        displayColor(&black),
        lightType(NO_LIGHT),
        body(newBody),
        dangerLevel(0),
        gender(GenderType::NONE),
        regenSpeed(RegenSpeedType::NORMAL),
        moveSpeed(MoveSpeedType::NORMAL),
        attackSpeed(AttackSpeedType::NORMAL) {

    if (ChimeraMonster::nextChimeraId == 0) {
        reset();
    }
    monsterId = ChimeraMonster::nextChimeraId;
    ChimeraMonster::nextChimeraId += 1;

    body.applyToMonster(*this);
}

ChimeraMonster::~ChimeraMonster() {
    
}

creatureType ChimeraMonster::convertToStruct() {
    creatureType creatureStruct = creatureType();
    
    memcpy(&creatureStruct.damage, &damage, sizeof(randomRange));
    
    generateName();
    generateDisplayChar();
    generateFlavor();
    AbsorbFlavorType absorb = generateAbsorbFlavor();
    strcpy(creatureStruct.monsterName, displayName.c_str());
    strcpy(creatureStruct.flavorText, flavor.c_str());
    strcpy(creatureStruct.absorbStatus, absorb.status.c_str());
    strcpy(creatureStruct.absorbing, absorb.message.c_str());
    strcpy(creatureStruct.summonMessage, summonMessage.c_str());

    creatureStruct.monsterID = monsterId;
    creatureStruct.displayChar = displayChar;
    creatureStruct.foreColor = displayColor;
    creatureStruct.maxHP = hp;
    creatureStruct.turnsBetweenRegen = regenSpeedToTurnsPerRegen(regenSpeed);
    creatureStruct.movementSpeed = moveSpeedToTicksPerMove(moveSpeed);
    creatureStruct.attackSpeed = attackSpeedToTicksPerAttack(attackSpeed);
    creatureStruct.bloodType = bloodType;
    creatureStruct.intrinsicLightType = lightType;
    
    for (unsigned long i = 0; i < this->hitMessages.size(); i += 1) {
        strcpy(creatureStruct.attack[i], hitMessages[i].c_str());
    }
    
    if (hp < 10) {
        creatureStruct.accuracy = 70;
    } else if (hp < 19) {
        creatureStruct.accuracy = 85;
    } else if (hp < 50) {
        creatureStruct.accuracy = 100;
    } else if (hp < 75) {
        creatureStruct.accuracy = 125;
    } else if (hp < 95){
        creatureStruct.accuracy = 150;
    } else {
        creatureStruct.accuracy = 200;
    }
    if (accuracy == AccuracyType::ACCURATE) {
        creatureStruct.accuracy = (short)((float)creatureStruct.accuracy * 1.5f);
    } else if (accuracy == AccuracyType::INACCURATE) {
        creatureStruct.accuracy -= 30;
    } else if (accuracy == AccuracyType::FIXED) {
        creatureStruct.accuracy = 150;
    }

    if (hp < 10) {
        creatureStruct.defense = 0;
    } else if (hp < 50) {
        creatureStruct.defense = 20;
    } else if (hp < 70) {
        creatureStruct.defense = 55;
    } else if (hp < 95) {
        creatureStruct.defense = 70;
    } else {
        creatureStruct.defense = 90;
    }
    if (defense == DefenseType::DEFENSELESS) {
        creatureStruct.defense = 0;
    }
    if (defense == DefenseType::HIGH) {
        creatureStruct.defense += 50;
    }
    if (defense == DefenseType::LOW) {
        creatureStruct.defense /= 2;
    }

    creatureStruct.flags = flags;
    creatureStruct.abilityFlags = abilFlags;

    for (unsigned int i = 0; i < bolts.size(); i += 1) {
        creatureStruct.bolts[i] = bolts[i];
    }

    if (genFlags & GF_SUPPORTS_CLASS) {
        // TODO: do this more smartlier
        creatureStruct.abilityFlags |= MA_AVOID_CORRIDORS;
    }
    if (genFlags & GF_AQUATIC_ONLY) {
        creatureStruct.flags |= MONST_RESTRICTED_TO_LIQUID | MONST_NEVER_SLEEPS;
    }
    
    creatureStruct.DFType = feature;
    strcpy(creatureStruct.DFMessage, featureMessage.c_str());
    creatureStruct.DFChance = featurePeriodicPercent;
    if (featureKamikaze) {
        creatureStruct.abilityFlags |= (MA_DF_ON_DEATH | MA_KAMIKAZE);
    }
    
    if (bloodType == DF_NONE) {
        if ((genFlags & GF_ANIMAL) > 0) {
            bloodType = DF_RED_BLOOD;
        } else if ((genFlags & GF_INSECTOID) > 0) {
            bloodType = DF_WORM_BLOOD;
        } else if ((genFlags & GF_AMORPHOUS) > 0) {
            bloodType = DF_GREEN_BLOOD;
        }
    }
    
    if (summon != SummonType::NONE) {
        creatureStruct.abilityFlags |= MA_CAST_SUMMON;
        if (summon == SummonType::TRANSFORM_MOOK) {
            creatureStruct.abilityFlags |= MA_ENTER_SUMMONS;
        }
    }
    
    creatureStruct.danger = (short)dangerLevel;

    return creatureStruct;
}

void ChimeraMonster::applyAbility(Ability &ability) {
    ability.applyToMonster(*this);
    abilities.push_back(ability);
}

std::string ChimeraMonster::debugReport() {
    generateName();
    generateFlavor();
    generateDisplayChar();
    
    std::string report = std::string(1, displayChar);
    
    report += " " + displayName + " (id " + printInt(monsterId) + " DL " + printInt(dangerLevel) + ")\n";
    report += "  HP: " + printInt(hp) + "  dmg: " + printInt(damage.lowerBound) + "-" + printInt(damage.upperBound) + "\n";
    report += "  Specials: ";
    if (accuracy == AccuracyType::ACCURATE) report += "(accurate) ";
    if (accuracy == AccuracyType::INACCURATE) report += "(inaccurate) ";
    if (defense == DefenseType::DEFENSELESS) report += "(defenseless) ";
    if (defense == DefenseType::LOW) report += "(low def) ";
    if (defense == DefenseType::HIGH) report += "(hi def) ";
    if (moveSpeed == MoveSpeedType::FAST) report += "(fast move) ";
    if (moveSpeed == MoveSpeedType::SLOW) report += "(slow move) ";
    if (moveSpeed == MoveSpeedType::VERY_SLOW) report += "(v.slow move) ";
    if (attackSpeed == AttackSpeedType::SLOW) report += "(slow attack) ";
    if (attackSpeed == AttackSpeedType::TURRET) report += "(turret attack) ";
    if (attackSpeed == AttackSpeedType::TOTEM) report += "(totem attack) ";
    if (regenSpeed == RegenSpeedType::EXTREMELY_FAST) report += "(e.fast regen) ";
    if (regenSpeed == RegenSpeedType::FAST) report += "(fast regen) ";
    if (regenSpeed == RegenSpeedType::VERY_FAST) report += "(v.fast regen) ";

    for (boltType bolt : bolts) {
        report += "{" + ChimeraMonster::boltToString(bolt) + "} ";
    }

    if ((flags & MONST_FLEES_NEAR_DEATH) > 0) report += "[flees] ";
    if ((flags & MONST_CAST_SPELLS_SLOWLY) > 0) report += "[slowspells] ";
    if ((flags & MONST_CARRY_ITEM_100) > 0) report += "[item100] ";
    if ((flags & MONST_CARRY_ITEM_25) > 0) report += "[item] ";
    if ((flags & MONST_DEFEND_DEGRADE_WEAPON) > 0) report += "[acid def] ";
    if ((flags & MONST_ALWAYS_HUNTING) > 0) report += "[nosleep] ";
    if ((flags & MONST_ALWAYS_USE_ABILITY) > 0) report += "[abilonly] ";
    if ((flags & MONST_DIES_IF_NEGATED) > 0) report += "[negatedeath] ";
    if ((flags & MONST_FIERY) > 0) report += "[fiery] ";
    if ((flags & MONST_FLIES) > 0) report += "[flies] ";
    if ((flags & MONST_FLITS) > 0) report += "[flits] ";
    if ((flags & MONST_IMMUNE_TO_FIRE) > 0) report += "[fire immune] ";
    if ((flags & MONST_IMMUNE_TO_WATER) > 0) report += "[water immune] ";
    if ((flags & MONST_IMMUNE_TO_WEAPONS) > 0) report += "[weapon immune] ";
    if ((flags & MONST_IMMUNE_TO_WEBS) > 0) report += "[web immune] ";
    if ((flags & MONST_INVISIBLE) > 0) report += "[invisible] ";
    if ((flags & MONST_MAINTAINS_DISTANCE) > 0) report += "[keeps distance] ";
    if ((flags & MONST_REFLECT_4) > 0) report += "[reflective] ";
    if ((flags & MONST_RESTRICTED_TO_LIQUID) > 0) report += "[liquids only] ";
    if ((flags & MONST_SUBMERGES) > 0) report += "[submerges] ";

    if ((abilFlags & MA_HIT_HALLUCINATE) > 0) report += ".hallucinative.";
    if ((abilFlags & MA_HIT_STEAL_FLEE) > 0) report += ".thief.";
    if ((abilFlags & MA_ENTER_SUMMONS) > 0) report += ".summon transforms.";
    if ((abilFlags & MA_HIT_DEGRADE_ARMOR) > 0) report += ".acid atk.";
    if ((abilFlags & MA_CAST_SUMMON) > 0) report += ".summons.";
    if ((abilFlags & MA_SEIZES) > 0) report += ".grappler.";
    if ((abilFlags & MA_POISONS) > 0) report += ".poisons.";
    if ((abilFlags & MA_DF_ON_DEATH) > 0) report += ".death effect.";
    if ((abilFlags & MA_CLONE_SELF_ON_DEFEND) > 0) report += ".splits.";
    if ((abilFlags & MA_KAMIKAZE) > 0) report += ".kamikaze.";
    if ((abilFlags & MA_TRANSFERENCE) > 0) report += ".vampiric.";
    if ((abilFlags & MA_CAUSES_WEAKNESS) > 0) report += ".weakens.";
    if ((abilFlags & MA_ATTACKS_PENETRATE) > 0) report += ".penetrate atk.";
    if ((abilFlags & MA_ATTACKS_ALL_ADJACENT) > 0) report += ".axe atk.";
    if ((abilFlags & MA_ATTACKS_EXTEND) > 0) report += ".whip atk.";
    if ((abilFlags & MA_AVOID_CORRIDORS) > 0) report += ".nohalls.";

    if (featureKamikaze) report += ".kamikaze.";
    if (featurePeriodicPercent > 0) report += ".periodic.";
    
    
    report += "\n";

    return report;
}

const std::string &ChimeraMonster::getDisplayName() const {
    return displayName;
}

std::string ChimeraMonster::boltToString(boltType bolt) {
    switch (bolt) {
        case BOLT_NONE:                         return "None";
        case BOLT_TELEPORT:                     return "Teleport";
        case BOLT_SLOW:                         return "Slow";
        case BOLT_POLYMORPH:                    return "Polymorph";
        case BOLT_NEGATION:                     return "Negation";
        case BOLT_DOMINATION:                   return "Domination";
        case BOLT_BECKONING:                    return "Beckoning";
        case BOLT_PLENTY:                       return "Plenty";
        case BOLT_INVISIBILITY:                 return "Invisibility";
        case BOLT_EMPOWERMENT:                  return "Empowerment";
        case BOLT_LIGHTNING:                    return "Lightning";
        case BOLT_FIRE:                         return "Fire";
        case BOLT_POISON:                       return "Poison";
        case BOLT_TUNNELING:                    return "Tunneling";
        case BOLT_BLINKING:                     return "Blinking";
        case BOLT_ENTRANCEMENT:                 return "Entrancement";
        case BOLT_OBSTRUCTION:                  return "Obstruction";
        case BOLT_DISCORD:                      return "Discord";
        case BOLT_CONJURATION:                  return "Conjuration";
        case BOLT_HEALING:                      return "Healing";
        case BOLT_HASTE:                        return "Haste";
        case BOLT_SLOW_2:                       return "Slow2";
        case BOLT_SHIELDING:                    return "Shielding";
        case BOLT_SPIDERWEB:                    return "Web";
        case BOLT_SPARK:                        return "Spark";
        case BOLT_DRAGONFIRE:                   return "Dragonfire";
        case BOLT_DISTANCE_ATTACK:              return "Arrow";
        case BOLT_POISON_DART:                  return "Dart";
        case BOLT_ACID_TURRET_ATTACK:           return "Acid";
        case BOLT_ANCIENT_SPIRIT_VINES:         return "Vines";
        case BOLT_WHIP:                         return "Whip";
        default:                                return printInt(bolt);
    }
    return "wat";
}

std::string ChimeraMonster::dungeonFeatureToString(dungeonFeatureTypes feature) {
    switch (feature) {
        case DF_BLOAT_DEATH:                    return "caustic gas";
        case DF_HOLE_POTION:                    return "pit";
        case DF_BLOAT_EXPLOSION:                return "asplode";
        case DF_MUTATION_LICHEN:                return "lichen";
        case DF_INCINERATION_POTION:            return "fire";
        case DF_SHATTERING_SPELL:               return "shatter";
        case DF_DEWAR_METHANE:                  return "explosive gas";
        case DF_CONFUSION_GAS_CLOUD_POTION:     return "confusion gas";
        default:                                return printInt(feature);
    }
    return "wat";
}

short ChimeraMonster::regenSpeedToTurnsPerRegen(RegenSpeedType speed) {
    switch(speed) {
        case RegenSpeedType::EXTREMELY_FAST:    return 1;
        case RegenSpeedType::VERY_FAST:         return 5;
        case RegenSpeedType::FAST:              return 10;
        case RegenSpeedType::NORMAL:            return 20;
        case RegenSpeedType::NONE:              return 0;
    }
    return 0;
}

short ChimeraMonster::moveSpeedToTicksPerMove(MoveSpeedType speed) {
    switch(speed) {
        case MoveSpeedType::FAST:               return 50;
        case MoveSpeedType::NORMAL:             return 100;
        case MoveSpeedType::SLOW:               return 150;
        case MoveSpeedType::VERY_SLOW:          return 200;
    }
    return 0;
}

short ChimeraMonster::attackSpeedToTicksPerAttack(AttackSpeedType speed) {
    switch(speed) {
        case AttackSpeedType::NORMAL:           return 100;
        case AttackSpeedType::SLOW:             return 200;
        case AttackSpeedType::TURRET:           return 250;
        case AttackSpeedType::TOTEM:            return 350;
    }
    return 0;
}

void ChimeraMonster::generateFlavor() {
    flavor = baseFlavor;
    for (Ability &ability : abilities) {
        if (ability.flavorOverride.length() > 0) {
            flavor = ability.flavorOverride;
        } else if (ability.flavorAddition.length() > 0) {
            flavor += " " + ability.flavorAddition;
        }
    }
    replace(flavor, "$BASE", getBaseName());
    replace(flavor, "$WEAPON", flavorWeaponNames[rand_range(0, flavorWeaponNames.size() - 1)]);
    replace(flavor, "$MAGIC", flavorMagicNames[rand_range(0, flavorMagicNames.size() - 1)]);
    replace(flavor, "$SWORD", flavorSwordNames[rand_range(0, flavorSwordNames.size() - 1)]);
    replace(flavor, "$FODDER", fodderNames[rand_range(0, fodderNames.size() - 1)]);
}

void ChimeraMonster::replace(std::string &source, const std::string &token, const std::string &replacement) {
    size_t startPos = 0;
    while ((startPos = source.find(token, startPos)) != std::string::npos) {
        source.replace(startPos, token.length(), replacement);
        startPos += replacement.length();
    }
}

AbsorbFlavorType ChimeraMonster::generateAbsorbFlavor() const {
    AbsorbFlavorType flavor;
    flavor.message = "absorbing";
    flavor.status = "Absorbing";
    return flavor;
}

std::list<std::string> ChimeraMonster::generateAttackFlavor() const {
    std::list<std::string> list;
    list.push_back("hits");
    return list;
}

std::string ChimeraMonster::generateSummonFlavor() const {
    return "summons allies.";
}

void ChimeraMonster::generateName() {
    if (displayName.length() > 0) {
        return;
    }
    
    std::string name = baseName;
    if (namePrefix.length() > 0) {
        name = namePrefix + " " + name;
    }
    if (nameSuffix.length() > 0) {
        name += " " + nameSuffix;
    }
    
    replace(name, "$BASE", getBaseName());
    
    std::string weapon = "";
    while (weapon.length() <= 0 || weapon.length() > 10) {
        weapon = flavorWeaponNames[rand_range(0, flavorWeaponNames.size() - 1)];
    }
    replace(name, "$WEAPON", weapon);
    replace(name, "$MAGIC", flavorMagicNames[rand_range(0, flavorMagicNames.size() - 1)]);
    replace(name, "$SWORD", flavorSwordNames[rand_range(0, flavorSwordNames.size() - 1)]);
    replace(name, "$FODDER", fodderNames[rand_range(0, fodderNames.size() - 1)]);
    
    if (name == "caustic bloat")            name = "bloat";
    if (name == "vampire jackal")           name = "chupacabra";
    if (name == "vampire dog")              name = "chupacabra";
    if (name == "vampire hyena")            name = "chupacabra";
    if (name == "infested spore crab")      name = "spore crab";
    if (name == "explosive kamikaze dwarf") name = "kamikaze dwarf";
    if (name == "lava rat")                 name = "hell rat";
    if (name == "cinder rat")               name = "hell rat";
    if (name == "sticky horror")            name = "tentacle horror";
    if (name == "rainbow frog")             name = "toad";
    if (name == "lizardman dominatrix")     name = "salamander";
    if (name == "lizardman axeman")         name = "naga";
    if (name == "horseman archer")          name = "centaur";
    if (name == "thieving ferret")          name = "ferret";
    if (name == "thieving monkey")          name = "monkey";
    if (name == "thieving magpie")          name = "magpie";
    if (name == "impish quasit")            name = "imp";
    if (name == "tentacle mollusk")         name = "kraken";
    if (name == "winged quasit")            name = "gargoyle";
    if (name == "acolyte idol")             name = "unholy idol";
    if (name == "acolyte archmage")         name = "archmage";
    if (name == "skeleton lord")            name = "skeletal lord";
    if (name == "skeleton king")            name = "skeletal king";
    if (name == "burrowing worm")           name = "underworm";
    if (name == "monkey bandit")            name = "lemur";
    if (name == "silk arachnid")            name = "spider";
    if (name == "were rat")                 name = "wererat";
    if (name == "were jackal")              name = "werejackal";

    displayName = name;
}

void ChimeraMonster::generateDisplayChar() {
    generateName();
    if (displayChar != '?') {
        return;
    }
    
    if (baseDisplayChar == '?') {
        baseDisplayChar = baseName[0];
    }
    displayChar = baseDisplayChar;
    
    if (otherSpeciesUsesChar() && !body.reusable) {
        if (islower(displayChar)) {
            displayChar = toupper(displayChar);
        } else {
            displayChar = tolower(displayChar);
        }
        while (otherSpeciesUsesChar()) {
            // someone else has BOTH letters? fuck
            uchar randomChars[] = {0x03D7,0x03D6,0x03B6,0x0376,0x03EA,0x03E0,0x054B,0x0556,0x07F7,0x0186,0x0518};
            displayChar = randomChars[rand_range(0, (sizeof(randomChars) / sizeof(uchar)) - 1)];
        }
    }
    usedChars[displayChar] = getBaseName();
}

bool ChimeraMonster::otherSpeciesUsesChar() {
    if (usedChars.count(displayChar)) {
        std::string user = usedChars[displayChar];
        return getBaseName() != user;
    } else {
        return false;
    }
}

const std::string &ChimeraMonster::getBaseName() const {
    if (baseMook != NULL) {
        return baseMook->baseName;
    } else if (baseMookName.length() > 0) {
        return baseMookName;
    } else {
        return baseName;
    }
}

void ChimeraMonster::reset() {
    ChimeraMonster::nextChimeraId = 3;
    ChimeraMonster::usedChars.clear();
}

