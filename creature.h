#ifndef CREATURE_H
#define CREATURE_H

#include <QString>
class Cell;

class Creature
{
public:
    Creature();
    virtual ~Creature();

    // return unique id of the creature
    int getId() const;
    // creature type
    // creatures with lower priority will be processed first
    virtual int getType() const = 0;
    // get species name (ex. "Rabbit", "Wolf", ...)
    virtual QString getSpecies() const = 0;

    // set new parent cell
    void setParent(Cell *newParent);
    // get parent cell
    Cell *getParent() const;

    // move this creature one tick forward in time
    void advance(int tickNumber);

    // jump to a random adjacent cell that has grassy terrain and space for
    // creature of our type
    // returns true on success, false if remained in the same cell
    bool jumpToRandomAdjacentCell();

protected:
    Cell *parent;
    int id;
    int lastTick;

    // actual implementation of advance, after tickNumber check
    virtual void advanceImpl() = 0;

    // remove ourselves from parent cell
    // this frees memory allocated for this instance of creature
    void die();

    // move to a different cell
    // works by copying ourselves to newCell and removing ourselves from current cell
    void moveTo(Cell *newCell);

    // searches 8 adjacent cells in random order, passing them to callback
    // returns pointer to first cell for which callback returned true
    // returns nullptr if callback returned false for all cells
    // argument - optional argument that will be passed to callback function
    typedef bool (*CellConditionFunct)(Cell *cell, int argument);
    Cell *findAdjacentCellByCondition(CellConditionFunct callback, int argument = 0) const;

    // for jumpToRandomAdjacentCell()
    static bool randomCellConditionChecker(Cell *cell, int argument);
};

#endif // CREATURE_H
