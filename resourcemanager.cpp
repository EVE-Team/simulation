#include "resourcemanager.h"

ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
    : texGrass(":/images/grass.png"),
      texWater(":/images/water.png"),
      texRock(":/images/rock.png")
{
}

ResourceManager *ResourceManager::instance()
{
    if (!m_instance)
        m_instance = new ResourceManager;
    return m_instance;
}

const QImage &ResourceManager::grassTexture() const
{
    return texGrass;
}

const QImage &ResourceManager::waterTexture() const
{
    return texWater;
}

const QImage &ResourceManager::rockTexture() const
{
    return texRock;
}
