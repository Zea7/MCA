#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>


#include <QLabel>

#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>

#include <QMenuBar>
#include <QStatusBar>

#include <QCheckBox>
#include <QThread>

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <QDebug>

#include <vector>
#include <algorithm>

#include <thread>
#include <atomic>

#include "menubar.h"
#include "toolbar.h"
#include "data.h"
#include "utils.h"
#include "spectochart.h"
#include "dialogs.h"
#include "widgets.h"
#include "serial.h"
#include "qth.h"

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

    void setSidePannel();

    void setInfoTab(QStringList list);
    // std::pair<double, double> getLinearFuncFromData(std::vector<std::pair<int, int>> points);
    // std::pair<double, double> getGaussianDistributionFromData(std::vector<double> dataset);
    double getAreaSizeFromData(std::vector<double> dataset);

    QWidget *mainWidget;
    QGridLayout *mainLayout;

    QLabel *xData;
    QLabel *yData;

    QGridLayout *bottomRemoteControl;
    QTabWidget *sidePannel;

    QCheckBox *setLogScaleCheckBox;

    QWidget *infoTab;
    ROITab *roiTab;

    SpectoChart *mainChart;
    QChartView *mainChartView;
    QValueAxis *axisX;
    QValueAxis *axisY;

    int startSample = 0;
    int endSample = 1024;
    int maxMagnitude = 1;
    int threshold = 0;

    int getNum = 9654000;

    QString port;
    /* 
        for test
    */
    int openTime = 0;

    std::vector<int> tempData;


    FileMenu *fileMenu;
    ViewMenu *viewMenu;
    AcquisitionMenu *acquisitionMenu;
    DisplayMenu *displayMenu;
    AnalyzeMenu *analyzeMenu;
    HelpMenu *helpMenu;

    ToolBar *toolbar;

    MCAData *data;

    // ROI
    std::vector<std::pair<int, int>> roiRegions;

    // Peaks
    std::vector<PeakInfo> peaks;

    UartCommunicator *uart;

    DetectThread *detectThread;
    QThread *work;

signals:
    void setSampleRange(int start, int end);
    void setMaxMagnitude(int magnitude);
    void resizeXAxis();
    void resizeYAxis();

    void setROIRegion(std::vector<std::pair<int, int>> roiRegions);
    void selectedROIRegionToShow(std::vector<std::pair<int, int>> roiRegions);

    void sendGaussianDistributionData(std::vector<std::pair<double, double>> data);

private slots:
    void openMCAFile();
    void saveMCAFile();
    void autoResizeXAxis();
    void autoResizeYAxis();
    
    // ROI
    void openROIDialog();
    void getROIRegions(std::vector<std::pair<int, int>> roiRegions);

    void showSpecificRegion();
    void sendSpecificRegion(int start, int end);

    // SpectoChart
    void ShowHoveredData(bool status, int index, int value);

    // Auto peak search
    void openAutoPeakDialog();
    void doAutoPeakSearch(int start, int end, int left, int right);

    void getArgumentsToCalculateGaussian(int roiRegionIndex, int pointIndex);

    void getSerialPort(QString port);

    void startDetection();
    void stopDetection();

    void setMainChartData(std::vector<int> data);


};

#endif