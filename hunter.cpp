#include "hunter.h"
#include "cell.h"
#include "rabbit.h"
#include <QDebug>

Hunter::Hunter()
{

}

int Hunter::getType() const
{
    return CREATURE_TYPE_HUNTER;
}

QString Hunter::getSpecies() const
{
    return "Hunter";
}

void Hunter::advanceImpl()
{
    if (eatRabbit(getParent()))
    {
        // successfully eaten
        return;
    }

    // no rabbits in current cell
    // try to find something to eat in adjacent cells
    Cell *newCell = findAdjacentCellByCondition(hasRabbitsCondition);
    if (newCell)
    {
        moveTo(newCell);
        assert(eatRabbit(newCell));
    } else {
        // don't sit in one place if there's nothing to eat
        jumpToRandomAdjacentCell();
    }
}

// ignore "unused parameter ‘argument’" warning
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool Hunter::hasRabbitsCondition(Cell *cell, int argument)
{
    return (cell->getRabbitCount() > 0) && (cell->getCreatureCount(CREATURE_TYPE_HUNTER) < 3);
}

bool Hunter::eatRabbit(Cell *cell)
{
    if (cell->getRabbitCount() < 1)
    {
        return false;
    } else {
        Creature *rabbit = cell->getCreature(CREATURE_TYPE_RABBIT, 0);
        assert(rabbit != nullptr);
        cell->killCreature(rabbit);
        return true;
    }
}
