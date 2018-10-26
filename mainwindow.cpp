#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMessageBox>
#include <QRandomGenerator>
#include "hunter.h"

const double zoomScaleFactor = 1.5;
const QSize worldSize(3, 3); // default world size

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    zoomLevel(1.0),
    isPanning(false),
    world(worldSize)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);

    ui->spnWidth->setValue(world.getSize().width());
    ui->spnHeight->setValue(world.getSize().height());

    // render world
    redrawWorld();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    ui->lblDrawArea->move(5, 5);
    ui->lblDrawArea->resize(size().width() - ui->grControls->size().width() - 10, size().height() - 5);
    ui->grControls->move(ui->lblDrawArea->size().width() + 10, 5);
    ui->grControls->resize(ui->grControls->size().width(), ui->lblDrawArea->size().height());

    redrawWorld();
}

void MainWindow::redrawWorld()
{
    const QSize renderAreaSize = ui->lblDrawArea->size();
    if (renderBuffer.size() != renderAreaSize)
    {
        renderBuffer = QPixmap(renderAreaSize);
    }

    QPainter painter(&renderBuffer);

    QRect fullArea(0, 0, renderAreaSize.width(), renderAreaSize.height());
    painter.fillRect(fullArea, QBrush(Qt::lightGray));
    QBrush background(Qt::darkGray, Qt::Dense4Pattern);
    painter.fillRect(fullArea, background);

    //painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(translPoint);
    painter.scale(zoomLevel, zoomLevel);

    world.render(painter);

    ui->lblDrawArea->setPixmap(renderBuffer);

    //

    ui->lblGlobalStats->setText(QString("%1 rabbits in total\n%2 hunters in total").
                                arg(world.getCreaturePopulation(CREATURE_TYPE_RABBIT)).
                                arg(world.getCreaturePopulation(CREATURE_TYPE_HUNTER)));
}

// ignore "enumeration value '...' not handled in switch' warning
#pragma GCC diagnostic ignored "-Wswitch"

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
    QPoint mousePosition = event->pos();

    if (isPanning)
    {
        static QPoint lastPanDelta;
        QPoint panDelta = mousePosition - panStartingPoint;

        if (lastPanDelta != panDelta)
        {
            //qDebug() << "panning " << panDelta;

            translPoint = translOnPanStart + panDelta;
            redrawWorld();

            lastPanDelta = panDelta;
        }
    } else {
        Cell *selectedCell = getCellFromFromPoint(mousePosition);
        if (selectedCell == nullptr)
        {
            ui->lblTileInfo->setText("Selected tile: None");
        } else {
            const static char* terrainNames[] = { "Grass", "Water", "Mountain" };
            ui->lblTileInfo->setText(QString("Selected tile: %1x%2\nTerrain: %3\nSun: %4; Rain: %5\nGrass: %6\nRabbits: %7\nHunters: %8").
                    arg(selectedCell->getPosition().x() + 1).arg(selectedCell->getPosition().y() + 1).
                    arg(terrainNames[selectedCell->getTerrain()], QString::number(selectedCell->getSunLevel()),
                                     QString::number(selectedCell->getRainLevel()),
                                     QString::number(selectedCell->getGrassLevel()),
                                     QString::number(selectedCell->getRabbitCount()),
                                     QString::number(selectedCell->getCreatureCount(CREATURE_TYPE_HUNTER))));
        }
    }
}

Cell *MainWindow::getCellFromFromPoint(QPoint point)
{
    QTransform viewportTransform;
    viewportTransform.translate(translPoint.x(), translPoint.y());
    viewportTransform.scale(zoomLevel, zoomLevel);
    viewportTransform = viewportTransform.inverted();

    QPoint projectedPosition = viewportTransform.map(point);
    if (projectedPosition.x() < 0 || projectedPosition.y() < 0)
    {
        return nullptr;
    }
    int tileX = projectedPosition.x() / 100, tileY = projectedPosition.y() / 100;
    if (tileX >= world.getSize().width() || tileY >= world.getSize().height())
    {
        return nullptr;
    }

    return world.cellAt(tileX, tileY);
}

void MainWindow::on_lblDrawArea_mouseButtonPress(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
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
        ui->lblDrawArea->setCursor(Qt::OpenHandCursor);
        isPanning = false;
    }
}

void MainWindow::on_btnTick_clicked()
{
    for (int i = 0; i < ui->spnTickCount->value(); i++)
    {
        world.advance();
    }

    redrawWorld();
}

void MainWindow::on_btnResize_clicked()
{
    world.resize(QSize(ui->spnWidth->value(), ui->spnHeight->value()));
    redrawWorld();
}

void MainWindow::addCreatures(int creatureType, int count)
{
    const int world_w = world.getSize().width(), world_h = world.getSize().height();
    int maxNumberOfCreaturesToAdd = 0;

    for (int x = 0; x < world_w; x++)
    {
        for (int y = 0; y < world_h; y++)
        {
            Cell *cell = world.cellAt(x, y);
            if (cell->getTerrain() == Cell::Terrain::Grass)
            {
                maxNumberOfCreaturesToAdd += 3 - cell->getCreatureCount(creatureType);
            }
        }
    }

    //

    if (count > maxNumberOfCreaturesToAdd)
    {
        QMessageBox msgbox;
        msgbox.setText(QString("%1 out of %2 creatures will be added due to space limitations").arg(maxNumberOfCreaturesToAdd).arg(count));
        msgbox.setIcon(QMessageBox::Warning);
        msgbox.exec();

        count = maxNumberOfCreaturesToAdd;
    }

    //

    QRandomGenerator *random = QRandomGenerator::global();
    do {
        int xpos = random->bounded(0, world_w), ypos = random->bounded(0, world_h);
        Cell *cell = world.cellAt(xpos, ypos);
        if (cell->getTerrain() == Cell::Terrain::Grass)
        {
            if (cell->getCreatureCount(creatureType) < 3)
            {
                Creature *newCreature;

                switch (creatureType)
                {
                case CREATURE_TYPE_RABBIT:
                    newCreature = new Rabbit();
                    break;
                case CREATURE_TYPE_HUNTER:
                    newCreature = new Hunter();
                    break;
                default:
                    abort();
                }

                assert(cell->addCreature(newCreature));
                count--;
            }
        }
    } while (count > 0);

    //

    redrawWorld();
}

void MainWindow::on_btnAddRabbits_clicked()
{
    addCreatures(CREATURE_TYPE_RABBIT, ui->spnRabbitCount->value());
}

void MainWindow::on_btnAddHunters_clicked()
{
    addCreatures(CREATURE_TYPE_HUNTER, ui->spnHunterCount->value());
}
