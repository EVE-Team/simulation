#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

const double zoomScaleFactor = 1.5;
const QSize worldSize(3, 3);

const Cell::Terrain terrain[3][3] =
    {
      { Cell::Terrain::Water, Cell::Terrain::Water,    Cell::Terrain::Grass },
      { Cell::Terrain::Grass, Cell::Terrain::Mountain, Cell::Terrain::Grass },
      { Cell::Terrain::Grass, Cell::Terrain::Grass,    Cell::Terrain::Grass }
    };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    zoomLevel(1.0),
    isPanning(false),
    world(worldSize)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    for (int x = 0; x < world.getSize().width(); x++)
    {
        for (int y = 0; y < world.getSize().height(); y++)
        {
            world.cellAt(x, y)->setTerrain(terrain[y][x]);
        }
    }

    redrawWorld();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    world.advance();
    redrawWorld();
}

void MainWindow::redrawWorld()
{
    QPixmap pixmap(ui->lblDrawArea->width(), ui->lblDrawArea->height());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(translPoint);
    painter.scale(zoomLevel, zoomLevel);

    world.render(painter);

    ui->lblDrawArea->setPixmap(pixmap);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lblDrawArea)
    {
        switch (event->type())
        {
        case QEvent::Wheel:
            on_lblDrawArea_wheel(static_cast<QWheelEvent*>(event));
            break;
        case QEvent::MouseButtonPress:
            on_lblDrawArea_mouseButtonPress(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            on_lblDrawArea_mouseButtonRelease(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            on_lblDrawArea_mouseMove(static_cast<QMouseEvent*>(event));
            break;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::on_lblDrawArea_wheel(QWheelEvent *event)
{
    double angle = event->angleDelta().ry() / 120.0;

    // positive - zoom in
    // negative - zoom out

    if (angle < 0)
    {
        zoomLevel /= -angle * zoomScaleFactor;
    } else {
        zoomLevel *= angle * zoomScaleFactor;
    }

    //qDebug() << "New zoom level = " << zoomLevel;

    redrawWorld();
}

void MainWindow::on_lblDrawArea_mouseMove(QMouseEvent *event)
{
    if (isPanning)
    {
        static QPoint lastPanDelta;
        QPoint panDelta = event->pos() - panStartingPoint;

        if (lastPanDelta != panDelta)
        {
            //qDebug() << "panning " << panDelta;

            translPoint = translOnPanStart + panDelta;
            redrawWorld();

            lastPanDelta = panDelta;
        }
    }
}

void MainWindow::on_lblDrawArea_mouseButtonPress(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //qDebug() << "button pressed";
        ui->lblDrawArea->setCursor(Qt::ClosedHandCursor);

        panStartingPoint = event->pos();
        translOnPanStart = translPoint;
        isPanning = true;
    }
}

void MainWindow::on_lblDrawArea_mouseButtonRelease(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //qDebug() << "button released";
        ui->lblDrawArea->setCursor(Qt::OpenHandCursor);
        isPanning = false;
    }
}
