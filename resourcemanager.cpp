#include "resourcemanager.h"
#include <QtDebug>

ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
    : texGrass(":/images/grass.png")
{
    qDebug() << texGrass.size();
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
