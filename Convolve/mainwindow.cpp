#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <QDebug>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  // m_signal_x(m_nPoints), m_signal_y(m_nPoints)
{
    ui->setupUi(this);
    m_signal_x = std::move(std::vector<double>(m_nPoints));
    m_signal_y = std::move(std::vector<double>(m_nPoints));

    createActions();
    createMenus();
    m_isDFT = false;
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_chart;
    delete m_series;
    delete m_chart_view;
}

std::vector<std::complex<double> > MainWindow::DFT(std::vector<std::complex<double> > data)
{
    int n = data.size();
    std::vector<std::complex<double>> outData(n);

    for (int i = 0; i < n; ++i)
    {
        double sumR = 0.0;
        double sumI = 0.0;
        for (int j = 0; j < n; ++j)
        {
            double a = 2 * M_PI * j * i / n;
            sumR += data[j].real() * cos(a) + data[j].imag() * sin(a);
            sumI += -data[j].real() * sin(a) + data[j].imag() * cos(a);
        }
        outData[i].real(sumR);
        outData[i].imag(sumI);
    }
    return std::move(outData);
}

void MainWindow::createMenus()
{
    m_menu_calculate = menuBar()->addMenu(tr("Calculate"));
    m_menu_calculate->addAction(m_action_convolve);
    m_menu_calculate->addAction(m_action_scale);
    m_menu_calculate->addAction(m_action_reverse_time);
    m_menu_calculate->addAction(m_action_distension_time);
    m_menu_calculate->addAction(m_action_shift_time);
    m_menu_calculate->addAction(m_action_DFT);

    m_menu_generate = menuBar()->addMenu(tr("Generate"));
    m_menu_generate->addAction(m_action_generate);

    m_menu_plot = menuBar()->addMenu(tr("Plot"));
    m_menu_plot->addAction(m_action_impulsePlot);
    m_menu_plot->addAction(m_action_modifiedDataPlot);
}

void MainWindow::createActions()
{
    m_action_convolve = new QAction(tr("Convolve"), this);
    connect(m_action_convolve, &QAction::triggered, this, &MainWindow::slotConvolve);

    m_action_scale = new QAction(tr("Scale"), this);
    connect(m_action_scale, &QAction::triggered, this, &MainWindow::slotScale);

    m_action_reverse_time = new QAction(tr("Reverse"), this);
    connect(m_action_reverse_time, &QAction::triggered, this, &MainWindow::slotReverseTime);

    m_action_distension_time = new QAction(tr("Time distension"), this);
    connect(m_action_distension_time, &QAction::triggered, this, &MainWindow::slotDistensionTime);

    m_action_shift_time = new QAction(tr("Time shift"), this);
    connect(m_action_shift_time, &QAction::triggered, this, &MainWindow::slotShiftTime);

    m_action_DFT = new QAction(tr("DFT"), this);
    connect(m_action_DFT, &QAction::triggered, this, &MainWindow::slotDFT);

    m_action_generate = new QAction(tr("Generate signal"), this);
    connect(m_action_generate, &QAction::triggered, this, &MainWindow::slotGenerateData);

    m_action_impulsePlot = new QAction(tr("Plot signal"), this);
    connect(m_action_impulsePlot, &QAction::triggered, this, &MainWindow::slotPlotData);

    m_action_modifiedDataPlot = new QAction(tr("Plot Modified data"), this);
    connect(m_action_modifiedDataPlot, &QAction::triggered, this, &MainWindow::slotPlotModifyedData);
}

//обчислення згортки двох сигналів
void MainWindow::slotConvolve()
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
     m_isDFT = false;
}
//генерування сигналів
void MainWindow::slotGenerateData()
{
    const double l_Amp1 = 10;
    const double l_Amp2 = 7;
    const double l_Freq1 = 100;
    const double l_Freq2 = 200;

    double l_nyqist_freq = 5 * std::max(l_Freq1, l_Freq2);
    m_dt = 1.0 / l_nyqist_freq;
    for (int i = 0; i < m_nPoints; ++i)
    {
        m_signal_x.at(i) = l_Amp1 * std::cos(2. * M_PI * l_Freq1 * i * m_dt)
                + l_Amp2 * std::cos(2. * M_PI * l_Freq2 * i * m_dt);
    }
    m_isDFT = false;
}

void MainWindow::slotPlotData()
{
    m_series = new QtCharts::QLineSeries();
    for (int i = 0; i < m_nPoints; ++i)
    {
        m_series->append(i, m_signal_x.at(i));
    }
    m_chart = new QtCharts::QChart();
    m_chart->addSeries(m_series);
    m_chart->createDefaultAxes();
    m_chart->axisY()->setRange(*std::min_element(m_signal_x.cbegin(), m_signal_x.cend()),
                               *std::max_element(m_signal_x.cbegin(), m_signal_x.cend()));
    m_chart_view = new QtCharts::QChartView(m_chart);
    this->setCentralWidget(m_chart_view);
}

void MainWindow::slotPlotModifyedData()
{
    if (m_series != nullptr)
    {
        m_series->clear();
    }

    if (!m_isDFT)
    {
        for (int i = 0; i < m_nPoints; ++i)
        {
            m_series->append(i, m_signal_y.at(i));
        }
    }
    else
    {
        for (int i = 0; i < m_nPoints; ++i)
        {
            m_series->append((1.0 / m_dt) / m_nPoints * i, m_signal_y.at(i));
        }
    }

    m_chart->axisY()->setRange(*std::min_element(m_signal_y.cbegin(), m_signal_y.cend()),
                                   *std::max_element(m_signal_y.cbegin(), m_signal_y.cend()));
    m_chart->axisX()->setRange(0, (1.0 / m_dt) / m_nPoints * m_nPoints);
}

void MainWindow::slotScale()
{
    const double alp = 2;


    for (size_t i = 0; i < m_signal_x.size(); ++i)
    {
        m_signal_y[i] = alp * m_signal_x[i];
    }
    m_isDFT = false;
}

void MainWindow::slotReverseTime()
{
    m_signal_y = std::vector<double>(m_signal_x.crbegin(), m_signal_x.crend());
     m_isDFT = false;
}

void MainWindow::slotShiftTime()
{
    const size_t shift = 50;
    std::fill(m_signal_y.begin(), m_signal_y.end(), 0);
    for (size_t i = 0; i < m_nPoints - shift; ++i)
    {
        m_signal_y[i] = m_signal_x[i + shift];
    }
    m_isDFT = false;
}

void MainWindow::slotDistensionTime()
{
    const size_t shift = 5;
    std::fill(m_signal_y.begin(), m_signal_y.end(), 0);
    for (size_t i = 0; i < m_nPoints / shift; ++i)
    {
        m_signal_y[i] = m_signal_x[i * shift];
    }
    m_isDFT = false;
}

void MainWindow::slotDFT()
{
    std::vector<std::complex<double>> inSig(m_nPoints);

    for (int i = 0; i < m_nPoints; ++i)
    {
        inSig[i].real(m_signal_x[i]);
        inSig[i].imag(0);
    }

    auto outData = DFT(inSig);

    for (int i = 0; i < m_nPoints/2; ++i)
    {
        m_signal_y.at(i) = std::abs(outData[i]);
    }
    m_isDFT = true;
}
