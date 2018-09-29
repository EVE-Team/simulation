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

private:
    ResourceManager();

    static ResourceManager *m_instance;

    QImage texGrass, texWater, texRock;
};

#endif // RESOURCEMANAGER_H
