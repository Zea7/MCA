#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>

#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>

#include <QMenuBar>
#include <QStatusBar>

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <QDebug>

#include <vector>
#include <algorithm>

#include "menubar.h"
#include "toolbar.h"
#include "data.h"
#include "utils.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    /*  
        UI를 세팅하는 함수들. 

        setUI()         :   메인 위젯을 설정해주고 기본적인 창의 크기를 결정하는 함수.
        setMCAMenuBar()    :   메뉴바의 형태를 결정짓는 함수.
        setChartView()   :   그래프를 보여줄 chart의 형태를 결정하는 함수.
    */


    void setUI();

    void setMCAMenuBar();

    void setChartView();

    void setSignalFromMenuBar();

    QWidget *mainWidget;
    QGridLayout *mainLayout;

    QHBoxLayout *bottomRemoteControl;
    QVBoxLayout *sidePannel;

    QChart *mainChart;
    QBarSeries *mainSeries;
    QChartView *mainChartView;
    QValueAxis *axisX;
    QValueAxis *axisY;

    int startSample = 0;
    int endSample = 2048;
    int maxMagnitude = 1;
    int threshold = 0;

    FileMenu *fileMenu;
    ViewMenu *viewMenu;
    AcquisitionMenu *acquisitionMenu;
    DisplayMenu *displayMenu;
    DPPMenu *dppMenu;
    AnalyzeMenu *analyzeMenu;
    HelpMenu *helpMenu;

    ToolBar *toolbar;

    MCAData *data;

private slots:
    void openMCAFile();
    void resizeXAxis();
    void resizeYAxis();
};

#endif