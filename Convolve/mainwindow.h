#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <vector>
#include <QMenu>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenu* m_menu_calculate;
    QMenu* m_menu_plot;
    QMenu* m_menu_generate;

    //меню обробки сигналу
    QAction* m_action_scale;
    QAction* m_action_convolve;
    QAction* m_action_generate;
    QAction* m_action_shift_time;
    QAction* m_action_reverse_time;
    QAction* m_action_distension_time;

    //меню відображення сигналу
    QAction* m_action_modifiedDataPlot;
    QAction* m_action_impulsePlot;

    QtCharts::QLineSeries* m_series;
    QtCharts::QChart* m_chart;
    QtCharts::QChartView* m_chart_view;

    const int m_nPoints = 100;
    //завжди малювати m_signal_x
    std::vector<double> m_signal_x;
    std::vector<double> m_signal_y;
    std::vector<double> m_signal_convolve;

private:
    void createMenus();
    void createActions();

private slots:
    void slotConvolve();
    void slotGenerateData();
    void slotPlotData(/*std::vector<double> data*/);
    void slotPlotModifyedData();
    void slotScale();
    void slotReverseTime();
    void slotShiftTime();
    void slotDistensionTime();
signals:
    //не виходить причепити до сигналу QAction::triggered
    void plot_data_signal(std::vector<double> data);
};

#endif // MAINWINDOW_H
