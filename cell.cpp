#include <QRandomGenerator>
#include "cell.h"
#include "resourcemanager.h"
#include "world.h"

const int cellSize = 100;

Cell::Cell(World *parent)
    : terrain(Terrain::Grass),
      parent(parent),
      sun(0), rain(0), grass(0)
{
}

void Cell::renderAt(QPainter &painter, QPoint pos) const
{
    painter.save();
    painter.translate(pos * cellSize);

    QImage texure;
    switch (terrain)
    {
    case Terrain::Grass:
        texure = ResourceManager::instance()->grassTexture();
        break;
    case Terrain::Mountain:
        texure = ResourceManager::instance()->rockTexture();
        break;
    case Terrain::Water:
        texure = ResourceManager::instance()->waterTexture();
        break;
    }
    painter.drawImage(QRect(0, 0, cellSize, cellSize), texure);

    painter.setPen(Qt::white);
    painter.drawText(QRect(0, 0, cellSize, cellSize), Qt::AlignHCenter,
                     QString("S:%1 R:%2 G:%3\n%4 %5").arg(sun).arg(rain).arg(grass).arg(posX).arg(posY));

    painter.restore();
}

Cell::Terrain Cell::getTerrain() const
{
    return terrain;
}

void Cell::setTerrain(Terrain newTerrain)
{
    terrain = newTerrain;
}

void Cell::setWeather(int sun, int rain)
{
    assert(sun >= 0 && sun <= 3);
    assert(rain >= 0 && rain <= 3);

    // При ливне солнца быть не может
    if (rain == 3)
        sun = 0;

    // при жгучем солнце нет дождя
    if (sun == 3)
        rain = 0;

    this->sun = sun; this->rain = rain;
}

void Cell::setRandomWeather()
{
    QRandomGenerator *gen = QRandomGenerator::global();
    setWeather(gen->bounded(0, 4), gen->bounded(0, 4));
}

void Cell::processGrass()
{
    // трава растет только если ландшафт - поле
    if (terrain != Terrain::Grass)
    {
        grass = 0;
        return;
    }

    if (isNearWater() && sun > 0)
    {
        // Вокруг озера или реки трава растет всегда, когда есть солнце, даже если дождь не идет.
        grass++;
    }
    else if ((rain == 0 && sun == 3) || (rain == 3 && sun == 0))
    {
        // 3 (Ливень)    0 (Нет солнца)    Трава умирает
        // 0 (Нет дождя) 3 (Жгучее солнце) Высыхает трава
        grass--;
    }
    else if ((sun == 1 && (rain == 1 || rain == 2)) ||
             (sun == 2 && rain != 0) ||
             (sun == 3 && rain >= 2))
    {
        // 1 (Слабый дождик) 1 (Слабое солнце)  Растет трава
        // 2 (Средний дождь) 1 (Слабое солнце)  Растет трава
        // 1 (Слабый дождик) 2 (Сильное солнце) Растет трава
        // 2 (Средний дождь) 2 (Сильное солнце) Растет трава
        // 3 (Ливень)        2 (Сильное солнце) Растет трава
        // 2 (Средний дождь) 3 (Жгучее солнце)  Растет трава
        // 3 (Ливень)        3 (Жгучее солнце)  Растет трава
        grass++;
    }

    if (grass < 0) grass = 0;
    if (grass > 5) grass = 5;
}

void Cell::advance()
{
    setRandomWeather();
    processGrass();
}

void Cell::setPosition(int x, int y)
{
    posX = x; posY = y;
}

bool Cell::isNearWater() const
{
    for (int x = posX - 1; x <= posX + 1; x++)
    {
        for (int y = posY - 1; y <= posY + 1; y++)
        {
            if (x >= 0 && y >= 0 && x < parent->getSize().width() && y < parent->getSize().height())
            {
                if (parent->cellAt(x, y)->getTerrain() == Terrain::Water)
                {
                    return true;
                }
            }
        }
    }

    return false;
}
