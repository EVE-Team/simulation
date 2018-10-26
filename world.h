#ifndef WORLD_H
#define WORLD_H

#include <QPainter>
#include <QSize>
#include "cell.h"
#include "rabbit.h"

class World
{
public:
    World(QSize size);

    void render(QPainter &painter);

    // returns cell with coordinates (x, y)
    // nullptr if no such cell exists
    Cell *cellAt(int x, int y);

    void resize(QSize size);
    QSize getSize() const;

    void advance();

    // return total number of creatures with id creatureId existing in this world
    int getCreaturePopulation(int creatureId);

protected:
    QVector<Cell> cells;
    QSize size;

    // called when the world is created or resized
    // resets all cells and generates new terrain
    void rebuildWorld();

    int tickNumber;
};

#endif // WORLD_H
