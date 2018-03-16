#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <QDebug>
#include <algorithm>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    m_menu_calculate = menuBar()->addMenu(tr("Calculate"));
    m_menu_calculate->addAction(m_action_convolve);

    m_menu_generate = menuBar()->addMenu(tr("Generate"));
    m_menu_generate->addAction(m_action_generate);

    m_menu_plot = menuBar()->addMenu(tr("Plot"));
    m_menu_plot->addAction(m_action_impulsePlot);
}

void MainWindow::createActions()
{
    m_action_convolve = new QAction(tr("Convolve"), this);
    connect(m_action_convolve, &QAction::triggered, this, &MainWindow::calculateConvolve);

    m_action_generate = new QAction(tr("Generate signal"), this);
    connect(m_action_generate, &QAction::triggered, this, &MainWindow::generateData);

    m_action_impulsePlot = new QAction(tr("Plot signal"), this);
    connect(m_action_impulsePlot, &QAction::triggered, this, &MainWindow::plotData);
}

//обчислення згортки двох сигналів
void MainWindow::calculateConvolve()
{
    m_signal_x = {1, 2, 3};
    m_signal_y = {1, 3, 5};
    m_signal_convolve = std::vector<double>(m_signal_x.size() +
                                           m_signal_y.size() - 1);

    std::reverse(m_signal_y.begin(), m_signal_y.end());

    for (size_t i = 0; i < m_signal_x.size(); ++i)
    {
        for (size_t j = 0; j < m_signal_y.size(); ++j)
        {
            m_signal_convolve.at(i + j) +=m_signal_x[i] * m_signal_y[j];
        }
    }

    qDebug() << "signal conv...\n";
    for (auto const&  item: m_signal_convolve)
    {
        qDebug() << item;
    }
}
//генерування сигналів
void MainWindow::generateData()
{
    m_signal_x.clear();
    m_signal_y.clear();

    const double l_Amp1 = 1;
    const double l_Amp2 = 1;
    const double l_Freq1 = 100;
    const double l_Freq2 = 150;

    double l_nyqist_freq = 13.5 * std::max(l_Freq1, l_Freq2);
    double dt = 1.0 / l_nyqist_freq;
    for (int i = 0; i < m_nPoints; ++i)
    {
        m_signal_x.push_back(l_Amp1 * std::sin(2. * M_PI * l_Freq1 * i * dt) +
                             l_Amp2 * std::sin(2. * M_PI * l_Freq2 * i * dt));
    }
}

void MainWindow::plotData()
{
    QtCharts::QLineSeries* l_series = new QtCharts::QLineSeries();

    for (int i = 0; i < m_nPoints; ++i)
    {
        l_series->append(i, m_signal_x.at(i));
    }

    QtCharts::QChart* l_chart = new QtCharts::QChart();
    l_chart->addSeries(l_series);
    l_chart->createDefaultAxes();

    QtCharts::QChartView* l_chart_view = new QtCharts::QChartView(l_chart);
    this->setCentralWidget(l_chart_view);
}
