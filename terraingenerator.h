#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <QVector>
#include <QSize>
#include "cell.h"

class TerrainGenerator
{
public:
    static QVector<QVector<Cell::Terrain>> generateTerrain(QSize size);
    struct Koefficient {
        int area;
        int genStep;
        int rectSize;
    };
private:
    static const int GEN_STEP = 40; //кол-во шагов генерации
    static const int RECT_SIZE = 3; //размеры генерируемого квадрата
    static const int Z_SCALE = 35; //коэффициент растяжения карты в высоту

    static const int GEN_STEP_MAX = 16000;
    static const int RECT_SIZE_MAX = 4;

    static const int GEN_STEP_MIN = 40;
    static const int RECT_SIZE_MIN = 3;

    static const int MIN_AREA = 9;
    static const int MAX_AREA = 10000;

    static int getGenStep(int area);
    static int getRectSize(int area);
};

QVector<QVector<float>> createFloatTerrain(int x, int y);
#endif // TERRAINGENERATOR_H
