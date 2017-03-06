/*
 * MonsterGenerator.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef MONSTERGENERATOR_H_
#define MONSTERGENERATOR_H_

#include <list>
#include <functional>

class ChimeraMonster;
class Body;
class Horde;

class MonsterGenerator {

public:
	MonsterGenerator();
	virtual ~MonsterGenerator();

	void generate();
	std::list<ChimeraMonster *> &getMonsters();
	std::list<Horde *> &getHordes();

private:

	std::list<ChimeraMonster *> monsters;
	std::list<Body *> bodies;
	std::list<Horde *> hordes;

	std::list<std::reference_wrapper<ChimeraMonster>> fodderMonsters;
	std::list<std::reference_wrapper<ChimeraMonster>> mookMonsters;

	Body *matchingBody(const std::function<bool(const Body *)>& filter);

	std::string debugReport() const;
};

#endif /* MONSTERGENERATOR_H_ */
