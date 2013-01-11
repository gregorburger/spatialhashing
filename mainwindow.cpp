#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsEllipseItem>

#include "graphicsscene.h"
#include "spatialhash.h"
#include <boost/random.hpp>



MainWindow::MainWindow(SpatialHash<vector> *hash, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), hash(hash) {
    ui->setupUi(this);
    this->addAction(ui->actionDraw);

    scene = new GraphicsScene(hash);
    ui->graphicsView->setScene(scene);
    on_actionDraw_activated();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *) {
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}


void MainWindow::on_actionDraw_activated() {
    scene->clear();
    const float pdim = 0.005;
    for (auto &kvpair: hash->map) {
        const tbb::concurrent_vector<vector> &cell = *kvpair.second;
        for (const vector &v: cell) {
            scene->addEllipse(v.x - pdim, v.y - pdim, 2*pdim, 2*pdim);
        }
    }
    for (double x = 0.0; x <= 1.0; x += hash->cell_size) {
        scene->addLine(x, 0, x, 1.0);
        scene->addLine(0, x, 1.0, x);
    }
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}
