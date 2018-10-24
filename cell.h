#ifndef CELL_H
#define CELL_H

#include <QPainter>
#include <QVector>
#include "creature.h"

class World;

// assume there are 2 creature types for now
#define NO_OF_CREATURE_TYPES 2

extern const int maxRabbitCount, minGrass;

// ignore initialization reorder warnings
#pragma GCC diagnostic ignored "-Wreorder"

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

    // lowers grass level by 1
    // returns true if grass level has been decreased
    // returns false if grass level is already at 0
    bool decreaseGrass();

    // returns world this cell is located in
    World *getParent() const;

    // returns cell with coordinates (dx, dy) relative to current cell
    // so getAdjacentCell(0, 0) will return current cell,
    // getAdjacentCell(0, -1) will return cell above this one
    // returns nullptr if cell doesn't exist
    Cell *getAdjacentCell(int dx, int dy) const;

    // move this cell one tick forward in time
    // (set random weather, process grass, process rabbits)
    void advance(int tickNumber);

    // adds new creature to this cell
    // returns true on success
    // returns false if there's no room for more creatures of this type
    bool addCreature(Creature *newCreature);
    //
    Creature *getCreature(int type, int index) const;

    // number of creatures living in this cell (0-3)
    int getCreatureCount(int creatureType) const;
    //
    int getRabbitCount() const;

    // returns true on success
    // returns false if creature couldn't be found in this cell
    // --
    // removes creature from list af creatures assigned to this cell
    // sets creature's parent to nullptr
    // Creature instance IS NOT freed, and can be assigned to a new cell afterwards
    bool disownCreature(Creature *creature);
    // same as disownCreature but Creature instance
    // IS DEALLOCATED, and will no longer be valid
    bool killCreature(Creature *creature);

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
    QVector<Creature*> creatures[NO_OF_CREATURE_TYPES];

    static void drawCreature(QPainter &painter, int count, int verticalPosition, const QImage &icon);
};

#endif // CELL_H
