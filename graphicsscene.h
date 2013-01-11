#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include "spatialhash.h"

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(SpatialHash<vector> *hash, QObject *parent = 0);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    SpatialHash<vector> *hash;
};

#endif // GRAPHICSSCENE_H
