#ifndef POPULATIONGRAPH_H
#define POPULATIONGRAPH_H

#include <QDialog>
#include <QVector>
#include <QtCharts>

using namespace QtCharts;

namespace Ui {
class PopulationGraph;
}

#define STAT_COUNT 3

class PopulationGraph : public QDialog
{
    Q_OBJECT

public:
    explicit PopulationGraph(QWidget *parent = nullptr);
    ~PopulationGraph();

    void RegisterStats(int rabbitCount, int hunterCount, int wolfCount, bool updateWindow = true);
    void ResetStats(bool updateWindow = true);

private:
    Ui::PopulationGraph *ui;

    enum STAT_MEMBERS
    {
        Rabbits = 0,
        Hunters = 1,
        Wolves = 2
    };
    typedef struct
    {
        int c[STAT_COUNT];
    } STATS;

    STATS max, current;
    QVector<STATS> history;

    void updateWindow();

    QLineSeries *seriesR, *seriesH, *seriesW;
    QChart *chart;
    QChartView *chartView;
};

#endif // POPULATIONGRAPH_H
