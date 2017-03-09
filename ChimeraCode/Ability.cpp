/*
 * Ability.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Ability.h"

Ability::Ability() :
        hpBoost(0),
        rarityPercent(50),
        requiredFlags(0),
        flags(0),
        forbiddenFlags(0),
        abilFlags(0),
        dangerBoost(0),
        minDamageBoost(0),
        maxDamageBoost(0),
        featureKamikaze(DF_NONE),
        featurePeriodic(DF_NONE),
        featureMessage(""),
        featurePeriodicPercent(0),
        regenSpeed(RegenSpeedType::NORMAL),
        moveSpeed(MoveSpeedType::NORMAL),
        attackSpeed(AttackSpeedType::NORMAL),
        accuracy(AccuracyType::NORMAL),
        defense(DefenseType::NORMAL),
        namePrefix(""),
        nameSuffix(""),
        colorOverride(NULL),
        colorMod(ColorModFlavor::NONE),
        light(NO_LIGHT),
        flavorOverride(""),
        flavorAddition(""),
        inUse(false) {

}

Ability::~Ability() {

}

void Ability::applyToMonster(ChimeraMonster &monster) {
    monster.hp += this->hpBoost;
    monster.dangerLevel += this->dangerBoost;
    monster.damage.lowerBound += this->minDamageBoost;
    monster.damage.upperBound += this->maxDamageBoost;

    if (this->regenSpeed != RegenSpeedType::NORMAL) {
        monster.regenSpeed = this->regenSpeed;
    }
    if (this->moveSpeed != MoveSpeedType::NORMAL) {
        monster.moveSpeed = this->moveSpeed;
    }
    if (this->attackSpeed != AttackSpeedType::NORMAL) {
        monster.attackSpeed = this->attackSpeed;
    }
    if (this->accuracy != AccuracyType::NORMAL) {
        monster.accuracy = this->accuracy;
    }
    if (this->defense != DefenseType::NORMAL) {
        monster.defense = this->defense;
    }
    if (this->namePrefix.size() > 0) {
        monster.name = this->namePrefix + " " + monster.name;
    }
    if (this->nameSuffix.size() > 0) {
        monster.name += " " + this->nameSuffix;
    }

    for (boltType bolt : this->bolts) {
        monster.bolts.push_back(bolt);
    }

    monster.flags |= this->flags;
    monster.abilFlags |= this->abilFlags;
    
    if (this->featureKamikaze != DF_NONE) {
        monster.feature = this->featureKamikaze;
        monster.flags |= (MA_DF_ON_DEATH);
        monster.featureMessage = this->featureMessage;
    } else if (this->featurePeriodic != DF_NONE) {
        monster.feature = this->featurePeriodic;
        monster.featurePeriodicPercent = this->featurePeriodicPercent;
        monster.featureMessage = this->featureMessage;
    }
    
    if (colorOverride == NULL) {
        monster.displayColor = this->blendColor(monster.displayColor);
    } else {
        monster.displayColor = this->colorOverride;
    }
    
    if (this->light != NO_LIGHT) {
        monster.lightType = this->light;
    }

    this->inUse = true;
}

bool Ability::validForMonster(const ChimeraMonster &monster) const {
    if (monster.genFlags & GF_NO_SPECIALS) {
        return false;
    }
    if ((this->requiredFlags & monster.genFlags) != this->requiredFlags) {
        return false;
    }
    if ((this->forbiddenFlags & monster.genFlags) > 0) {
        return false;
    }
    if ((this->featureKamikaze != DF_NONE || this->featurePeriodic != DF_NONE) && monster.feature != DF_NONE) {
        return false;
    }
    if ((monster.flags & this->flags) > 0) {
        // this means we'd generate something stupid like a "winged bat" or "mounted horseman"
        return false;
    }
    if (((monster.flags & MONST_FLIES) && this->moveSpeed != MoveSpeedType::NORMAL) ||
            (monster.moveSpeed != MoveSpeedType::NORMAL && (monster.flags & MONST_FLIES))) {
        // flying is incompatible with movement modifiers
        return false;
    }
    if (monster.defense == DefenseType::DEFENSELESS && this->defense != DefenseType::NORMAL) {
        return false;
    }
    if (this->dangerBoost + monster.dangerLevel > 32) {
        // no one would see this monstrosity, it's probably like an explosive horror or something
        return false;
    }

    return true;
}

const color *Ability::blendColor(const color *baseColor) const {
    switch (this->colorMod) {
    case ColorModFlavor::COMBAT:
        if (baseColor == &white) {
            return &gray;
        } else if (baseColor == &brown || baseColor == &gray) {
            return &darkGray;
        } else if (baseColor == &darkGray || baseColor == &darkRed) {
            return &veryDarkGray;
        } else if (baseColor == &tanColor || baseColor == &darkGreen) {
            return &darkBlue;
        } else if (baseColor == &darkYellow) {
            return &darkOrange;
        } else if (baseColor == &darkPurple) {
            return &blue;
        } else {
            return &red;
        }
    case ColorModFlavor::MOBILITY:
        if (baseColor == &white || baseColor == &darkRed) {
            return &orange;
        } else if (baseColor == &darkPurple) {
            return &pink;
        } else {
            return &white;
        }
    case ColorModFlavor::POISONOUS:
        if (baseColor == &darkYellow) {
            return &yellow;
        } else if (baseColor == &darkGreen) {
            return &green;
        } else {
            return &purple;
        }
    case ColorModFlavor::SPELLCASTING:
        if (baseColor == &darkPurple) {
            return &white;
        } else if (baseColor == &brown) {
            return &green;
        } else if (baseColor == &darkGreen) {
            return &pink;
        } else {
            return &teal;
        }
    case ColorModFlavor::FIRE:
        if (baseColor == &darkGray || baseColor == &darkGreen) {
            return &darkRed;
        } else if (baseColor == &darkPurple) {
            return &orange;
        } else {
            return &red;
        }
    case ColorModFlavor::TOTEM:
        if (baseColor == &brown || baseColor == &tanColor) {
            return &orange;
        } else if (baseColor == &darkGreen) {
            return &green;
        } else if (baseColor == &darkPurple || baseColor == &darkGray) {
            return &pink;
        } else if (baseColor == &white) {
            return &black;
        } else {
            return &darkTurquoise;
        }
    case ColorModFlavor::NONE:
        return baseColor;
    }
    return baseColor;
}

std::vector<Ability *> Ability::loadModifierAbilities() {
    std::vector<Ability *> abilities = std::vector<Ability *>();
    Ability *ability;

    ability = new Ability();
    ability->namePrefix = "dire";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->minDamageBoost = 2;
    ability->maxDamageBoost = 6;
    ability->requiredFlags = GF_ANIMAL;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "knight";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->hpBoost = 12;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "bulwark";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->hpBoost = 20;
    ability->defense = DefenseType::LOW;
    ability->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "berserker";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 5;
    ability->minDamageBoost = 0;
    ability->maxDamageBoost = 6;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "scout";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 3;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "winged";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 3;
    ability->flags = (MONST_FLIES | MONST_FLITS);
    ability->moveSpeed = MoveSpeedType::FAST;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "swordsman";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 5;
    ability->defense = DefenseType::HIGH;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = (GF_ARMED);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "webweaver";
    ability->colorOverride = &white;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_SPIDERWEB};
    ability->requiredFlags = GF_INSECTOID;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY | MONST_IMMUNE_TO_WEBS);
    ability->rarityPercent = 75;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "mystic";
    ability->colorOverride = &goblinMysticColor;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_SHIELDING};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -2;
    ability->minDamageBoost = -2;
    ability->requiredFlags = (GF_PACK_MEMBER | GF_SHAMANISTIC | GF_SUPPORTS_CLASS);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "archer";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_DISTANCE_ATTACK};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -1;
    ability->minDamageBoost = -1;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "mounted";
    ability->colorOverride = &tanColor;
    ability->dangerBoost = 8;
    ability->bolts = {BOLT_DISTANCE_ATTACK};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -1;
    ability->minDamageBoost = -1;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "shaman";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_SPARK, BOLT_HASTE};
    ability->hpBoost = -6;
    ability->maxDamageBoost = -3;
    ability->minDamageBoost = -3;
    ability->requiredFlags = (GF_SHAMANISTIC | GF_SUPPORTS_CLASS);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "blademaster";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_BLINKING};
    ability->defense = DefenseType::HIGH;
    ability->requiredFlags = (GF_SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "blink";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 1;
    ability->bolts = {BOLT_BLINKING};
    ability->requiredFlags = (GF_ANIMAL);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "priest";
    ability->colorOverride = &darPriestessColor;
    ability->dangerBoost = 7;
    ability->bolts = {BOLT_SPARK, BOLT_HEALING, BOLT_NEGATION, BOLT_HASTE};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -5;
    ability->minDamageBoost = -5;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GF_PACK_MEMBER | GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "healer";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 6;
    ability->bolts = {BOLT_HEALING, BOLT_SHIELDING};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -6;
    ability->minDamageBoost = -6;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GF_PACK_MEMBER | GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "battlemage";
    ability->colorOverride = &darMageColor;
    ability->dangerBoost = 6;
    ability->bolts = {BOLT_FIRE, BOLT_DISCORD, BOLT_SLOW_2};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -5;
    ability->minDamageBoost = -5;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "fire mage";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -5;
    ability->minDamageBoost = -5;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "air mage";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_SPARK};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -5;
    ability->minDamageBoost = -5;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "firebreathing";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->light = LAVA_LIGHT;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE};
    ability->hpBoost = -2;
    ability->maxDamageBoost = -1;
    ability->minDamageBoost = -1;
    ability->requiredFlags = (GF_ANIMAL);
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "trickster";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->light = IMP_LIGHT;
    ability->dangerBoost = 8;
    ability->bolts = {BOLT_NEGATION, BOLT_SLOW_2, BOLT_DISCORD, BOLT_SPARK};
    ability->hpBoost = -2;
    ability->maxDamageBoost = -1;
    ability->minDamageBoost = -1;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "poison-spitting";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_POISON_DART};
    ability->defense = DefenseType::LOW;
    ability->attackSpeed = AttackSpeedType::TOTEM;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    ability->requiredFlags = (GF_INSECTOID);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "acid-spitting";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 10;
    ability->bolts = {BOLT_ACID_TURRET_ATTACK};
    ability->defense = DefenseType::LOW;
    ability->attackSpeed = AttackSpeedType::TURRET;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    ability->requiredFlags = (GF_INSECTOID);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "skirmisher";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 1;
    ability->flags = (MONST_FLEES_NEAR_DEATH);
    ability->regenSpeed = RegenSpeedType::FAST;
    ability->requiredFlags = (GF_SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "mirror";
    ability->colorOverride = &beckonColor;
    ability->dangerBoost = 3;
    ability->flags = MONST_REFLECT_4;
    ability->forbiddenFlags = (GF_SUPPORTS_CLASS | GF_ANIMAL);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "lava";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->light = SALAMANDER_LIGHT;
    ability->dangerBoost = 4;
    ability->flags = (MONST_IMMUNE_TO_FIRE | MONST_FIERY);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "cinder";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->dangerBoost = 1;
    ability->flags = (MONST_IMMUNE_TO_FIRE | MONST_SUBMERGES);
    ability->requiredFlags = (GF_ANIMAL);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "waterbreathing";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    ability->requiredFlags = (GF_ANIMAL | GF_AQUATIC);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "water";
    ability->colorOverride = &lightBlue;
    ability->dangerBoost = 2;
    ability->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    ability->requiredFlags = (GF_INSECTOID);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "spearman";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->abilFlags = (MA_ATTACKS_PENETRATE);
    ability->requiredFlags = (GF_ARMED);
    ability->forbiddenFlags = (GF_PACK_MEMBER);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "axeman";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->requiredFlags = (GF_ARMED);
    ability->abilFlags = (MA_ATTACKS_ALL_ADJACENT);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "dominatrix";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->requiredFlags = (GF_ARMED);
    ability->abilFlags = (MA_ATTACKS_EXTEND);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "whiptail";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->requiredFlags = (GF_INSECTOID);
    ability->abilFlags = (MA_ATTACKS_EXTEND);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "rainbow";
    ability->colorOverride = &rainbow;
    ability->light = UNICORN_LIGHT;
    ability->dangerBoost = 5;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_HIT_HALLUCINATE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "vampire";
    ability->colorOverride = &gray;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GF_ANIMAL);
    ability->abilFlags = (MA_TRANSFERENCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "grappler";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GF_SUPPORTS_CLASS);
    ability->forbiddenFlags = (GF_WIZARDLY);
    ability->abilFlags = (MA_SEIZES);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "sticky";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_SEIZES);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "venemous";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GF_AMORPHOUS | GF_ANIMAL);
    ability->abilFlags = (MA_POISONS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "deadly";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 5;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_CAUSES_WEAKNESS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "acid";
    ability->colorOverride = &acidBackColor;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_HIT_DEGRADE_ARMOR);
    ability->flags = (MONST_DEFEND_DEGRADE_WEAPON);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "caustic";
    ability->colorOverride = &poisonGasColor;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_BLOAT_DEATH;
    ability->featureMessage = "bursts, leaving behind an expanding cloud of caustic gas!";
    ability->rarityPercent = 66;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "pit";
    ability->colorOverride = &lightBlue;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_HOLE_POTION;
    ability->featureMessage = "bursts, causing the floor underneath $HIMHER to disappear!";
    ability->rarityPercent = 33;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "explosive";
    ability->colorOverride = &orange;
    ability->light = EXPLOSIVE_BLOAT_LIGHT;
    ability->dangerBoost = 17;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_BLOAT_EXPLOSION;
    ability->featureMessage = "detonates with terrifying force!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "infested";
    ability->colorOverride = &lichenColor;
    ability->dangerBoost = 15;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_MUTATION_LICHEN;
    ability->featureMessage = "bursts, filling the air with a cloud of fungal spores!";
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "fire";
    ability->colorOverride = &red;
    ability->light = LAVA_LIGHT;
    ability->dangerBoost = 8;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_INCINERATION_POTION;
    ability->featureMessage = "detonates into an immense conflagration!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "shattering";
    ability->light = SENTINEL_LIGHT;
    ability->colorOverride = &teal;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_SHATTERING_SPELL;
    ability->featureMessage = "bursts, releasing a wave of turquoise radiation! The walls begin to shimmer.";
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "stink";
    ability->colorOverride = &teal;
    ability->dangerBoost = 14;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_DEWAR_METHANE;
    ability->featureMessage = "bursts, and an offensive odor accompanies the hiss of escaping methane!";
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "gassy";
    ability->colorOverride = &confusionGasColor;
    ability->dangerBoost = 15;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_CONFUSION_GAS_CLOUD_POTION;
    ability->featureMessage = "bursts, and the air starts to shimmer and sparkle!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "thieving";
    ability->colorMod = ColorModFlavor::NONE;
    ability->dangerBoost = 3;
    ability->requiredFlags = (GF_THIEVING | GF_ANIMAL);
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "pickpocket";
    ability->colorOverride = &ogreColor;
    ability->dangerBoost = 3;
    ability->requiredFlags = (GF_THIEVING | GF_SUPPORTS_CLASS);
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "impish";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->light = IMP_LIGHT;
    ability->dangerBoost = 12;
    ability->defense = DefenseType::HIGH;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = (GF_THIEVING);
    ability->forbiddenFlags = (GF_AQUATIC | GF_AMORPHOUS | GF_INSECTOID);
    ability->bolts = {BOLT_BLINKING};
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    ability->rarityPercent = 66;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "rogue";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 11;
    ability->defense = DefenseType::HIGH;
    ability->requiredFlags = (GF_THIEVING);
    ability->bolts = {BOLT_SLOW_2, BOLT_HASTE};
    ability->requiredFlags = (GF_THIEVING | GF_SUPPORTS_CLASS);
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    ability->rarityPercent = 66;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "bandit";
    ability->colorOverride = &ogreColor;
    ability->dangerBoost = 8;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->hpBoost = -6;
    ability->maxDamageBoost = -3;
    ability->minDamageBoost = -3;
    ability->defense = DefenseType::LOW;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = (GF_THIEVING);
    ability->forbiddenFlags = (GF_AQUATIC | GF_AMORPHOUS | GF_INSECTOID);
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "river";
    ability->colorOverride = &brown;
    ability->dangerBoost = -3;
    ability->flags = (MONST_SUBMERGES | MONST_IMMUNE_TO_WATER);
    ability->requiredFlags = (GF_AQUATIC);
    ability->forbiddenFlags = (GF_AQUATIC_ONLY);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "abyssal";
    ability->colorOverride = &darkGray;
    ability->dangerBoost = 0;
    ability->regenSpeed = RegenSpeedType::VERY_FAST;
    ability->flags = (MONST_SUBMERGES | MONST_IMMUNE_TO_WATER | MONST_FLEES_NEAR_DEATH);
    ability->requiredFlags = (GF_AQUATIC);
    ability->forbiddenFlags = (GF_AQUATIC_ONLY);
    ability->rarityPercent = 50;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "electric";
    ability->colorOverride = &yellow;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_SPARK};
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "razortooth";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->minDamageBoost = 2;
    ability->maxDamageBoost = 4;
    ability->hpBoost = 5;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "stinging";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 3;
    ability->abilFlags = MA_POISONS;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "tentacle";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->abilFlags = MA_SEIZES;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "silverback";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 4;
    ability->flags = MONST_FLEES_NEAR_DEATH;
    ability->defense = DefenseType::HIGH;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "swarm";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 3;
    ability->flags = MONST_FLEES_NEAR_DEATH;
    ability->defense = DefenseType::HIGH;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_PACK_MEMBER | GF_ANIMAL);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "caller";
    ability->colorOverride = &yellow;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_HEALING, BOLT_DISCORD};
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_SUPPORTS_CLASS | GF_PACK_MEMBER);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "spearfisher";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 4;
    ability->abilFlags = MA_CAUSES_WEAKNESS;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ARMED);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "elder";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_DISCORD, BOLT_SLOW_2};
    ability->defense = DefenseType::LOW;
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_SUPPORTS_CLASS);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "totem";
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 1;
    ability->bolts = {BOLT_SPARK, BOLT_SHIELDING};
    ability->requiredFlags = (GF_TOTEM | GF_SHAMANISTIC);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "totem";
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 1;
    ability->bolts = {BOLT_HEALING, BOLT_SLOW_2};
    ability->requiredFlags = (GF_TOTEM | GF_SHAMANISTIC);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "statue";
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_BECKONING, BOLT_HASTE};
    ability->requiredFlags = (GF_TOTEM | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "idol";
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_CONJURATION, BOLT_INVISIBILITY};
    ability->requiredFlags = (GF_TOTEM | GF_WIZARDLY);
    ability->rarityPercent = 33;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "crystalline";
    ability->colorOverride = &darkTurquoise;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE, BOLT_SHIELDING};
    ability->requiredFlags = (GF_TOTEM | GF_WIZARDLY);
    ability->rarityPercent = 12;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "onyx";
    ability->colorOverride = &black;
    ability->light = DARKNESS_CLOUD_LIGHT;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE, BOLT_HASTE};
    ability->requiredFlags = (GF_TOTEM | GF_SHAMANISTIC);
    ability->rarityPercent = 12;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "arrow";
    ability->colorOverride = &black;
    ability->dangerBoost = 9;
    ability->bolts = {BOLT_DISTANCE_ATTACK};
    ability->minDamageBoost = 2;
    ability->maxDamageBoost = 6;
    ability->requiredFlags = (GF_TURRET);
    ability->forbiddenFlags = (GF_SHAMANISTIC | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "spark";
    ability->colorOverride = &lightningColor;
    ability->light = SPARK_TURRET_LIGHT;
    ability->dangerBoost = 14;
    ability->hpBoost = 50;
    ability->bolts = {BOLT_SPARK};
    ability->requiredFlags = (GF_TURRET);
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "electric";
    ability->colorOverride = &lightningColor;
    ability->dangerBoost = 11;
    ability->bolts = {BOLT_SPARK};
    ability->requiredFlags = (GF_TURRET);
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "acid";
    ability->colorOverride = &acidBackColor;
    ability->dangerBoost = 17;
    ability->minDamageBoost = 1;
    ability->maxDamageBoost = 2;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->bolts = {BOLT_ACID_TURRET_ATTACK};
    ability->requiredFlags = (GF_TURRET);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "flame";
    ability->colorOverride = &lavaForeColor;
    ability->light = LAVA_LIGHT;
    ability->dangerBoost = 19;
    ability->minDamageBoost = 1;
    ability->maxDamageBoost = 2;
    ability->bolts = {BOLT_FIRE};
    ability->requiredFlags = (GF_TURRET);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "dart";
    ability->colorOverride = &centipedeColor;
    ability->dangerBoost = 17;
    ability->minDamageBoost = 1;
    ability->maxDamageBoost = 2;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->bolts = {BOLT_POISON_DART};
    ability->requiredFlags = (GF_TURRET);
    ability->forbiddenFlags = (GF_SHAMANISTIC | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "oppulent";
    ability->colorOverride = &white;
    ability->dangerBoost = 10;
    ability->bolts = {BOLT_SLOW_2};
    ability->requiredFlags = (GF_TURRET | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "hypnotic";
    ability->colorOverride = &white;
    ability->light = SPECTRAL_BLADE_LIGHT;
    ability->dangerBoost = 18;
    ability->bolts = {BOLT_DISCORD};
    ability->requiredFlags = (GF_TURRET | GF_WIZARDLY);
    abilities.push_back(ability);

    return abilities;
}
