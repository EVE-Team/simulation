#include "hunter.h"
#include "cell.h"
#include "rabbit.h"
#include "wolf.h"
#include <QDebug>

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
    Cell *parent = getParent();
    int wolfCount = parent->getCreatureCount(CREATURE_TYPE_WOLF);
    int humanCount = parent->getCreatureCount(CREATURE_TYPE_HUNTER);

    if (wolfCount > 0)
    {
        // Если в клетке кролики, волки и люди, то разборка ведется только между людьми и волками, а
        // кролики спокойно кушают травку.

        if (humanCount > wolfCount) {
            // Если волков меньше чем людей, то люди снимают шкуру с одного волка. Остальные волки
            // прыгают на соседние клетки.
            Creature *wolf = parent->getCreature(CREATURE_TYPE_WOLF, 0);
            assert(wolf != nullptr);
            parent->killCreature(wolf);

            wolfCount = parent->getCreatureCount(CREATURE_TYPE_WOLF);
            for (int i = 0; i < wolfCount; i++)
            {
                Wolf* wolf = dynamic_cast<Wolf*>(parent->getCreature(CREATURE_TYPE_WOLF, i));
                wolf->jumpToRandomAdjacentCell();
                wolf->setLasTick(this->lastTick);
                wolf->increaseStarveLevel();
            }
        } else if (humanCount < wolfCount) {
            // Если в клетке волков больше чем людей, то волки обедают одним человеком. Остальные
            // люди прыгают на соседние клетки.
            Creature *human = parent->getCreature(CREATURE_TYPE_HUNTER, 0);
            assert(human != nullptr);
            parent->killCreature(human);

            humanCount = parent->getCreatureCount(CREATURE_TYPE_HUNTER);
            for (int i = 0; i < humanCount; i++)
            {
                Hunter* hunter = dynamic_cast<Hunter*>(parent->getCreature(CREATURE_TYPE_HUNTER, i));
                hunter->jumpToRandomAdjacentCell();
                hunter->setLasTick(this->lastTick);
                hunter->increaseStarveLevel();
            }
        } else if (humanCount == wolfCount) {
            // Если волков и людей в одной клетке одинаковое количество, то все спасаются бегством в
            // соседние клетки.
            for (int i = 0; i < wolfCount; i++)
            {
                Wolf* wolf = dynamic_cast<Wolf*>(parent->getCreature(CREATURE_TYPE_WOLF, i));
                wolf->jumpToRandomAdjacentCell();
                wolf->setLasTick(this->lastTick);
                wolf->increaseStarveLevel();
            }
            for (int i = 0; i < humanCount; i++)
            {
                Hunter* hunter = dynamic_cast<Hunter*>(parent->getCreature(CREATURE_TYPE_HUNTER, i));
                hunter->jumpToRandomAdjacentCell();
                hunter->setLasTick(this->lastTick);
                hunter->increaseStarveLevel();
            }

        }
    } else if (eatRabbit(getParent())) {
        // successfully eaten        
    } else {
        // no rabbits in current cell
        // try to find something to eat in adjacent cells
        Cell *newCell = findAdjacentCellByCondition(hasRabbitsCondition);
        if (newCell)
        {
            moveTo(newCell);
            assert(eatRabbit(newCell));
        } else {
            // don't sit in one place if there's nothing to eat
            jumpToRandomAdjacentCell();
        }
    }
    checkStarvation();
}

// ignore "unused parameter ‘argument’" warning
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool Hunter::hasRabbitsCondition(Cell *cell, int argument)
{
    return (cell->getRabbitCount() > 0) && (cell->getCreatureCount(CREATURE_TYPE_HUNTER) < 3);
}

bool Hunter::eatRabbit(Cell *cell)
{
    if (cell->getRabbitCount() < 1)
    {
        return false;
    } else {
        Creature *rabbit = cell->getCreature(CREATURE_TYPE_RABBIT, 0);
        assert(rabbit != nullptr);
        cell->killCreature(rabbit);
        resetStarveLevel();
        return true;
    }
}

void Hunter::checkStarvation()
{
    if (starveLevel == MAX_STARVE_LEVEL){
        this->die();
    }
}
