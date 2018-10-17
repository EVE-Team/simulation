#include "rabbit.h"
#include "cell.h"
#include <QtDebug>
#include "world.h"
#include <QRandomGenerator>

Rabbit::Rabbit()
    : parent(nullptr),
      lastTick(-1)
{
    static int globalId = 0;
    id = globalId++;

    //qDebug() << "rabbit No" << id << "created";
}

void Rabbit::setParent(Cell *newParent)
{
    parent = newParent;
}

Cell *Rabbit::getParent() const
{
    return parent;
}

void Rabbit::advance(int tickNumber)
{
    // preven eating twice in one tick if rabbit moved to not yet processed cell
    if (lastTick >= tickNumber)
    {
        //qDebug() << "rabbit" << getId() << "skipped a tick";
        return;
    }
    lastTick = tickNumber;

    if (!getParent()->decreaseGrass())
    {
        // not enough food
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

        for (int x = x_start; x_end == 1 ? x <= x_end : x >= x_end; x+=x_dir)
        {
            for (int y = y_start; y_end == 1 ? y <= y_end : y >= y_end; y+=y_dir)
            {
                if (!(x == 0 && y == 0))
                {
                    Cell *newCell = getParent()->getAdjacentCell(x, y);
                    if (newCell != nullptr && newCell->rabbitCanMoveHere())
                    {
                        moveTo(newCell);
                        //qDebug() << "rabbit" << getId() << "jumped to cell" << getParent()->getPosition();
                        // we still need to eat, so we eat from our new cell
                        assert(newCell->decreaseGrass());
                        return;
                    }
                }
            }
        }

        // couldn't find a cell to move to
        //qDebug() << "rabbit" << getId() << "died from hunger";
        die();
        // prevent dead rabbits from reproducing
        // TODO: separate eating into its own method (returns bool)
        // TODO: currently the rabbit won't attempt to reproduce if it jumped to another cell
        return;
    }

    // if there are 2 rabbits in a cell, they reproduce and third is added
    if (getParent()->getRabbitCount() == 2)
    {
        //qDebug() << "rabbits in cell" << getParent()->getPosition() << "reproduced";
        getParent()->addRabbit(Rabbit());
    }
}

int Rabbit::getId() const
{
    return id;
}

void Rabbit::die()
{
    //qDebug() << "rabbit No" << id << "removed from cell" << getParent()->getPosition();
    assert(getParent()->removeRabbit(getId()));
}

void Rabbit::moveTo(Cell *newCell)
{
    if (getParent() == newCell)
    {
        return;
    }

    //qDebug() << "rabbit" << getId() << "moving from" << getParent()->getPosition() << "to" << newCell->getPosition();

    // copy ourselves to new cell
    assert(newCell->addRabbit(*this));
    // delete ourselves from current cell
    die();
}
