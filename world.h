#ifndef WORLD_H
#define WORLD_H

#include <QPainter>
#include <QSize>
#include "cell.h"

class World
{
public:
    World(QSize size);

    void render(QPainter &painter);

    Cell *cellAt(int x, int y);

    void resize(QSize size);
    QSize getSize() const;

    void advance();

protected:
    QVector<Cell> cells;
    QSize size;

    // called when the world is created or resized
    // resets all cells and generates new terrain
    void rebuildWorld();
};

#endif // WORLD_H
