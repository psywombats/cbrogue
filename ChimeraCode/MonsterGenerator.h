/*
 * MonsterGenerator.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef MONSTERGENERATOR_H_
#define MONSTERGENERATOR_H_

#include <list>

class ChimeraMonster;
class Body;

class MonsterGenerator {

public:
	MonsterGenerator();
	virtual ~MonsterGenerator();

	void generate();
	std::list<ChimeraMonster *> &getMonsters();

private:

	std::list<ChimeraMonster *> monsters;
	std::list<Body *> bodies;

};

#endif /* MONSTERGENERATOR_H_ */
