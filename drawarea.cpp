#include "drawarea.h"
#include <QPainter>

DrawArea::DrawArea(QWidget *parent) : QWidget(parent)
{

}

void DrawArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawRect(30, 30, 100, 100);
}
