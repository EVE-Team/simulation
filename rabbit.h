#pragma once

#include "creature.h"

#define CREATURE_TYPE_RABBIT 0

class Rabbit : public Creature
{
public:
    Rabbit();

    virtual int getType() const;
    virtual QString getSpecies() const;
    virtual void advanceImpl();

protected:
    // attempts to eat one unit of grass
    // if there's no grass in current cell, will jump to adjacent
    // cell that has any and eat from new cell
    // returns true if rabbit has successfully eaten
    // returns false if rabbit strved to death (and we should call die())
    bool eatGrass();
};
