#ifndef HUNTER_H
#define HUNTER_H

#include "creature.h"

#define CREATURE_TYPE_HUNTER 1

class Hunter : public Creature
{
public:
    Hunter();

    virtual int getType() const;
    virtual QString getSpecies() const;
    virtual void advanceImpl();

protected:
    bool eatRabbit(Cell *cell);

    // used in finding new cell to jump to
    static bool hasRabbitsCondition(Cell *cell, int argument);

    void checkStarvation();
};

#endif // HUNTER_H
