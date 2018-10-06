#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QImage>

class ResourceManager
{
public:
    static ResourceManager *instance();

    const QImage &grassTexture() const;
    const QImage &waterTexture() const;
    const QImage &rockTexture() const;

    const QImage &sunIcon() const;
    const QImage &rainIcon() const;
    const QImage &grassIcon() const;

private:
    ResourceManager();

    static ResourceManager *m_instance;

    QImage texGrass, texWater, texRock;
    QImage iconSun, iconRain, iconGrass;
};

#endif // RESOURCEMANAGER_H
