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
};

#endif // HUNTER_H
