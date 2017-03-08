/*
 * MonsterGenerator.h
 *
 *  Created on: Mar 4, 2017
 *      Author: aking_2
 */

#ifndef MONSTERGENERATOR_H_
#define MONSTERGENERATOR_H_

#include <vector>
#include <functional>

class ChimeraMonster;
class Body;
class Horde;
class Ability;

class MonsterGenerator {

public:
    MonsterGenerator();
    virtual ~MonsterGenerator();

    void generate();
    std::vector<ChimeraMonster *> &getMonsters();
    std::vector<Horde *> &getHordes();

private:

    std::vector<ChimeraMonster *> monsters;
    std::vector<Body *> bodies;
    std::vector<Ability *> abilities;
    std::vector<Horde *> hordes;

    ChimeraMonster &newMonster(Body &body);
    ChimeraMonster &newMonster(const ChimeraMonster &baseMonster);
    Horde &newHorde(const ChimeraMonster &monster);

    Body *matchingBody(const std::function<bool(const Body *)>& filter);
    Ability *matchingAbility(const std::function<bool(const Ability *)>& filter);

    std::string debugReport() const;
};

#endif /* MONSTERGENERATOR_H_ */
