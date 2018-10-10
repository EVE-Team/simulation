#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <QVector>
#include <QSize>
#include "cell.h"

class TerrainGenerator
{
public:
//    TerrainGenerator(QSize size);
    static QVector<QVector<Cell::Terrain>> generateTerrain(QSize size);
    Cell::Terrain getTerrain(int x, int y) const;

protected:
    int width, height;
    QVector<QVector<Cell::Terrain>> terrain;
private:
    static const int GEN_STEP = 512;
    static const int RECT_SIZE = 4;
    static const int Z_SCALE = 40;

};

QVector<QVector<float>> createFloatTerrain(int x, int y);
#endif // TERRAINGENERATOR_H
