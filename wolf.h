#ifndef WOLF_H
#define WOLF_H

#include "creature.h"

#define CREATURE_TYPE_WOLF 2

class Wolf : public Creature
{
public:
    Wolf();

    virtual int getType() const;
    virtual QString getSpecies() const;
    virtual void advanceImpl();

    void checkStarvation();
};

#endif // WOLF_H
