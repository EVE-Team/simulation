#include "terraingenerator.h"
#include <QVector>

class Cell;

static QVector<TerrainGenerator::Koefficient> test() {
    QVector<TerrainGenerator::Koefficient> koefs;
    koefs.append(TerrainGenerator::Koefficient{9,40,3});
    koefs.append(TerrainGenerator::Koefficient{25,125,3});
    koefs.append(TerrainGenerator::Koefficient{100,100,4});
    koefs.append(TerrainGenerator::Koefficient{400,500,4});
    koefs.append(TerrainGenerator::Koefficient{1600,2500,4});
    koefs.append(TerrainGenerator::Koefficient{3600,6000,4});
    koefs.append(TerrainGenerator::Koefficient{6400,10000,4});
    koefs.append(TerrainGenerator::Koefficient{10000,16000,4});
    return koefs;
}

int TerrainGenerator::getGenStep(int area) {
    if (area == TerrainGenerator::MAX_AREA) {
        return TerrainGenerator::GEN_STEP_MAX;
    } else if (area == TerrainGenerator::MIN_AREA) {
        return TerrainGenerator::GEN_STEP_MIN;
    } else {
        int genStep = TerrainGenerator::GEN_STEP_MIN;
        QVector<TerrainGenerator::Koefficient> koefs = test();
        for (int i = koefs.size() - 1; i >= 0; --i) {
            if (area > koefs[i].area) {
               TerrainGenerator::Koefficient koef = koefs[i];
               double k = (double)area / (double)koef.area;
               genStep = k * (double)koef.genStep;
               break;
            }
        }
        return genStep;
    }
}

int TerrainGenerator::getRectSize(int area) {
    if (area == TerrainGenerator::MAX_AREA) {
        return TerrainGenerator::RECT_SIZE_MAX;
    } else if (area == TerrainGenerator::MIN_AREA) {
        return TerrainGenerator::RECT_SIZE_MIN;
    } else {
        int rectSize = TerrainGenerator::RECT_SIZE_MIN;
        QVector<TerrainGenerator::Koefficient> koefs = test();
        for (int i = koefs.size() - 1; i >= 0; --i) {
            if (area > koefs[i].area) {
               rectSize = koefs[i].rectSize;
               break;
            }
        }
        return rectSize;
    }
}

QVector<QVector<Cell::Terrain>> TerrainGenerator::generateTerrain(QSize qsize) {
    QVector<QVector<float>> floatTerrain = createFloatTerrain(qsize.height(), qsize.width());

    int genStep = getGenStep(qsize.height() * qsize.width());
    int rectSize = getRectSize(qsize.height() * qsize.width());

    for (int step = 0; step < genStep; ++step) {
        int genX1 = rand() % qsize.height();
        int genY1 = rand() % qsize.width();
        int genX2 = genX1 + rectSize / 4 + rand() % rectSize;
        int genY2 = genY1 + rectSize / 4 + rand() % rectSize;
        if (genX2 > qsize.height()) genX2 = qsize.height();
        if (genY2 > qsize.width()) genY2 = qsize.width();

        for (int i = genX1; i < genX2; ++i) {
            for (int j = genY1; j < genY2; ++j) {
                floatTerrain[i][j] += float(TerrainGenerator::Z_SCALE) / float(genStep)
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
