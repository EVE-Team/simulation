#include "rabbit.h"
#include "cell.h"
#include <QtDebug>
#include "world.h"
#include <QRandomGenerator>

Rabbit::Rabbit()
{
}

int Rabbit::getType() const
{
    return CREATURE_TYPE_RABBIT;
}

QString Rabbit::getSpecies() const
{
    return "Rabbit";
}

void Rabbit::advanceImpl()
{
    if (eatGrass())
    {
        // if there are 2 rabbits in a cell, they reproduce and third is added
        if (getParent()->getRabbitCount() == 2)
        {
            getParent()->addCreature(new Rabbit());
        }
    } else {
        die();
    }
}

// ignore "unused parameter ‘argument’" warning
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool Rabbit::newCellConditionChecker(Cell *cell, int argument)
{
    return (cell->getRabbitCount() < maxRabbitCount) && (cell->getGrassLevel() > minGrass);
}

bool Rabbit::eatGrass()
{
    if (getParent()->decreaseGrass())
    {
        // there's food in current cell
        return true;
    }

    // not enough food in current cell
    // try to move to a different cell
    Cell *newCell = findAdjacentCellByCondition(&newCellConditionChecker);
    if (newCell)
    {
        // found a cell with food and free space
        moveTo(newCell);
        // we still need to eat, so we eat from our new cell
        assert(newCell->decreaseGrass());
        return true;
    } else {
        // couldn't find a cell to move to; starved to death
        return false;
    }
}
