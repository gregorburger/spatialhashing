#include "graphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>
#include "spatialhash.h"

GraphicsScene::GraphicsScene(SpatialHash<vector> *hash, QObject *parent) :
    QGraphicsScene(parent), hash(hash) {
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
    float x = event->scenePos().x();
    float y = event->scenePos().y();

    std::vector<vector> points = hash->query(vector(x, y));

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidthF(0.005);

    const float pdim = 0.005;
    for (const vector &v: points) {
        addEllipse(v.x - pdim, v.y - pdim, 2*pdim, 2*pdim, pen);
    }

    double rad = hash->cell_size;
    addEllipse(x - rad, y - rad, 2*rad, 2*rad);
}
