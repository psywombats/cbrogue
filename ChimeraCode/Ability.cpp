/*
 * Ability.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#include "Ability.h"
#include "Body.h"

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
        summon(SummonType::NONE),
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
    monster.hp += hpBoost;
    monster.dangerLevel += dangerBoost;
    monster.damage.lowerBound += minDamageBoost;
    monster.damage.upperBound += maxDamageBoost;

    if (regenSpeed != RegenSpeedType::NORMAL) {
        monster.regenSpeed = regenSpeed;
    }
    if (moveSpeed != MoveSpeedType::NORMAL) {
        monster.moveSpeed = moveSpeed;
    }
    if (attackSpeed != AttackSpeedType::NORMAL) {
        monster.attackSpeed = attackSpeed;
    }
    if (accuracy != AccuracyType::NORMAL) {
        monster.accuracy = accuracy;
    }
    if (defense != DefenseType::NORMAL) {
        monster.defense = defense;
    }
    if (physique != PhysiqueType::NORMAL) {
        monster.physique = physique;
    }
    if (summon != SummonType::NONE) {
        monster.summon = summon;
    }
    
    int origHp = monster.hp;
    switch (physique) {
        case PhysiqueType::BUFF:
            monster.hp += 5;
            monster.hp += (float)monster.dangerLevel * 1.25f;
            
            monster.damage.lowerBound += (monster.dangerLevel / 5) + 1;
            monster.damage.upperBound += (monster.dangerLevel / 3) + 1;
            
            break;
        case PhysiqueType::SPELLCASTER:
            monster.hp = (float)monster.dangerLevel * 1.33;
            monster.hp += 8;
            monster.hp = MIN(monster.hp, origHp);
            
            monster.damage.lowerBound = MAX(1, monster.dangerLevel / 10);
            monster.damage.upperBound = MAX(1, monster.dangerLevel / 4);
            monster.hp += hpBoost;
            monster.damage.lowerBound += minDamageBoost;
            monster.damage.upperBound += maxDamageBoost;
            
            break;
        case PhysiqueType::NORMAL:
            break;
    }
    
    monster.damage.lowerBound = MAX(monster.damage.lowerBound, 0);
    
    monster.namePrefix = namePrefix;
    monster.nameSuffix = nameSuffix;

    for (boltType bolt : bolts) {
        monster.bolts.push_back(bolt);
    }

    monster.flags |= flags;
    monster.abilFlags |= abilFlags;
    
    if (featureKamikaze != DF_NONE) {
        monster.feature = featureKamikaze;
        monster.flags |= (MA_DF_ON_DEATH);
        monster.featureMessage = featureMessage;
    } else if (featurePeriodic != DF_NONE) {
        monster.feature = featurePeriodic;
        monster.featurePeriodicPercent = featurePeriodicPercent;
        monster.featureMessage = featureMessage;
    }
    
    if (colorOverride == NULL) {
        monster.displayColor = blendColor(monster.displayColor);
    } else {
        monster.displayColor = colorOverride;
    }
    
    if (light != NO_LIGHT) {
        monster.lightType = light;
    }

    inUse = true;
}

bool Ability::validForMonster(const ChimeraMonster &monster) const {
    return validForBody(monster.body);
}

bool Ability::validForBody(const Body &body) const {
    return validForBodyWithFlags(body, 0);
}

bool Ability::validForBodyWithFlags(const Body &body, unsigned long ignoredFlags) const {
    unsigned long effectiveRequiredFlags = (requiredFlags & (~ignoredFlags));
    if ((effectiveRequiredFlags & body.genFlags) != effectiveRequiredFlags) {
        return false;
    }
    if (body.genFlags & GF_NO_SPECIALS) {
        return false;
    }
    if ((forbiddenFlags & body.genFlags) > 0) {
        return false;
    }
    if ((featureKamikaze != DF_NONE || featurePeriodic != DF_NONE) && body.periodicFeature != DF_NONE) {
        return false;
    }
    if ((body.flags & flags) > 0) {
        // this means we'd generate something stupid like a "winged bat" or "mounted horseman"
        return false;
    }
    if (((body.flags & MONST_FLIES) && moveSpeed != MoveSpeedType::NORMAL) ||
        (body.moveSpeed != MoveSpeedType::NORMAL && (body.flags & MONST_FLIES))) {
        // flying is incompatible with movement modifiers
        return false;
    }
    if (body.defense == DefenseType::DEFENSELESS && defense != DefenseType::NORMAL) {
        return false;
    }
    if (dangerBoost + body.dangerLevel > 32) {
        // no one would see this monstrosity, it's probably like an explosive horror or something
        return false;
    }
    
    return true;
}

const color *Ability::blendColor(const color *baseColor) const {
    switch (colorMod) {
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
            return &darMageColor;
        } else if (baseColor == &brown) {
            return &teal;
        } else if (baseColor == &darkGreen) {
            return &pink;
        } else {
            return &goblinMysticColor;
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
    case ColorModFlavor::SUMMONING:
        if (baseColor == &brown || baseColor == &darkGreen) {
            return &goblinConjurerColor;
        } else if (baseColor == &white || baseColor == &gray || baseColor == &darkGray) {
            return &ectoplasmColor;
        } else {
            return &teal;
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
    ability->flavorAddition = "This individual ripples with muscles underneath $HISHER skin.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->physique = PhysiqueType::BUFF;
    ability->requiredFlags = GF_ANIMAL;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "knight";
    ability->flavorOverride = "This $BASE wears chainmail and holds aloft a $WEAPON, ready to accurately strike $HISHER foes.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->hpBoost = 12;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "bulwark";
    ability->flavorOverride = "A particularly durable $BASE, this stalwart heals extremely quickly.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->hpBoost = 20;
    ability->defense = DefenseType::LOW;
    ability->regenSpeed = RegenSpeedType::EXTREMELY_FAST;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "berserker";
    ability->flavorOverride = "This $BASE is covered in warpaint and charges down the dungeon halls at high speed.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 5;
    ability->minDamageBoost = 0;
    ability->maxDamageBoost = 6;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "scout";
    ability->flavorOverride = "Lightly armed and armored, this $BASE often travels ahead of the crowd to identify new threats.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 2;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "winged";
    ability->flavorAddition = "A pair of misshapen wings bear $HIMHER aloft over other $BASE.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 1;
    ability->flags = (MONST_FLIES | MONST_FLITS);
    ability->moveSpeed = MoveSpeedType::FAST;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "swordsman";
    ability->flavorOverride = "This $BASE wields a $SWORD and buckler and stands ready to engage in swordplay with the enemy.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 4;
    ability->defense = DefenseType::HIGH;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->requiredFlags = (GF_ARMED);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "webweaver";
    ability->flavorAddition = "Spinnerets stand at attention, ready to fire vollies of webs to trap $HISHER victims.";
    ability->colorOverride = &white;
    ability->dangerBoost = 2;
    ability->bolts = {BOLT_SPIDERWEB};
    ability->requiredFlags = GF_INSECTOID;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY | MONST_IMMUNE_TO_WEBS);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "mystic";
    ability->flavorOverride = "This $BASE is unarmed, but $HISHER fingers ripple with an innate magic that $HESHE uses to shield $HISHER allies.";
    ability->colorOverride = &goblinMysticColor;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_SHIELDING};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->requiredFlags = (GF_PACK_MEMBER | GF_SHAMANISTIC | GF_SUPPORTS_CLASS);
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "archer";
    ability->flavorOverride = "Wielding a sinew-stringed bow, this $BASE is capable of firing projectiles at range.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_DISTANCE_ATTACK};
    ability->maxDamageBoost = 2;
    ability->minDamageBoost = 0;
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->requiredFlags = GF_SUPPORTS_CLASS;
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "seer";
    ability->flavorOverride = "This $BASE is particularly wizened and leers from under robes of fur and feathers, gesturing with $HISHER $MAGIC.";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_SPARK, BOLT_HASTE};
    ability->requiredFlags = (GF_SHAMANISTIC | GF_SUPPORTS_CLASS | GF_PACK_MEMBER);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CAST_SPELLS_SLOWLY);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "blademaster";
    ability->flavorOverride = "With $SWORD in hand, this $BASE crosses great distances in an instant to cross swords with $HISHER enemies.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_BLINKING};
    ability->defense = DefenseType::HIGH;
    ability->requiredFlags = (GF_SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "blink";
    ability->flavorAddition = "Stripes line $HISHER ever-quivering body.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 1;
    ability->bolts = {BOLT_BLINKING};
    ability->requiredFlags = (GF_ANIMAL);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "priest";
    ability->flavorOverride = "Underneath the robe of this $BASE, the $BASE priest carries $HISHER holy $MAGIC and $MAGIC.";
    ability->colorOverride = &darPriestessColor;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_SPARK, BOLT_HEALING, BOLT_NEGATION, BOLT_HASTE};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->requiredFlags = (GF_PACK_MEMBER | GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "healer";
    ability->flavorOverride = "This $BASE wears a laurel wreath and carries a $MAGIC, brimming with arcane energy.";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_HEALING, BOLT_SHIELDING};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->requiredFlags = (GF_PACK_MEMBER | GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "battlemage";
    ability->flavorOverride = "The hands of this $BASE radiate a sickly light, indicating $HISHER eldritch power far above the average $BASE.";
    ability->colorOverride = &darMageColor;
    ability->dangerBoost = 6;
    ability->bolts = {BOLT_FIRE, BOLT_DISCORD, BOLT_SLOW_2};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "fire mage";
    ability->flavorOverride = "The many gowns and scarves worn by this $BASE would give $HIMHER a more dignified air if they weren't singed and smelling strongly of sulfur.";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "air mage";
    ability->flavorOverride = "Spark and streaks of white dance around this $BASE, belying $HISHER mastery over lightning.";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 2;
    ability->bolts = {BOLT_SPARK};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "firebreathing";
    ability->flavorAddition = "Brimstone falls from $HISHER maw and burns disfigure $HISHER breast.";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->light = LAVA_LIGHT;
    ability->dangerBoost = 6;
    ability->bolts = {BOLT_FIRE};
    ability->requiredFlags = (GF_ANIMAL);
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "trickster";
    ability->flavorOverride = "Ominously enough, the eyes of this $BASE glint with a craftiness and magical potency not usually seen among $HISHER kin.";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->light = IMP_LIGHT;
    ability->dangerBoost = 8;
    ability->bolts = {BOLT_NEGATION, BOLT_SLOW_2, BOLT_DISCORD, BOLT_SPARK};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->defense = DefenseType::HIGH;
    ability->maxDamageBoost = 2;
    ability->hpBoost = 5;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->flags = (MONST_MAINTAINS_DISTANCE | MONST_CARRY_ITEM_25);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "poison-spitting";
    ability->flavorAddition = "A poison gland in the back of $HISHER throat shoots globs of weakening venom at $HISHER prey.";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_POISON_DART};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->hpBoost = 10;
    ability->attackSpeed = AttackSpeedType::TOTEM;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    ability->requiredFlags = (GF_INSECTOID);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "acid-spitting";
    ability->flavorAddition = "This $BASE is highly venemous and can spit its armor-dissolving acid at foes.";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 10;
    ability->bolts = {BOLT_ACID_TURRET_ATTACK};
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->hpBoost = 10;
    ability->attackSpeed = AttackSpeedType::TURRET;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    ability->requiredFlags = (GF_INSECTOID);
    ability->rarityPercent = 100;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "skirmisher";
    ability->flavorOverride = "This $BASE towers above its peers and carries a crude $WEAPON and dances in and out of combat as the flow of fate decides.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 5;
    ability->physique = PhysiqueType::BUFF;
    ability->flags = (MONST_FLEES_NEAR_DEATH);
    ability->regenSpeed = RegenSpeedType::VERY_FAST;
    ability->requiredFlags = (GF_SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "mirror";
    ability->flavorOverride = "Instead of hide or hair, $HISHER body is covered in weird metal plates.";
    ability->colorOverride = &beckonColor;
    ability->dangerBoost = 2;
    ability->flags = MONST_REFLECT_4;
    ability->forbiddenFlags = (GF_SUPPORTS_CLASS | GF_ANIMAL);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "lava";
    ability->flavorAddition = "Molten skin mars $HISHER appearance and the ground smolders before this $BASE.";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->light = SALAMANDER_LIGHT;
    ability->dangerBoost = 1;
    ability->flags = (MONST_IMMUNE_TO_FIRE | MONST_FIERY);
    ability->forbiddenFlags = (GF_SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "cinder";
    ability->flavorAddition = "A blackened, stony countenance grants $HIMHER immunity to fire.";
    ability->colorMod = ColorModFlavor::FIRE;
    ability->dangerBoost = 1;
    ability->flags = (MONST_IMMUNE_TO_FIRE | MONST_SUBMERGES);
    ability->requiredFlags = (GF_ANIMAL);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "waterbreathing";
    ability->flavorAddition = "$HESHE is covered in gills that flap and gasp in the open air.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    ability->requiredFlags = (GF_ANIMAL | GF_AQUATIC);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "water";
    ability->flavorAddition = "$HESHE is able to skim the surface of the water on occassion dive below.";
    ability->colorOverride = &lightBlue;
    ability->dangerBoost = 0;
    ability->flags = (MONST_IMMUNE_TO_WATER | MONST_SUBMERGES);
    ability->requiredFlags = (GF_INSECTOID);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "spearman";
    ability->flavorOverride = "Carrying a pike almost twice $HISHER height, this $BASE is capable of piercing two enemies at once.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 0;
    ability->abilFlags = (MA_ATTACKS_PENETRATE);
    ability->requiredFlags = (GF_ARMED);
    ability->forbiddenFlags = (GF_PACK_MEMBER);
    ability->rarityPercent = 33;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "brute";
    ability->flavorOverride = "Looming over any fellow $BASE, this monster batters anything before $HIMHER with slow but earthshaking blows from $HISHER $WEAPON.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->physique = PhysiqueType::BUFF;
    ability->maxDamageBoost = 2;
    ability->attackSpeed = AttackSpeedType::SLOW;
    ability->requiredFlags = (GF_ARMED);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "axeman";
    ability->flavorOverride = "This $BASE growls as $HESHE hefts a stone waraxe, capable of hewing down many foes before $HIMHER.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 4;
    ability->physique = PhysiqueType::BUFF;
    ability->hpBoost = -5;
    ability->defense = DefenseType::LOW;
    ability->requiredFlags = (GF_ARMED);
    ability->abilFlags = (MA_ATTACKS_ALL_ADJACENT);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "dominatrix";
    ability->flavorOverride = "A curious $BASE, this individual wields a long-reaching whip, not to mention some unusual leather gear.";
    ability->colorOverride = &pink;
    ability->dangerBoost = 1;
    ability->requiredFlags = (GF_ARMED);
    ability->abilFlags = (MA_ATTACKS_EXTEND);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "whiptail";
    ability->flavorAddition = "$HISHER tail flashes through the air with a menacing crack.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 1;
    ability->requiredFlags = (GF_INSECTOID);
    ability->abilFlags = (MA_ATTACKS_EXTEND);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "rainbow";
    ability->flavorOverride = "The $BASE is covered with an alluring, pulsing membrane that glows many colors in the light. Surely touching $HIMHER would have pleasant effects...";
    ability->colorOverride = &rainbow;
    ability->light = UNICORN_LIGHT;
    ability->dangerBoost = 3;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_HIT_HALLUCINATE);
    ability->rarityPercent = 75;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "vampire";
    ability->flavorAddition = "$HISHER teeth appear sharper than usual, and $HISHER eyes gleam hungrily in the dark.";
    ability->colorOverride = &gray;
    ability->dangerBoost = 2;
    ability->hpBoost = 10;
    ability->abilFlags = (MA_TRANSFERENCE);
    ability->forbiddenFlags = (GF_SUPPORTS_CLASS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "grappler";
    ability->flavorOverride = "Despite not wielding a weapon, this $BASE is more than capable of grabbing and battering $HISHER victims.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GF_SUPPORTS_CLASS);
    ability->forbiddenFlags = (GF_WIZARDLY);
    ability->abilFlags = (MA_SEIZES);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "sticky";
    ability->flavorAddition = "$HESHE is covered in a mucus membrane that adheres to the floor behind $HIMHER.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 2;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_SEIZES);
    ability->rarityPercent = 33;
    abilities.push_back(ability);

    ability = new Ability();
    ability->nameSuffix = "venemous";
    ability->flavorAddition = "$HISHER violent coloring serves as a reminder of the poisonous toxin that $HESHE delivers with each attack.";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 4;
    ability->requiredFlags = (GF_AMORPHOUS | GF_ANIMAL);
    ability->abilFlags = (MA_POISONS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "deadly";
    ability->flavorAddition = "A single hit from this $BASE causes weakness, and a prolonged fight could well be fatal.";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 5;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_CAUSES_WEAKNESS);
    abilities.push_back(ability);

    ability = new Ability();
    ability->namePrefix = "acid";
    ability->flavorAddition = "$HESHE sizzles and hisses with a corrossive compound sure to degrade weapons and armor.";
    ability->colorOverride = &acidBackColor;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GF_AMORPHOUS);
    ability->abilFlags = (MA_HIT_DEGRADE_ARMOR);
    ability->flags = (MONST_DEFEND_DEGRADE_WEAPON);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "caustic";
    ability->flavorAddition = "Caustic gas trails behind $HIMHER, and at the slightest stress, promises to explode out of $HISHER body.";
    ability->colorOverride = &poisonGasColor;
    ability->dangerBoost = 6;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_BLOAT_DEATH;
    ability->featureMessage = "bursts, leaving behind an expanding cloud of caustic gas!";
    ability->rarityPercent = 66;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "pit";
    ability->flavorAddition = "When disturbed, the magical liquid $HESHE carries will open a chasm beneath $HIMHER.";
    ability->colorOverride = &lightBlue;
    ability->dangerBoost = 7;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_HOLE_POTION;
    ability->featureMessage = "bursts, causing the floor underneath $HIMHER to disappear!";
    ability->rarityPercent = 33;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "explosive";
    ability->flavorAddition = "The loud hissing emitting from $HIMHER warns of a massive explosion on $HISHER death.";
    ability->colorOverride = &orange;
    ability->light = EXPLOSIVE_BLOAT_LIGHT;
    ability->dangerBoost = 17;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_BLOAT_EXPLOSION;
    ability->featureMessage = "detonates with terrifying force!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "infested";
    ability->flavorAddition = "$HESHE appears decayed and rotted, the same deadly lichen ready to spread from $HISHER body on $HISHER death.";
    ability->colorOverride = &lichenColor;
    ability->dangerBoost = 15;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_MUTATION_LICHEN;
    ability->featureMessage = "bursts, filling the air with a cloud of fungal spores!";
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "fire";
    ability->flavorAddition = "As $HESHE approaches, the air grows hotter, threatening to engulf the whole area in flames on $HISHER death.";
    ability->colorOverride = &red;
    ability->light = LAVA_LIGHT;
    ability->dangerBoost = 8;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_INCINERATION_POTION;
    ability->featureMessage = "detonates into an immense conflagration!";
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "shattering";
    ability->flavorAddition = "The magical energy fuelling $HIMHER will erode the surrounding walls when $HESHE detonates.";
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
    ability->flavorAddition = "And for some reason, $HESHE smells absolutely terrible.";
    ability->colorOverride = &teal;
    ability->dangerBoost = 13;
    ability->requiredFlags = (GF_BURSTS);
    ability->featureKamikaze = DF_DEWAR_METHANE;
    ability->featureMessage = "bursts, and an offensive odor accompanies the hiss of escaping methane!";
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "gassy";
    ability->flavorAddition = "The sparkling gas $HESHE carries will rapidly expand on detonation, confusing any caught in the blast.";
    ability->colorOverride = &confusionGasColor;
    ability->dangerBoost = 14;
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
    ability->flavorOverride = "This fast-fingered $BASE appears more interested in looting than fighting.";
    ability->colorOverride = &ogreColor;
    ability->dangerBoost = 3;
    ability->requiredFlags = (GF_THIEVING | GF_SUPPORTS_CLASS);
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "impish";
    ability->flavorAddition = "Curious, kleptomaniac magics seem to move $HIMHER as if possessed.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->light = IMP_LIGHT;
    ability->dangerBoost = 12;
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->hpBoost = 10;
    ability->minDamageBoost = 2;
    ability->maxDamageBoost = 4;
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
    ability->flavorOverride = "Hiding under a hooded cloak and with a variety of magic at $HISHER disposal, this $BASE is a professional thief.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 11;
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->defense = DefenseType::HIGH;
    ability->hpBoost = 12;
    ability->minDamageBoost = 2;
    ability->maxDamageBoost = 5;
    ability->requiredFlags = (GF_THIEVING);
    ability->bolts = {BOLT_SLOW_2, BOLT_HASTE};
    ability->requiredFlags = (GF_THIEVING | GF_SUPPORTS_CLASS);
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    ability->flags = (MONST_CAST_SPELLS_SLOWLY);
    ability->rarityPercent = 66;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "bandit";
    ability->flavorAddition = "$HESHE is almost as good as running as $HESHE is at theft.";
    ability->colorOverride = &ogreColor;
    ability->dangerBoost = 8;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->requiredFlags = (GF_THIEVING);
    ability->forbiddenFlags = (GF_AQUATIC | GF_AMORPHOUS | GF_INSECTOID);
    ability->abilFlags = (MA_HIT_STEAL_FLEE);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "river";
    ability->flavorOverride = "While other $BASE roam the dungeon proper, this aquatic $BASE is more at home in the deep lakes that dot the depths.";
    ability->colorOverride = &brown;
    ability->dangerBoost = 0;
    ability->flags = (MONST_SUBMERGES | MONST_IMMUNE_TO_WATER);
    ability->requiredFlags = (GF_AQUATIC);
    ability->forbiddenFlags = (GF_AQUATIC_ONLY);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "abyssal";
    ability->flavorOverride = "Entirely devoid of eyes and relying on unknown senses to guide $HIMHER, this $BASE would look more at home in the botom of the ocean than wandering the dungeon halls.";
    ability->colorOverride = &darkGray;
    ability->dangerBoost = 2;
    ability->regenSpeed = RegenSpeedType::VERY_FAST;
    ability->flags = (MONST_SUBMERGES | MONST_IMMUNE_TO_WATER | MONST_FLEES_NEAR_DEATH);
    ability->requiredFlags = (GF_AQUATIC);
    ability->forbiddenFlags = (GF_AQUATIC_ONLY);
    ability->rarityPercent = 50;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "electric";
    ability->flavorAddition = "$HESHE possesses a specialized organ that allows $HIMHER to subdue $HISHER prey with electric shocks.";
    ability->colorOverride = &yellow;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_SPARK};
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "razortooth";
    ability->flavorAddition = "$HISHER knifelike teeth significantly overbite $HISHER jaw.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->physique = PhysiqueType::BUFF;
    ability->hpBoost = 5;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "stinging";
    ability->flavorAddition = "Foreign, aquatic toxins give its hit a poisonous punch.";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 3;
    ability->hpBoost = -5;
    ability->abilFlags = MA_POISONS;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "tentacle";
    ability->flavorAddition = "$HESHE can entangle prey with giant tentacles sprouting from its body.";
    ability->colorMod = ColorModFlavor::COMBAT;
    ability->dangerBoost = 3;
    ability->abilFlags = MA_SEIZES;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ANIMAL);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "silverback";
    ability->flavorAddition = "$HESHE possesses an exceptionally mobile body.";
    ability->colorMod = ColorModFlavor::MOBILITY;
    ability->dangerBoost = 4;
    ability->flags = MONST_FLEES_NEAR_DEATH;
    ability->defense = DefenseType::HIGH;
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "caller";
    ability->flavorOverride = "The sirenic calls of this shaman $BASE spread discord amongst $HISHER enemies and restore life to the denizens of the lake.";
    ability->colorOverride = &yellow;
    ability->dangerBoost = 0;
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->bolts = {BOLT_HEALING, BOLT_DISCORD};
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_SUPPORTS_CLASS | GF_PACK_MEMBER);
    ability->rarityPercent = 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "spearfisher";
    ability->flavorAddition = "The spear $HESHE carries is tipped with an ennervating toxin that softens the muscles.";
    ability->colorMod = ColorModFlavor::POISONOUS;
    ability->dangerBoost = 4;
    ability->abilFlags = (MA_CAUSES_WEAKNESS | MA_ATTACKS_PENETRATE);
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_ARMED);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "elder";
    ability->flavorOverride = "Oldest among the $BASE, this elder is well-versed in debilitating spells -- $HISHER eons beneath the surface have only sharpened $HISHER abilities.";
    ability->colorMod = ColorModFlavor::SPELLCASTING;
    ability->dangerBoost = 5;
    ability->hpBoost = -5;
    ability->bolts = {BOLT_DISCORD, BOLT_SLOW_2};
    ability->defense = DefenseType::LOW;
    ability->flags = (MONST_MAINTAINS_DISTANCE);
    ability->requiredFlags = (GF_AQUATIC_ONLY | GF_AQUATIC | GF_SUPPORTS_CLASS);
    abilities.push_back(ability);
    
    ability = new Ability();
//    ability->nameSuffix = "totem";
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 1;
    ability->bolts = {BOLT_SPARK, BOLT_SHIELDING};
    ability->requiredFlags = (GF_TOTEM | GF_SHAMANISTIC);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 1;
    ability->bolts = {BOLT_HEALING, BOLT_SLOW_2};
    ability->requiredFlags = (GF_TOTEM | GF_SHAMANISTIC);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_BECKONING, BOLT_HASTE};
    ability->requiredFlags = (GF_TOTEM | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 3;
    ability->bolts = {BOLT_CONJURATION, BOLT_INVISIBILITY};
    ability->requiredFlags = (GF_TOTEM | GF_WIZARDLY);
    ability->rarityPercent = 33;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE, BOLT_SHIELDING};
    ability->requiredFlags = (GF_TOTEM | GF_WIZARDLY);
    ability->rarityPercent = 12;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->colorMod = ColorModFlavor::TOTEM;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_FIRE, BOLT_HASTE};
    ability->requiredFlags = (GF_TOTEM | GF_SHAMANISTIC);
    ability->rarityPercent = 12;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "arrow";
    ability->flavorAddition = "$HESHE automatically acquires targets and fires arrows from a spring-loaded mechanism.";
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
    ability->flavorAddition = "When intruders approach, magical sigils spring to life and $HESHE fires arcs of lightning across the room.";
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
    ability->flavorAddition = "$HESHE is powered by electrical crystals capable of shocking intruders from across the dungeon.";
    ability->colorOverride = &lightningColor;
    ability->dangerBoost = 11;
    ability->bolts = {BOLT_SPARK};
    ability->requiredFlags = (GF_TURRET);
    ability->rarityPercent = 25;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "acid";
    ability->flavorAddition = "$HESHE conceals a store of acid that $HESHE liberally applies to anything nearby.";
    ability->colorOverride = &acidBackColor;
    ability->dangerBoost = 17;
    ability->minDamageBoost = 1;
    ability->maxDamageBoost = 2;
    ability->accuracy = AccuracyType::ACCURATE;
    ability->bolts = {BOLT_ACID_TURRET_ATTACK};
    ability->requiredFlags = (GF_TURRET);
    ability->forbiddenFlags = (GF_SHAMANISTIC | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "flame";
    ability->flavorAddition = "Great streams of fire emanate from $HESHE to set distant intruders alight.";
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
    ability->flavorAddition = "$HESHE is outfitted with a bolt of poisonous darts.";
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
    ability->flavorAddition = "Strange, white lights flow almost imperceptibly across $HISHER surface.";
    ability->colorOverride = &white;
    ability->dangerBoost = 10;
    ability->bolts = {BOLT_SLOW_2};
    ability->requiredFlags = (GF_TURRET | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "hypnotic";
    ability->flavorAddition = "Its lustrous surface gleams maddeningly in the dark.";
    ability->colorOverride = &discordColor;
    ability->light = SPECTRAL_BLADE_LIGHT;
    ability->dangerBoost = 18;
    ability->bolts = {BOLT_DISCORD};
    ability->requiredFlags = (GF_TURRET | GF_WIZARDLY);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "flameshroud";
    ability->flavorOverride = "Fire and smoke congeal to form the silhouette of a $BASE, concealed completely behind an aura of raging flames.";
    ability->colorOverride = &fireForeColor;
    ability->light = FLAMEDANCER_LIGHT;
    ability->dangerBoost = 5;
    ability->bolts = {BOLT_FIRE};
    ability->requiredFlags = (GF_BOSS_ONLY);
    ability->featurePeriodic = DF_FLAMEDANCER_CORONA;
    ability->featurePeriodicPercent= 100;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "dragon";
    ability->flavorOverride = "A fearsome hybrid between a $BASE and a dragon, this reptile breaths terrifying hellfire sure to incinerate all that stand before $HIMHER.";
    ability->colorOverride = &dragonColor;
    ability->light = FLAMEDANCER_LIGHT;
    ability->dangerBoost = 10;
    ability->bolts = {BOLT_DRAGONFIRE};
    ability->flags = MONST_IMMUNE_TO_FIRE;
    ability->requiredFlags = (GF_BOSS_ONLY | GF_ANIMAL);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "lightning";
    ability->flavorOverride = "Only the thin outline of a $BASE is visible in a rush of sparks and ozone.";
    ability->colorOverride = &lightningColor;
    ability->light = SENTINEL_LIGHT;
    ability->dangerBoost = 4;
    ability->bolts = {BOLT_SPARK, BOLT_BLINKING};
    ability->requiredFlags = (GF_BOSS_ONLY);
    ability->flags = MONST_FLEES_NEAR_DEATH;
    ability->regenSpeed = RegenSpeedType::VERY_FAST;
    ability->moveSpeed = MoveSpeedType::FAST;
    ability->defense = DefenseType::HIGH;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "crystalline";
    ability->flavorOverride = "This jumble of green and turquoise crystal superficially resembles a $BASE, emitting both a faint light and sure malevolence.";
    ability->colorOverride = &crystalWallLightColor;
    ability->light = CRYSTAL_WALL_LIGHT;
    ability->dangerBoost = 6;
    ability->physique = PhysiqueType::BUFF;
    ability->bolts = {BOLT_SPARK};
    ability->requiredFlags = (GF_BOSS_ONLY);
    ability->regenSpeed = RegenSpeedType::NONE;
    ability->defense = DefenseType::HIGH;
    ability->flags = MONST_REFLECT_4;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->namePrefix = "were";
    ability->flavorOverride = "This hunched and furred form could be a human wearing $BASE skin, or else a malformed $BASE learned to walk upright. $HESHE seems able to switch forms at will.";
    ability->colorMod = ColorModFlavor::SUMMONING;
    ability->dangerBoost = 1;
    ability->summon = SummonType::TRANSFORM_MOOK;
    ability->requiredFlags = (GF_ANIMAL);
    ability->forbiddenFlags = (GF_AQUATIC | GF_SUPPORTS_CLASS);
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "conjurer";
    ability->flavorOverride = "When this $BASE waves $HISHER $MAGIC, hidden energy manifests itself in front of $HIMHER.";
    ability->colorMod = ColorModFlavor::SUMMONING;
    ability->dangerBoost = 1;
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->summon = SummonType::CONJURATION;
    ability->flags = (MONST_CARRY_ITEM_25);
    ability->requiredFlags = (GF_SUPPORTS_CLASS);
    ability->rarityPercent = 75;
    abilities.push_back(ability);
    
    ability = new Ability();
    ability->nameSuffix = "summoner";
    ability->flavorOverride = "The $MAGIC in the hand of this $BASE binds $HIMHER to $HISHER allies, only a spell away.";
    ability->colorMod = ColorModFlavor::SUMMONING;
    ability->dangerBoost = 4;
    ability->physique = PhysiqueType::SPELLCASTER;
    ability->bolts = {BOLT_HASTE};
    ability->summon = SummonType::SPAWN_BASE_MOOK;
    ability->flags = (MONST_CAST_SPELLS_SLOWLY | MONST_MAINTAINS_DISTANCE);
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_WIZARDLY);
    ability->rarityPercent = 33;
    
    ability = new Ability();
    ability->nameSuffix = "shaman";
    ability->flavorOverride = "An old and ruined $BASE, that nevertheless inspires fear and awe in $HISHER myriad companions.";
    ability->colorMod = ColorModFlavor::SUMMONING;
    ability->dangerBoost = 6;
    ability->hpBoost = -5;
    ability->maxDamageBoost = -1;
    ability->bolts = {BOLT_SHIELDING, BOLT_SPARK};
    ability->flags = MONST_CAST_SPELLS_SLOWLY;
    ability->summon = SummonType::SPAWN_BASE_MOOK;
    ability->requiredFlags = (GF_SUPPORTS_CLASS | GF_SHAMANISTIC);
    ability->rarityPercent = 33;

    return abilities;
}
