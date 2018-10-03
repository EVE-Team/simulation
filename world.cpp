#include "world.h"
#include <QVector>

World::World(QSize size)
    : cells(size.width() * size.height(), Cell(this)),
      size(size)
{
    setCellsPositions();
}

void World::render(QPainter &painter)
{
    for (int x = 0; x < size.width(); x++)
    {
        for (int y = 0; y < size.height(); y++)
        {
            cellAt(x, y)->renderAt(painter, QPoint(x, y));
        }
    }
}

Cell *World::cellAt(int x, int y)
{
    if (x >= 0 && y >= 0 && x < size.width() && y < size.height())
    {
        return &cells[x * size.width() + y];
    } else {
        return nullptr;
    }
}

void World::resize(QSize newSize)
{
    size = newSize;
    cells.resize(size.width() * size.height());
    setCellsPositions();
}

QSize World::getSize() const
{
    return size;
}

void World::advance()
{
    for (int x = 0; x < size.width(); x++)
    {
        for (int y = 0; y < size.height(); y++)
        {
            cellAt(x, y)->advance();
        }
    }
}

void World::setCellsPositions()
{
    for (int x = 0; x < size.width(); x++)
    {
        for (int y = 0; y < size.height(); y++)
        {
            cellAt(x, y)->setPosition(x, y);
        }
    }
}
