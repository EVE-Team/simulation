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

bool Rabbit::eatGrass()
{
    if (getParent()->decreaseGrass())
    {
        // there's food in current cell
        return true;
    }

    // not enough food in current cell
    // try to move to a different cell

    // randomize direction in which rabbit jumps
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
                if (newCell != nullptr && newCell->rabbitCanMoveHere())
                {
                    // found a cell with food and free space
                    moveTo(newCell);;
                    // we still need to eat, so we eat from our new cell
                    assert(newCell->decreaseGrass());
                    return true;
                }
            }
        }
    }

    // couldn't find a cell to move to; starved to death
    return false;
}
