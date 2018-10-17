#include <QRandomGenerator>
#include "cell.h"
#include "resourcemanager.h"
#include "world.h"
#include <QDebug>

// cell size in pixels when rendered
const int cellSize = 100;

// minimum level of weather properties (sun and rain level)
const int minWeather = 0, maxWeather = 3;
const int minGrass = 0, maxGrass = 5;
const int minRabbitCount = 0, maxRabbitCount = 3;

Cell::Cell(World *parent)
    : terrain(Terrain::Grass),
      parent(parent),
      sun(minWeather), rain(minWeather), grass(minGrass)
{
    rabbits.reserve(maxRabbitCount);
}

void Cell::renderAt(QPainter &painter, QPoint pos) const
{
    painter.save();
    painter.translate(pos * cellSize);

    QImage texure;
    switch (terrain)
    {
    case Terrain::Grass:
        texure = ResourceManager::instance()->grassTexture(grass);
        break;
    case Terrain::Mountain:
        texure = ResourceManager::instance()->rockTexture();
        break;
    case Terrain::Water:
        texure = ResourceManager::instance()->waterTexture();
        break;
    }
    painter.drawImage(QRect(0, 0, cellSize, cellSize), texure);

    painter.drawImage(QPoint(11, 1), ResourceManager::instance()->sunIcon());
    painter.drawImage(QPoint(36, 1), ResourceManager::instance()->rainIcon());
    painter.drawImage(QPoint(63, 1), ResourceManager::instance()->grassIcon());

    const int rc = getRabbitCount();
    if (rc >= 1)
    {
        painter.drawImage(QPoint(2, 60), ResourceManager::instance()->rabbitIcon());
    }
    if (rc >= 2) {
        painter.drawImage(QPoint(34, 60), ResourceManager::instance()->rabbitIcon());
    }
    if (rc >= 3) {
        painter.drawImage(QPoint(66, 60), ResourceManager::instance()->rabbitIcon());
    }

    painter.setPen(Qt::white);
    painter.drawText(QPoint(26, 13), QString::number(sun));
    painter.drawText(QPoint(52, 13), QString::number(rain));
    painter.drawText(QPoint(79, 13), QString::number(grass));

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
    assert(sun >= minWeather && sun <= maxWeather);
    assert(rain >= minWeather && rain <= maxWeather);

    // При ливне солнца быть не может
    if (rain == maxWeather)
    {
        assert(sun == minWeather);
    }

    // при жгучем солнце нет дождя
    if (sun == maxWeather)
    {
        assert(rain == minWeather);
    }

    this->sun = sun; this->rain = rain;
}

void Cell::setRandomWeather()
{
    const int numOfWeatherCombinations = 11;
    const static int weatherValues[numOfWeatherCombinations][2] = {
            { 0, 0 },
            { 0, 1 },
            { 0, 2 },
            { 0, 3 },
            { 1, 0 },
            { 1, 1 },
            { 1, 2 },
            { 2, 0 },
            { 2, 1 },
            { 2, 2 },
            { 3, 0 }
        };

    QRandomGenerator *random = QRandomGenerator::global();
    int weatherIndex = random->bounded(0, numOfWeatherCombinations);
    setWeather(weatherValues[weatherIndex][0], weatherValues[weatherIndex][1]);
}

void Cell::processGrass()
{
    // трава растет только если ландшафт - поле
    if (terrain != Terrain::Grass)
    {
        grass = minGrass;
        return;
    }

    if (isNearWater() && sun > minWeather)
    {
        // Вокруг озера или реки трава растет всегда, когда есть солнце, даже если дождь не идет.
        grass++;
    }
    else if ((rain == minWeather && sun == maxWeather) || (rain == maxWeather && sun == minWeather))
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

    if (grass < minGrass) grass = minGrass;
    if (grass > maxGrass) grass = maxGrass;
}

void Cell::advance(int tickNumber)
{
    setRandomWeather();
    processGrass();

    for (int i = 0; i < getRabbitCount(); i++)
    {
        rabbits[i].advance(tickNumber);
    }
}

void Cell::setPosition(int x, int y)
{
    posX = x; posY = y;
}

bool Cell::isNearWater() const
{
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            Cell *adjCell = getAdjacentCell(x, y);
            if (adjCell != nullptr && adjCell->getTerrain() == Terrain::Water)
            {
                return true;
            }
        }
    }

    return false;
}

QPoint Cell::getPosition() const
{
    return QPoint(posX, posY);
}

int Cell::getSunLevel() const
{
    return sun;
}

int Cell::getRainLevel() const
{
    return rain;
}

int Cell::getGrassLevel() const
{
    return grass;
}

Cell *Cell::getAdjacentCell(int dx, int dy) const
{
    auto pos = getPosition();
    return getParent()->cellAt(pos.x() + dx, pos.y() + dy);
}

World *Cell::getParent() const
{
    return parent;
}

bool Cell::decreaseGrass()
{
    if (grass > minGrass)
    {
        grass--;
        return true;
    } else {
        return false;
    }
}

bool Cell::rabbitCanMoveHere() const
{
    return (getRabbitCount() < maxRabbitCount) && (getGrassLevel() > minGrass);
}

bool Cell::addRabbit(const Rabbit &newRabbit)
{
    if (getRabbitCount() < maxRabbitCount)
    {
        rabbits.append(newRabbit);
        //qDebug() << "setting parent for rabbit" << rabbits[rabbits.size() - 1].getId();
        rabbits[rabbits.size() - 1].setParent(this);
        //qDebug() << "added rabbit" << newRabbit.getId() << "to cell" << getPosition();
        return true;
    } else {
        return false;
    }
}

int Cell::getRabbitCount() const
{
    return rabbits.size();
}

bool Cell::removeRabbit(int id)
{
    for (int i = 0; i < getRabbitCount(); i++)
    {
        if (rabbits[i].getId() == id)
        {
            rabbits.remove(i);
            //qDebug() << "rabbit with id" << id << "removed from cell" << getPosition();
            return true;
        }
    }

    qWarning() << "failed to remove - no rabbit with id" << id;
    return false;
}
