#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "world.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    void on_lblDrawArea_wheel(QWheelEvent *event);
    void on_lblDrawArea_mouseMove(QMouseEvent *event);
    void on_lblDrawArea_mouseButtonPress(QMouseEvent *event);
    void on_lblDrawArea_mouseButtonRelease(QMouseEvent *event);

    void resizeEvent(QResizeEvent* event);

    void redrawWorld();

    // returns pointer to a cell located under specified point, or nullptr if there's no such cell
    // point - coordinates of a point inside viewport (lblDrawArea)
    Cell *getCellFromFromPoint(QPoint point);
    // user can select a cell to 'watch' it
    // this updates info on such cell
    // celled from redrawWorld()
    void updateWatchedCell();

    void addCreatures(int creatureType, int count);

private slots:
    void on_btnTick_clicked();

    void on_btnResize_clicked();

    void on_btnAddRabbits_clicked();

    void on_btnAddHunters_clicked();

private:
    Ui::MainWindow *ui;

    QPixmap renderBuffer;

    double zoomLevel;
    QPoint translPoint;

    bool isPanning;
    QPoint panStartingPoint;
    QPoint translOnPanStart;

    World world;

    Cell *watchedCell;
};

#endif // MAINWINDOW_H
