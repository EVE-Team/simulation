#ifndef CELL_H
#define CELL_H

#include <QPainter>

class Cell
{
public:
    Cell();

    enum Terrain
    {
        Grass = 0,
        Water = 1,
        Mountain = 2
    };

    void renderAt(QPainter &painter, QPoint pos) const;

    Terrain getTerrain() const;
    void setTerrain(Terrain terrain);

    void setWeather(int sun, int rain);
    void setRandomWeather();

    void processGrass();

    void advance();

protected:
    Terrain terrain;
    int sun, rain, grass;
};

#endif // CELL_H
