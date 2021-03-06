#include "battle.h"
#include <cassert>

NumberOfShips Battle::countRebelFleet() const {
    NumberOfShips numberOfShips = 0;
    
    for (const auto &ship: rebelShips) {
        numberOfShips += ship->howManyUndestroyedUnits();
    }
    
    return numberOfShips;
}

NumberOfShips Battle::countImperialFleet() const {
    NumberOfShips numberOfShips = 0;
    
    for (const auto &ship: imperialShips) {
        numberOfShips += ship->howManyUndestroyedUnits();
    }
    
    return numberOfShips;
}

void Battle::tick(Time timeStep) {
    NumberOfShips rebelFleet = countRebelFleet(), imperialFleet = countImperialFleet();
    
    if (rebelFleet == 0 && imperialFleet == 0) {
        std::cout << "DRAW\n";
    }
    else if (imperialFleet == 0) {
        std::cout << "REBELLION WON\n";
    }
    else if (rebelFleet == 0) {
        std::cout << "IMPERIUM WON\n";
    }
    else {
        if (spaceTime->isItAttackTime()) {
            fight();
        }
    }
    
    spaceTime->tick(timeStep);
}

const std::shared_ptr<SpaceTime> &Battle::getSpaceTime() const {
    return spaceTime;
}

Battle::Battle(const std::vector<std::shared_ptr<ImperialStarship>> &imperialShips,
               const std::vector<std::shared_ptr<RebelStarship>> &rebelShips,
               Time t0, Time t1) :
        imperialShips(imperialShips),
        rebelShips(rebelShips),
        spaceTime(std::make_shared<DefaultSpaceTime>(t0, t1)) {}

Battle::Battle(const std::vector<std::shared_ptr<ImperialStarship>> &imperialShips,
               const std::vector<std::shared_ptr<RebelStarship>> &rebelShips,
               const std::shared_ptr<SpaceTime> sTime) :
        imperialShips(imperialShips),
        rebelShips(rebelShips),
        spaceTime(sTime) {}

void Battle::fight() {
    for (auto &imperialShip: imperialShips) {
        for (auto &rebelShip: rebelShips) {
            if (imperialShip->getShield() > 0 && rebelShip->getShield() > 0) {
                imperialShip->maybeAttack(rebelShip);
                rebelShip->maybeAttack(imperialShip);
            }
        }
    }
}

SpaceBattle::Builder &SpaceBattle::Builder::startTime(Time t) {
    t0 = t;
    return *this;
}

SpaceBattle::Builder &SpaceBattle::Builder::maxTime(Time t) {
    t1 = t;
    return *this;
}

SpaceBattle::Builder &SpaceBattle::Builder::spaceTimeKind(std::shared_ptr<SpaceTime> sTime) {
    spaceTime = sTime;
    return *this;
}

SpaceBattle::Builder &SpaceBattle::Builder::ship(std::shared_ptr<ImperialStarship> imperialShip) {
    imperialShips.push_back(imperialShip);
    return *this;
}

SpaceBattle::Builder &SpaceBattle::Builder::ship(std::shared_ptr<RebelStarship> rebelShip) {
    rebelShips.push_back(rebelShip);
    return *this;
}

Battle SpaceBattle::Builder::build() {
    if (spaceTime == nullptr) {
        assert(t0 <= t1);
        return Battle(imperialShips, rebelShips, std::make_shared<DefaultSpaceTime>(t0, t1));
    }
    else {
        return Battle(imperialShips, rebelShips, spaceTime);
    }
}