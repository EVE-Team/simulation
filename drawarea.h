#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>

class DrawArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawArea(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
};

#endif // DRAWAREA_H
