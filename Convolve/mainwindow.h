#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <vector>
#include <QMenu>

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

    QAction* m_action_convolve;
    QAction* m_action_impulsePlot;
    QAction* m_action_generate;

    std::vector<double> m_signal_x;
    std::vector<double> m_signal_y;
    std::vector<double> m_signal_convolve;

    const int m_nPoints = 100;
private:
    void createMenus();
    void createActions();

private slots:
    void calculateConvolve();
    void generateData();
    void plotData();

};

#endif // MAINWINDOW_H
