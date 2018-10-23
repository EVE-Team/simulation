#include "hunter.h"

Hunter::Hunter()
{

}

int Hunter::getType() const
{
    return CREATURE_TYPE_HUNTER;
}

QString Hunter::getSpecies() const
{
    return "Hunter";
}

void Hunter::advanceImpl()
{
}
