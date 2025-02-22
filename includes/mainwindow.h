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

#include "widgets.h"
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    /*  
        UI를 세팅하는 함수들. 

        setMainUI()         :   Window의 CentralWidget을 MainWindow::mainWidget으로 설정하고 
                                mainWidget의 side, bottom pannel과 chartview의 사이즈를 설정하는 함수.
        setMCAMenuBar()     :   메뉴바의 형태를 결정짓는 함수.
        setMainChartUI()    :   mainWidget에 들어가는 chartview의 형태를 전부 결정하는 함수.   
        setSidePannelUI()   :   mainWidget의 side pannel tab의 형태를 전부 결정하는 함수.
        setBottomControlPannelUI()  :   mainWidget의 bottom control pannel의 형태를 전부 결정하는 함수.
    */

    void setMainUI();
    void setMainChartUI();
    void setSidePannelUI();
    void setBottomControlPannelUI();

    void setMenuBar();

    // void setSignalFromMenuBar();

    // void setInfoTab(QStringList list);
    // // std::pair<double, double> getLinearFuncFromData(std::vector<std::pair<int, int>> points);
    // // std::pair<double, double> getGaussianDistributionFromData(std::vector<double> dataset);
    // double getAreaSizeFromData(std::vector<double> dataset);

    QWidget *mainWidget;
    QGridLayout *mainLayout;

    QLabel *xData;
    QLabel *yData;

    QGridLayout *bottomControlPannel;
    QTabWidget *sidePannel;

    QCheckBox *setLogScaleCheckBox;

    QWidget *basicInformationTab;
    ROITabWidget *regionInformationTab;

    MainSpectrumChart *mainChart;
    QChartView *mainChartView;
    QValueAxis *axisX;
    QValueAxis *axisY;

    int startSample = 0;
    int endSample = 1024;
    int maxMagnitude = 1;
    int threshold = 0;


    /* 
        for test
    */
    int openTime = 0;


    // FileMenu *fileMenu;
    // ViewMenu *viewMenu;
    // AcquisitionMenu *acquisitionMenu;
    // DisplayMenu *displayMenu;
    // AnalyzeMenu *analyzeMenu;
    // HelpMenu *helpMenu;

    // ToolBar *toolbar;

    // MCAData *data;

    // ROI
    std::vector<std::pair<int, int>> roiRegions;

    // Peaks
    // std::vector<PeakInfo> peaks;

    // UartCommunicator *uart;

    // DetectThread *detectThread;

signals:
    void setSampleRange(int start, int end);
    void setMaxMagnitude(int magnitude);
    void resizeXAxis();
    void resizeYAxis();

    void setROIRegion(std::vector<std::pair<int, int>> roiRegions);
    void selectedROIRegionToShow(std::vector<std::pair<int, int>> roiRegions);

    void sendGaussianDistributionData(std::vector<std::pair<double, double>> data);

private slots:
    // void openMCAFile();
    // void saveMCAFile();
    // void autoResizeXAxis();
    // void autoResizeYAxis();
    
    // // ROI
    // void openROIDialog();
    // void getROIRegions(std::vector<std::pair<int, int>> roiRegions);

    // void showSpecificRegion();
    // void sendSpecificRegion(int start, int end);

    // // SpectoChart
    // void ShowHoveredData(bool status, int index, int value);

    // // Auto peak search
    // void openAutoPeakDialog();
    // void doAutoPeakSearch(int start, int end, int left, int right);

    // void getArgumentsToCalculateGaussian(int roiRegionIndex, int pointIndex);

    // void getSerialPort(QString port);

    // void startDetection();
    // void stopDetection();

    // void setMainChartData(std::vector<int> data);
};

#endif