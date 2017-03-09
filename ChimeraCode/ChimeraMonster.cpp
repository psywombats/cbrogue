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

ChimeraMonster::ChimeraMonster(Body &body) :
        hp(0),
        genFlags(0),
        abilFlags(0),
        flags(0),
        displayChar('x'),
        accuracy(AccuracyType::NORMAL),
        defense(DefenseType::NORMAL),
        bloodType(DF_NONE),
        feature(DF_NONE),
        featureKamikaze(false),
        featurePeriodicPercent(0),
        featureMessage(""),
        damage({0, 0, 0}),
        displayColor(&black),
        lightType(NO_LIGHT),
        body(body),
        dangerLevel(0),
        gender(GenderType::NONE),
        regenSpeed(RegenSpeedType::NORMAL),
        moveSpeed(MoveSpeedType::NORMAL),
        attackSpeed(AttackSpeedType::NORMAL) {

    if (ChimeraMonster::nextChimeraId == 0) {
        ChimeraMonster::nextChimeraId = getWardenMonsterId() + 1;
    }
    this->monsterId = ChimeraMonster::nextChimeraId;
    ChimeraMonster::nextChimeraId += 1;

    body.applyToMonster(*this);
}

ChimeraMonster::~ChimeraMonster() {

}

creatureType ChimeraMonster::convertToStruct() {
    creatureType creatureStruct = creatureType();
    
    this->damage.lowerBound = MAX(damage.lowerBound, 0);

    creatureStruct.monsterID = this->monsterId;
    memcpy(&creatureStruct.monsterName, this->name.c_str(), this->name.length()+1);
    creatureStruct.displayChar = this->displayChar;
    creatureStruct.foreColor = this->displayColor;
    creatureStruct.maxHP = this->hp;
    memcpy(&creatureStruct.damage, &this->damage, sizeof(randomRange));
    creatureStruct.turnsBetweenRegen = regenSpeedToTurnsPerRegen(this->regenSpeed);
    creatureStruct.movementSpeed = moveSpeedToTicksPerMove(this->moveSpeed);
    creatureStruct.attackSpeed = attackSpeedToTicksPerAttack(this->attackSpeed);
    creatureStruct.bloodType = this->bloodType;
    creatureStruct.intrinsicLightType = this->lightType;

    AbsorbFlavorType absorb = this->generateAbsorbFlavor();
    std::string flavor = this->generateFlavor();
    memcpy(&creatureStruct.flavorText, flavor.c_str(), flavor.length()+1);
    memcpy(&creatureStruct.absorbStatus, absorb.status.c_str(), absorb.status.length()+1);
    memcpy(&creatureStruct.absorbing, absorb.message.c_str(), absorb.message.length()+1);

    if (this->hp < 10) {
        creatureStruct.accuracy = 70;
    } else if (this->hp < 19) {
        creatureStruct.accuracy = 85;
    } else if (this->hp < 50) {
        creatureStruct.accuracy = 100;
    } else if (this->hp < 75) {
        creatureStruct.accuracy = 125;
    } else if (this->hp < 95){
        creatureStruct.accuracy = 150;
    } else {
        creatureStruct.accuracy = 225;
    }
    if (accuracy == AccuracyType::ACCURATE) {
        creatureStruct.accuracy = (short)((float)creatureStruct.accuracy * 1.5f);
    }
    if (accuracy == AccuracyType::INACCURATE) {
        creatureStruct.accuracy -= 30;
    }

    if (this->hp < 10) {
        creatureStruct.defense = 0;
    } else if (this->hp < 50) {
        creatureStruct.defense = 20;
    } else if (this->hp < 70) {
        creatureStruct.defense = 55;
    } else if (this->hp < 95) {
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

    creatureStruct.flags = this->flags;
    creatureStruct.abilityFlags = this->abilFlags;

    for (unsigned int i = 0; i < this->bolts.size(); i += 1) {
        creatureStruct.bolts[i] = this->bolts[i];
        i += 1;
    }

    if (this->genFlags & GF_SUPPORTS_CLASS) {
        // TODO: do this more smartlier
        creatureStruct.abilityFlags |= MA_AVOID_CORRIDORS;
    }
    
    if (this->genFlags & GF_AQUATIC_ONLY) {
        creatureStruct.flags |= MONST_RESTRICTED_TO_LIQUID | MONST_NEVER_SLEEPS;
    }
    
    creatureStruct.DFType = this->feature;
    memcpy(&creatureStruct.DFMessage, this->featureMessage.c_str(), this->featureMessage.length()+1);
    creatureStruct.DFChance = this->featurePeriodicPercent;
    
    if (this->bloodType == DF_NONE) {
        if ((this->genFlags & GF_ANIMAL) > 0) {
            this->bloodType = DF_RED_BLOOD;
        } else if ((this->genFlags & GF_INSECTOID) > 0) {
            this->bloodType = DF_WORM_BLOOD;
        } else if ((this->genFlags & GF_AMORPHOUS) > 0) {
            this->bloodType = DF_GREEN_BLOOD;
        }
    }

    // TODO
    //short DFChance;                     // percent chance to spawn the dungeon feature per awake turn
    //char summonMessage[DCOLS * 2];

    return creatureStruct;
}

void ChimeraMonster::applyAbility(Ability &ability) {
    ability.applyToMonster(*this);
    this->abilities.push_back(ability);
    specializeName();
}

std::string ChimeraMonster::debugReport() const {
    std::string report = "";

    report += name + " (id " + printInt(monsterId) + " DL " + printInt(dangerLevel) + ")\n";
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

    if ((this->flags & MONST_FLEES_NEAR_DEATH) > 0) report += "[flees] ";
    if ((this->flags & MONST_CAST_SPELLS_SLOWLY) > 0) report += "[slowspells] ";
    if ((this->flags & MONST_CARRY_ITEM_100) > 0) report += "[item100] ";
    if ((this->flags & MONST_CARRY_ITEM_25) > 0) report += "[item] ";
    if ((this->flags & MONST_DEFEND_DEGRADE_WEAPON) > 0) report += "[acid def] ";
    if ((this->flags & MONST_ALWAYS_HUNTING) > 0) report += "[nosleep] ";
    if ((this->flags & MONST_ALWAYS_USE_ABILITY) > 0) report += "[abilonly] ";
    if ((this->flags & MONST_DIES_IF_NEGATED) > 0) report += "[negatedeath] ";
    if ((this->flags & MONST_FIERY) > 0) report += "[fiery] ";
    if ((this->flags & MONST_FLIES) > 0) report += "[flies] ";
    if ((this->flags & MONST_FLITS) > 0) report += "[flits] ";
    if ((this->flags & MONST_IMMUNE_TO_FIRE) > 0) report += "[fire immune] ";
    if ((this->flags & MONST_IMMUNE_TO_WATER) > 0) report += "[water immune] ";
    if ((this->flags & MONST_IMMUNE_TO_WEAPONS) > 0) report += "[weapon immune] ";
    if ((this->flags & MONST_IMMUNE_TO_WEBS) > 0) report += "[web immune] ";
    if ((this->flags & MONST_INVISIBLE) > 0) report += "[invisible] ";
    if ((this->flags & MONST_MAINTAINS_DISTANCE) > 0) report += "[keeps distance] ";
    if ((this->flags & MONST_REFLECT_4) > 0) report += "[reflective] ";
    if ((this->flags & MONST_RESTRICTED_TO_LIQUID) > 0) report += "[liquids only] ";
    if ((this->flags & MONST_SUBMERGES) > 0) report += "[submerges] ";

    if ((this->abilFlags & MA_HIT_HALLUCINATE) > 0) report += ".hallucinative.";
    if ((this->abilFlags & MA_HIT_STEAL_FLEE) > 0) report += ".thief.";
    if ((this->abilFlags & MA_ENTER_SUMMONS) > 0) report += ".summon transforms.";
    if ((this->abilFlags & MA_HIT_DEGRADE_ARMOR) > 0) report += ".acid atk.";
    if ((this->abilFlags & MA_CAST_SUMMON) > 0) report += ".summons.";
    if ((this->abilFlags & MA_SEIZES) > 0) report += ".grappler.";
    if ((this->abilFlags & MA_POISONS) > 0) report += ".poisons.";
    if ((this->abilFlags & MA_DF_ON_DEATH) > 0) report += ".death effect.";
    if ((this->abilFlags & MA_CLONE_SELF_ON_DEFEND) > 0) report += ".splits.";
    if ((this->abilFlags & MA_KAMIKAZE) > 0) report += ".kamikaze.";
    if ((this->abilFlags & MA_TRANSFERENCE) > 0) report += ".vampiric.";
    if ((this->abilFlags & MA_CAUSES_WEAKNESS) > 0) report += ".weakens.";
    if ((this->abilFlags & MA_ATTACKS_PENETRATE) > 0) report += ".penetrate atk.";
    if ((this->abilFlags & MA_ATTACKS_ALL_ADJACENT) > 0) report += ".axe atk.";
    if ((this->abilFlags & MA_ATTACKS_EXTEND) > 0) report += ".whip atk.";
    if ((this->abilFlags & MA_AVOID_CORRIDORS) > 0) report += ".nohalls.";

    if (this->featureKamikaze) report += ".kamikaze ";
    if (this->featurePeriodicPercent > 0) report += ".periodic ";
    
    
    report += "\n";

    return report;
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

std::string ChimeraMonster::generateFlavor() {
    return "a monster";
}

AbsorbFlavorType ChimeraMonster::generateAbsorbFlavor() {
    AbsorbFlavorType flavor;
    flavor.message = "absorbing";
    flavor.status = "Absorbing";
    return flavor;
}

std::list<std::string> ChimeraMonster::generateAttackFlavor() {
    std::list<std::string> list;
    list.push_back("hits");
    return list;
}

std::string ChimeraMonster::generateSummonFlavor() {
    return "summons allies.";
}

void ChimeraMonster::specializeName() {
    std::string nameBefore = this->name;
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
    if (name == "mounted goblin")           name = "warg rider";
    if (name == "thieving ferret")          name = "ferret";
    if (name == "thieving monkey")          name = "monkey";
    if (name == "thieving raven")           name = "raven";
    if (name == "thieving magpie")          name = "magpie";
    if (name == "impish quasit")            name = "imp";
    if (name == "tentacle mollusk")         name = "kraken";
    if (name == "winged quasit")            name = "gargoyle";
    if (name == "acolyte idol")             name = "demonic idol";
    if (name == "acolyte obelisk")          name = "obsidian obelisk";
    if (name == "onyx acolyte")             name = "angel statue";
    
    if (nameBefore != this->name && (genFlags & GF_TOTEM)) {
        if (isupper(this->displayChar)) {
            this->displayChar = toupper(name[0]);
        } else {
            this->displayChar = tolower(name[0]);
        }
    }
}

