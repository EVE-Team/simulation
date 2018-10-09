#ifndef CELL_H
#define CELL_H

#include <QPainter>

class World;

class Cell
{
public:
    // QVector requires us to have empty constructor, even though it's never called
    Cell() { abort(); }
    Cell(World *parent);

    enum Terrain
    {
        Grass = 0,
        Water = 1,
        Mountain = 2
    };

    void renderAt(QPainter &painter, QPoint pos) const;

    void setPosition(int x, int y);

    Terrain getTerrain() const;
    void setTerrain(Terrain terrain);

    void setWeather(int sun, int rain);
    void setRandomWeather();

    void processGrass();

    void advance();

protected:
    Terrain terrain;
    int sun, rain, grass;

    World *parent;
    int posX, posY;

    bool isNearWater() const;
};

#endif // CELL_H
