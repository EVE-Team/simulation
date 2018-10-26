#include "wolf.h"

Wolf::Wolf()
{

}

int Wolf::getType() const
{
    return CREATURE_TYPE_WOLF;
}

QString Wolf::getSpecies() const
{
    return "Wolf";
}

void Wolf::advanceImpl()
{
}
