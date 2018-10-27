#include "wolf.h"
#include "cell.h"
#include "hunter.h"
#include "rabbit.h"

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

// standard library? anyone?
static int min(int a, int b)
{
    return a < b ? a : b;
}

static int max(int a, int b)
{
    return a > b ? a : b;
}

void Wolf::advanceImpl()
{
    Cell *parent = getParent();
    int wolfCount = parent->getCreatureCount(CREATURE_TYPE_WOLF);
    int humanCount = parent->getCreatureCount(CREATURE_TYPE_HUNTER);
    const int rabbitCount = parent->getCreatureCount(CREATURE_TYPE_RABBIT);

    if (humanCount > 0)
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
                parent->getCreature(CREATURE_TYPE_WOLF, i)->jumpToRandomAdjacentCell();
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
                parent->getCreature(CREATURE_TYPE_HUNTER, i)->jumpToRandomAdjacentCell();
            }
        } else if (humanCount == wolfCount) {
            // Если волков и людей в одной клетке одинаковое количество, то все спасаются бегством в
            // соседние клетки.
            for (int i = 0; i < wolfCount; i++)
            {
                parent->getCreature(CREATURE_TYPE_WOLF, i)->jumpToRandomAdjacentCell();
            }
            for (int i = 0; i < humanCount; i++)
            {
                parent->getCreature(CREATURE_TYPE_HUNTER, i)->jumpToRandomAdjacentCell();
            }

        }
    } else if (rabbitCount > 0) {
        // Если волки и кролики встречаются в одной клетке, то волки обедают (за один такт волк
        // съедает не более двух кроликов, остальные кролики прыгают на соседние клетки).

        const int rabbitsToBeEaten = min(wolfCount * 2, rabbitCount);
        const int rabbitsRemaining = max(rabbitCount - rabbitsToBeEaten, 0);

        for (int i = 0; i < rabbitsToBeEaten; i++)
        {
            Creature *rabbit = parent->getCreature(CREATURE_TYPE_RABBIT, 0);
            assert(rabbit != nullptr);
            parent->killCreature(rabbit);
        }

        for (int i = 0; i < rabbitsRemaining; i++)
        {
            parent->getCreature(CREATURE_TYPE_RABBIT, i)->jumpToRandomAdjacentCell();
        }
    }
}
