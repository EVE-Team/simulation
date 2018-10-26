#include "world.h"
#include "terraingenerator.h"
#include <QVector>

World::World(QSize size)
    : cells(size.width() * size.height(), Cell(this)),
      size(size), tickNumber(0)
{
    rebuildWorld();
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
        return &cells[y * size.width() + x];
    } else {
        return nullptr;
    }
}

void World::resize(QSize newSize)
{
    int newElemCount = newSize.width() * newSize.height();
    int numOfNewElements = newElemCount - cells.size();

    size = newSize;
    if (numOfNewElements < 0)
    {
        cells.resize(newElemCount);
    } else if (numOfNewElements > 0) {
        // avoid adding new elements with default constructor
        cells.reserve(newElemCount);
        for (int i = 0 ; i < numOfNewElements; i++)
        {
            cells.append(Cell(this));
        }
    }

    assert(newElemCount == cells.size());

    rebuildWorld();
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
            cellAt(x, y)->advance(tickNumber);
        }
    }

    tickNumber++;
}

void World::rebuildWorld()
{
    QVector<QVector<Cell::Terrain>> tgen = TerrainGenerator::generateTerrain(size);

    // reset all cells
    for (int x = 0; x < size.width(); x++)
    {
        for (int y = 0; y < size.height(); y++)
        {
            Cell *cell = cellAt(x, y);
            *cell = Cell(this);
            cell->setPosition(x, y);
            cell->setTerrain(tgen[y][x]);
        }
    }
}

int World::getCreaturePopulation(int creatureId)
{
    int creaturePopulation = 0;

    const int w = getSize().width(), h = getSize().height();
    for (int x = 0; x < w; x++)
    {
        for (int y = 0 ; y < h; y++)
        {
            creaturePopulation += cellAt(x, y)->getCreatureCount(creatureId);
        }
    }

    return creaturePopulation;
}
