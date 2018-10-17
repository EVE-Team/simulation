#ifndef CELL_H
#define CELL_H

#include <QPainter>
#include "rabbit.h"

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
    QPoint getPosition() const;

    Terrain getTerrain() const;
    void setTerrain(Terrain terrain);

    void setWeather(int sun, int rain);
    void setRandomWeather();
    int getSunLevel() const;
    int getRainLevel() const;
    int getGrassLevel() const;

    // modify grass level (+1, -1) according to weather in current cell
    void processGrass();

    // returns world this cell is located in
    World *getParent() const;

    // move this cell one tick forward in time
    // (set random weather, process grass, process rabbits)
    void advance(int tickNumber);

    // lowers grass level by 1
    // returns true if grass level has been decreased
    // returns false if grass level is already at 0
    bool decreaseGrass();

    // returns true if this cell has grass and space for another rabbit
    // always returns false for cells with water and mountain landscape
    bool rabbitCanMoveHere() const;

    // adds newRabbit to this cell
    // returns true on success
    // returns false if there's no room for more rabbits
    bool addRabbit(const Rabbit &newRabbit);

    // number of rabbits living in this cell (0-3)
    int getRabbitCount() const;

    // returns cell with coordinates (dx, dy) relative to current cell
    // so getAdjacentCell(0, 0) will return current cell,
    // getAdjacentCell(0, -1) will return cell above this one
    // returns nullptr if cell doesn't exist
    Cell *getAdjacentCell(int dx, int dy) const;

    // removes the rabbit by id from rabbits array
    // true if rabbit was removed
    // false otherwise (if there were no rabbit with such id)
    bool removeRabbit(int id);

protected:
    // landscape (grass, water or mountain)
    Terrain terrain;
    // sun (0-3), rain (0-3) and grass (0-5) levels
    int sun, rain, grass;

    // world class on which this cell is located
    World *parent;
    // position of this cell in the world (x, y)
    int posX, posY;

    // returns true if one of 8 adjacent cells has Water terrain
    bool isNearWater() const;

    // rabbits living on this cell (0-3)
    QVector<Rabbit> rabbits;
};

#endif // CELL_H
