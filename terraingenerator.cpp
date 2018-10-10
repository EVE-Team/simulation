#include "terraingenerator.h"
#include <QRandomGenerator>

TerrainGenerator::TerrainGenerator(QSize size)
    : width(size.width()), height(size.height()),
      terrain(height)
{
    for (int i = 0; i < height; i++)
    {
        terrain[i].resize(width);
    }

    generateTerrain();
}

Cell::Terrain TerrainGenerator::getTerrain(int x, int y) const
{
    return terrain[x][y];
}

void TerrainGenerator::generateTerrain()
{
    QRandomGenerator *random = QRandomGenerator::global();

    // TODO: add better terrain generation
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            terrain[x][y] = static_cast<Cell::Terrain>(random->bounded(0, 3));
        }
    }
}
