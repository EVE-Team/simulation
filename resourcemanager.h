#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QImage>

class ResourceManager
{
public:
    static ResourceManager *instance();

    const QImage &grassTexture() const;

private:
    ResourceManager();

    static ResourceManager *m_instance;

    QImage texGrass;
};

#endif // RESOURCEMANAGER_H
