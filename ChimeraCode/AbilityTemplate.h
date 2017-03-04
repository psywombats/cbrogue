/*
 * AbilityTemplate.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef ABILITYTEMPLATE_H_
#define ABILITYTEMPLATE_H_

#include "Ability.h"

class AbilityTemplate {

public:

	AbilityTemplate();
	virtual ~AbilityTemplate();

	virtual Ability *instantiate() = 0;
};

#endif /* ABILITYTEMPLATE_H_ */
