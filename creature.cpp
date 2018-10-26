#include <QRandomGenerator>
#include "creature.h"
#include "cell.h"
#include <QDebug>

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

Cell *Creature::findAdjacentCellByCondition(CellConditionFunct callback, int argument) const
{
    // randomize search direction
    int x_start = -1, x_end = 1, x_dir = 1;
    int y_start = -1, y_end = 1, y_dir = 1;
    QRandomGenerator *random = QRandomGenerator::global();
    if (random->bounded(0, 2))
    {
        x_start = -x_start; x_end = -x_end; x_dir = -x_dir;
    }
    if (random->bounded(0, 2))
    {
        y_start = -y_start; y_end = -y_end; y_dir = -y_dir;
    }

    // search adjacent cells
    for (int x = x_start; x_end == 1 ? x <= x_end : x >= x_end; x+=x_dir)
    {
        for (int y = y_start; y_end == 1 ? y <= y_end : y >= y_end; y+=y_dir)
        {
            if (!(x == 0 && y == 0))
            {
                Cell *newCell = getParent()->getAdjacentCell(x, y);
                if (newCell != nullptr && callback(newCell, argument))
                {
                    return newCell;
                }
            }
        }
    }

    return nullptr;
}

bool Creature::randomCellConditionChecker(Cell *cell, int argument)
{
    return (cell->getTerrain() == Cell::Terrain::Grass) && (cell->getCreatureCount(argument) < 3);
}

bool Creature::jumpToRandomAdjacentCell()
{
    // findAdjacentCellByCondition doesn't return strictly random cell, it prefers cells at corners
    // but close enough
    Cell *newCell = findAdjacentCellByCondition(randomCellConditionChecker, getType());
    if (newCell)
    {
        moveTo(newCell);
        return true;
    } else {
        return false;
    }
}
