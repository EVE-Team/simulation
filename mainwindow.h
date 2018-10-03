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

private slots:
    void on_btnTick_clicked();

private:
    Ui::MainWindow *ui;

    double zoomLevel;
    QPoint translPoint;

    bool isPanning;
    QPoint panStartingPoint;
    QPoint translOnPanStart;

    World world;
};

#endif // MAINWINDOW_H
