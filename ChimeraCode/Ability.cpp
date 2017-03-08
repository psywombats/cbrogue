/*
 * Ability.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Ability.h"

Ability::Ability() :
        hpBoost(0),
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

    this->inUse = true;
}

bool Ability::validForMonster(const ChimeraMonster &monster) const {
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
    ability->requiredFlags = GenerateFlag::ANIMAL;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "knight";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->hpBoost = 12;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = GenerateFlag::SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "bulwark";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->hpBoost = 20;
    ability->defense = DefenseType::LOW;
    ability->regenSpeed = RegenSpeedType::VERY_FAST;
    ability->requiredFlags = GenerateFlag::SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "berserker";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 5;
    ability->minDamageBoost = 0;
    ability->maxDamageBoost = 6;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::SHAMANISTIC);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "scout";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 3;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->requiredFlags = GenerateFlag::SUPPORTS_CLASS;
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
    ability->dangerBoost = 3;
    ability->minDamageBoost = 1;
    ability->defense = DefenseType::HIGH;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = (GenerateFlag::ARMED);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "webweaver";
    ability->colorOverride = &white;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_SPIDERWEB};
    ability->requiredFlags = GenerateFlag::INSECTOID;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY | MONST_IMMUNE_TO_WEBS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "mystic";
    ability->colorOverride = &goblinMysticColor;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_SHIELDING};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -2;
    ability->minDamageBoost = -2;
    ability->requiredFlags = (GenerateFlag::PACK_MEMBER | GenerateFlag::SHAMANISTIC | GenerateFlag::SUPPORTS_CLASS);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "archer";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_DISTANCE_ATTACK};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -1;
    ability->minDamageBoost = -1;
    ability->requiredFlags = GenerateFlag::SUPPORTS_CLASS;
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
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::SHAMANISTIC);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "shaman";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_SPARK, BOLT_HASTE};
    ability->hpBoost = -6;
    ability->maxDamageBoost = -3;
    ability->minDamageBoost = -3;
    ability->requiredFlags = (GenerateFlag::SHAMANISTIC | GenerateFlag::SUPPORTS_CLASS);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "blademaster";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_BLINKING};
    ability->defense = DefenseType::HIGH;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "blink";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 1;
    ability->bolts = {BOLT_BLINKING};
    ability->requiredFlags = (GenerateFlag::ANIMAL);
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
    ability->requiredFlags = (GenerateFlag::PACK_MEMBER | GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
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
    ability->requiredFlags = (GenerateFlag::PACK_MEMBER | GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
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
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
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
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "lightning mage";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_FIRE};
    ability->hpBoost = -4;
    ability->maxDamageBoost = -5;
    ability->minDamageBoost = -5;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "firebreathing";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE};
    ability->hpBoost = -2;
    ability->maxDamageBoost = -1;
    ability->minDamageBoost = -1;
    ability->requiredFlags = (GenerateFlag::ANIMAL);
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "trickster";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 8;
    ability->bolts = {BOLT_NEGATION, BOLT_SLOW_2, BOLT_DISCORD, BOLT_SPARK};
    ability->hpBoost = -2;
    ability->maxDamageBoost = -1;
    ability->minDamageBoost = -1;
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS | GenerateFlag::WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "poison-spitting";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_POISON_DART};
    ability->defense = DefenseType::LOW;
    ability->attackSpeed = AttackSpeedType::TOTEM;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    ability->requiredFlags = (GenerateFlag::INSECTOID);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "skirmisher";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 1;
    ability->flags = (MONST_FLEES_NEAR_DEATH);
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "pack";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->minDamageBoost = 1;
    ability->maxDamageBoost = 2;
    ability->flags = (MONST_FLEES_NEAR_DEATH);
    ability->requiredFlags = (GenerateFlag::ANIMAL | GenerateFlag::PACK_MEMBER);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "lava";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->dangerBoost = 4;
    ability->flags = (MONST_IMMUNE_TO_FIRE | MONST_FIERY);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "cinder";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->dangerBoost = 1;
    ability->flags = (MONST_IMMUNE_TO_FIRE | MONST_SUBMERGES);
    ability->requiredFlags = (GenerateFlag::ANIMAL);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "waterbreathing";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    ability->requiredFlags = (GenerateFlag::ANIMAL);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "water";
    ability->colorOverride = &lightBlue;
    ability->dangerBoost = 2;
    ability->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    ability->requiredFlags = (GenerateFlag::INSECTOID);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "spearman";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->requiredFlags = (GenerateFlag::ARMED);
    ability->abilFlags = (MA_ATTACKS_PENETRATE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "axeman";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->requiredFlags = (GenerateFlag::ARMED);
    ability->abilFlags = (MA_ATTACKS_ALL_ADJACENT);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "dominatrix";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->requiredFlags = (GenerateFlag::ARMED);
    ability->abilFlags = (MA_ATTACKS_EXTEND);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "whiptail";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->requiredFlags = (GenerateFlag::INSECTOID);
    ability->abilFlags = (MA_ATTACKS_EXTEND);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "rainbow";
    ability->colorOverride = &rainbow;
    ability->dangerBoost = 5;
    ability->requiredFlags = (GenerateFlag::AMORPHOUS);
    ability->abilFlags = (MA_HIT_HALLUCINATE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "vampire";
    ability->colorOverride = &gray;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GenerateFlag::ANIMAL);
    ability->abilFlags = (MA_TRANSFERENCE);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "grappler";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GenerateFlag::SUPPORTS_CLASS);
    ability->forbiddenFlags = (GenerateFlag::WIZARDLY);
    ability->abilFlags = (MA_SEIZES);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "sticky";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->requiredFlags = (GenerateFlag::AMORPHOUS);
    ability->abilFlags = (MA_SEIZES);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "venemous";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GenerateFlag::AMORPHOUS | GenerateFlag::ANIMAL);
    ability->abilFlags = (MA_POISONS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "deadly";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 5;
    ability->requiredFlags = (GenerateFlag::AMORPHOUS);
    ability->abilFlags = (MA_CAUSES_WEAKNESS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "acid";
    ability->colorOverride = &acidBackColor;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GenerateFlag::AMORPHOUS);
    ability->abilFlags = (MA_HIT_DEGRADE_ARMOR);
    ability->flags = (MONST_DEFEND_DEGRADE_WEAPON);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "caustic";
    ability->colorOverride = &poisonGasColor;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_BLOAT_DEATH;
    ability->featureMessage = "bursts, leaving behind an expanding cloud of caustic gas!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "pit";
    ability->colorOverride = &lightBlue;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_HOLE_POTION;
    ability->featureMessage = "bursts, causing the floor underneath $HIMHER to disappear!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "explosive";
    ability->colorOverride = &orange;
    ability->dangerBoost = 17;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_BLOAT_EXPLOSION;
    ability->featureMessage = "detonates with terrifying force!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "infested";
    ability->colorOverride = &lichenColor;
    ability->dangerBoost = 15;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_MUTATION_LICHEN;
    ability->featureMessage = "bursts, filling the air with a cloud of fungal spores!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "fire";
    ability->colorOverride = &red;
    ability->dangerBoost = 8;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_INCINERATION_POTION;
    ability->featureMessage = "detonates into an immense conflagration!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "shattering";
    ability->colorOverride = &teal;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_SHATTERING_SPELL;
    ability->featureMessage = "bursts, releasing a wave of turquoise radiation! The walls begin to shimmer.";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "stink";
    ability->colorOverride = &teal;
    ability->dangerBoost = 14;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_DEWAR_METHANE;
    ability->featureMessage = "bursts, and an offensive odor accompanies the hiss of escaping methane!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "gassy";
    ability->colorOverride = &confusionGasColor;
    ability->dangerBoost = 15;
    ability->requiredFlags = (GenerateFlag::BURSTS);
    ability->featureKamikaze = DF_CONFUSION_GAS_CLOUD_POTION;
    ability->featureMessage = "bursts, and the air starts to shimmer and sparkle!";
    abilities.push_back(ability);

    return abilities;
}
