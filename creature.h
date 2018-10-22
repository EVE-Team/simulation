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
    // actual implementation of advance, after tickNumber check
    virtual void advanceImpl() = 0;

    // remove ourselves from parent cell
    void die();

    // move to a different cell
    // works by copying ourselves to newCell and removing ourselves from current cell
    void moveTo(Cell *newCell);

protected:
    Cell *parent;
    int id;
    int lastTick;
};

#endif // CREATURE_H
