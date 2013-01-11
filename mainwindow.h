#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spatialhash.h"

class GraphicsScene;
class QResizeEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(SpatialHash<vector> *hash, QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent *);

private slots:
    void on_actionDraw_activated();

private:
    Ui::MainWindow *ui;
    GraphicsScene *scene;
    SpatialHash<vector> *hash;
};

#endif // MAINWINDOW_H
