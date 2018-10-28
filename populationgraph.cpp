#include "populationgraph.h"
#include "ui_populationgraph.h"

PopulationGraph::PopulationGraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopulationGraph)
{
    ui->setupUi(this);

    //

    current.c[Rabbits] = 0;
    current.c[Hunters] = 0;
    current.c[Wolves] = 0;

    max.c[Rabbits] = 0;
    max.c[Hunters] = 0;
    max.c[Wolves] = 0;

    //

    seriesR = new QLineSeries();
    seriesR->setName("Rabbits");

    seriesH = new QLineSeries();
    seriesH->setName("Hunters");

    seriesW = new QLineSeries();
    seriesW->setName("Wolves");

    chart = new QChart();
    chart->addSeries(seriesR);
    chart->addSeries(seriesH);
    chart->addSeries(seriesW);
    chart->createDefaultAxes();

    //

    //QValueAxis *xAxis = new QValueAxis();
    //xAxis->setTickCount(1);
    //chart->setAxisX(xAxis);

    //

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //

    ui->lyMain->addWidget(chartView);
    setLayout(ui->lyMain);
}

PopulationGraph::~PopulationGraph()
{
    delete ui;
}

void PopulationGraph::RegisterStats(int rabbitCount, int hunterCount, int wolfCount, bool update)
{
    current.c[Rabbits] = rabbitCount;
    current.c[Hunters] = hunterCount;
    current.c[Wolves] = wolfCount;

    for (int i = 0; i < STAT_COUNT; i++)
    {
        if (current.c[i] > max.c[i])
        {
            max.c[i] = current.c[i];
        }
    }

    history.append(current);

    if (update)
    {
        updateWindow();
    }
}

void PopulationGraph::ResetStats(bool update)
{
    for (int i = 0; i < STAT_COUNT; i++)
    {
        max.c[i] = 0;
        current.c[i] = 0;
    }

    history.resize(0);

    if (update)
    {
        updateWindow();
    }
}

void PopulationGraph::updateWindow()
{
    ui->lblCurRabbits->setText(QString("%1 rabbits").arg(current.c[Rabbits]));
    ui->lblCurHunters->setText(QString("%1 hunters").arg(current.c[Hunters]));
    ui->lblCurWolves->setText(QString("%1 wolves").arg(current.c[Wolves]));

    ui->lblMaxRabbits->setText(QString("%1 rabbits").arg(max.c[Rabbits]));
    ui->lblMaxHunters->setText(QString("%1 hunters").arg(max.c[Hunters]));
    ui->lblMaxWolves->setText(QString("%1 wolves").arg(max.c[Wolves]));

    //

    seriesR->clear();
    seriesH->clear();
    seriesW->clear();

    for (int i = 0; i < history.size(); i++)
    {
        seriesR->append(i, history[i].c[Rabbits]);
        seriesH->append(i, history[i].c[Hunters]);
        seriesW->append(i, history[i].c[Wolves]);
    }

    chart->axisX()->setRange(0, history.size() - 1);

    int m = max.c[Rabbits];
    m = m > max.c[Hunters] ? m : max.c[Hunters];
    m = m > max.c[Wolves] ? m : max.c[Wolves];
    chart->axisY()->setRange(0, m);
}
