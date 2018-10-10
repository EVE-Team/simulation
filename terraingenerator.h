#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <QVector>
#include <QSize>
#include "cell.h"

class TerrainGenerator
{
public:
    TerrainGenerator(QSize size);

    Cell::Terrain getTerrain(int x, int y) const;

protected:
    void generateTerrain();

    int width, height;
    QVector<QVector<Cell::Terrain>> terrain;
};

#endif // TERRAINGENERATOR_H
