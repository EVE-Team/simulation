#include "terraingenerator.h"
#include <QVector>

class Cell;

QVector<QVector<Cell::Terrain>> TerrainGenerator::generateTerrain(QSize qsize) {
    QVector<QVector<float>> floatTerrain = createFloatTerrain(qsize.height(), qsize.width());

    for (int step = 0; step < TerrainGenerator::GEN_STEP; ++step) {
        int genX1 = rand() % qsize.height();
        int genY1 = rand() % qsize.width();
        int genX2 = genX1 + TerrainGenerator::RECT_SIZE / 4 + rand() % TerrainGenerator::RECT_SIZE;
        int genY2 = genY1 + TerrainGenerator::RECT_SIZE / 4 + rand() % TerrainGenerator::RECT_SIZE;
        if (genX2 > qsize.height()) genX2 = qsize.height();
        if (genY2 > qsize.width()) genY2 = qsize.width();

        for (int i = genX1; i < genX2; ++i) {
            for (int j = genY1; j < genY2; ++j) {
                floatTerrain[i][j] += float(TerrainGenerator::Z_SCALE) / float(TerrainGenerator::GEN_STEP)
                        + rand() % 50 / float(50);
            }
        }
    }

    QVector<QVector<Cell::Terrain>> terrain(qsize.height());
    for (int i = 0; i < qsize.height(); i++) {
        terrain[i].resize(qsize.width());
    }

    for (int i = 0; i < qsize.height(); ++i) {
        for (int j = 0; j < qsize.width(); ++j) {
            if (floatTerrain[i][j] < 0) {
                terrain[i][j] = Cell::Water;
            } else if (floatTerrain[i][j] > 0 && floatTerrain[i][j] < 5) {
                terrain[i][j] = Cell::Grass;
            } else {
                terrain[i][j] = Cell::Mountain;
            }
        }
    }

    return terrain;
}

QVector<QVector<float>> createFloatTerrain(int x, int y){
    QVector<QVector<float>> terrain;
    for (int i = 0; i < x; ++i) {
        QVector<float> *terrainLine = new QVector<float>();
        for (int j = 0; j < y; ++j) {
            terrainLine->push_back(-2.0);
        }
        terrain.push_back(*terrainLine);
    }
    return terrain;
}
