#include "resourcemanager.h"

ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
    : texWater(":/images/water.png"),
      texRock(":/images/rock.png"),
      iconSun(":/images/sun.png"),
      iconRain(":/images/rain.png"),
      iconGrass(":/images/grass_icon.png")
{
    for (int i = 0; i <= 5; i++)
    {
        texGrass[i] = QImage(QString(":/images/grass%1.png").arg(i));
    }
}

ResourceManager *ResourceManager::instance()
{
    if (!m_instance)
        m_instance = new ResourceManager;
    return m_instance;
}

const QImage &ResourceManager::grassTexture(int level) const
{
    assert(level >= 0 && level <= 5);
    return texGrass[level];
}

const QImage &ResourceManager::waterTexture() const
{
    return texWater;
}

const QImage &ResourceManager::rockTexture() const
{
    return texRock;
}

const QImage &ResourceManager::sunIcon() const
{
    return iconSun;
}

const QImage &ResourceManager::rainIcon() const
{
    return iconRain;
}

const QImage &ResourceManager::grassIcon() const
{
    return iconGrass;
}
