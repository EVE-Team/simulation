#include "creature.h"
#include "cell.h"

Creature::Creature()
    : parent(nullptr),
      lastTick(-1)
{
    static int globalId = 0;
    id = globalId++;
}

Creature::~Creature()
{
    // empty
}

int Creature::getId() const
{
    return id;
}

void Creature::setParent(Cell *newParent)
{
    parent = newParent;
}

Cell *Creature::getParent() const
{
    return parent;
}

void Creature::advance(int tickNumber)
{
    // prevent advancing twice in one tick if creature moved to not yet processed cell
    if (lastTick >= tickNumber)
    {
        return;
    }
    lastTick = tickNumber;

    advanceImpl();
}

void Creature::die()
{
    assert(getParent()->killCreature(this));
}

void Creature::moveTo(Cell *newCell)
{
    if (getParent() == newCell)
    {
        return;
    }

    // deattach ourselves from current cell
    assert(getParent()->disownCreature(this));
    // assign ourselves to new cell
    assert(newCell->addCreature(this));
}
