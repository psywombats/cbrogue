/*
 * MonsterGenerator.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef MONSTERGENERATOR_H_
#define MONSTERGENERATOR_H_

class ChimeraMonster;
class Body;

class MonsterGenerator {

public:
	MonsterGenerator();
	virtual ~MonsterGenerator();

	std::list<ChimeraMonster *> &generateMonsters();

private:

	void loadBodies();

	std::list<ChimeraMonster *> monsters;
	std::list<Body *> bodies;

};

#endif /* MONSTERGENERATOR_H_ */
