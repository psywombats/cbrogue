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
		dangerBoost(0),
		minDamageBoost(0),
		maxDamageBoost(0),
		regenSpeed(RegenSpeedType::NORMAL),
		moveSpeed(MoveSpeedType::NORMAL),
		attackSpeed(AttackSpeedType::NORMAL),
		accuracy(AccuracyType::NORMAL),
		defense(DefenseType::NORMAL),
		flits(false),
		flies(false),
		namePrefix(""),
		nameSuffix(""),
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

	if (this->flies) {
		monster.flies = true;
		monster.moveSpeed = MoveSpeedType::FAST;
	}
	monster.flits = this->flits;

	if (this->namePrefix.size() > 0) {
		monster.name = this->namePrefix + " " + monster.name;
	}
	if (this->nameSuffix.size() > 0) {
		monster.name += " " + this->nameSuffix;
	}

	monster.displayColor = this->blendColor(monster.displayColor);

	this->inUse = true;
}

bool Ability::validForMonster(const ChimeraMonster &monster) const {
	if ((this->requiredFlags & monster.flags) != this->requiredFlags) {
		return false;
	}
	if (monster.flies && this->flies) {
		return false;
	}
	if (monster.flits && this->flits) {
		return false;
	}
	if (monster.defense == DefenseType::DEFENSELESS && this->defense != DefenseType::NORMAL) {
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
	case ColorModFlavor::NONE:
		return baseColor;
	}
	return baseColor;
}

std::list<Ability *> Ability::loadModifierAbilities() {
	std::list<Ability *> abilities = std::list<Ability *>();
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
	ability->nameSuffix = "champion";
	ability->colorMod = ColorModFlavor::COMBAT;
	ability->dangerBoost = 3;
	ability->hpBoost = 8;
	ability->minDamageBoost = 3;
	ability->maxDamageBoost = 5;
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
	ability->requiredFlags = GenerateFlag::SUPPORTS_CLASS;
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
	ability->flies = true;
	ability->flits = true;
	abilities.push_back(ability);

	ability = new Ability();
	ability->nameSuffix = "swordsman";
	ability->colorMod = ColorModFlavor::MOBILITY;
	ability->dangerBoost = 3;
	ability->minDamageBoost = 1;
	ability->defense = DefenseType::HIGH;
	ability->accuracy = AccuracyType::ACCURATE;
	ability->requiredFlags = GenerateFlag::SUPPORTS_CLASS;
	abilities.push_back(ability);

	return abilities;
}
