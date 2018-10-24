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

    // no rebbits in current cell
    Cell *newCell = findAdjacentCellByCondition(hasRabbitsCondition);
    if (newCell)
    {
        moveTo(newCell);
        assert(eatRabbit(newCell));
    } else {
        // no food
        die();
    }
}

bool Hunter::hasRabbitsCondition(Cell *cell)
{
    return cell->getRabbitCount() > 0;
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
