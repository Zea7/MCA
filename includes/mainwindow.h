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

#include <QMenu>
#include <QAction>
#include <QToolBar>

#include <QFile>
#include <QFileDialog>
#include <QThread>

#include "widgets.h"
#include "types.h"
#include "dialogs.h"
#include "utils.h"
#include "threads.h"


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

    void setMenuAndToolBarUI();

    QWidget *mainWidget;
    QGridLayout *mainLayout;

    QLabel *xData;
    QLabel *yData;

    QGridLayout *bottomControlPannel;
    QTabWidget *sidePannel;

    QCheckBox *setLogScaleCheckBox;

    InfoTabWidget *basicInformationTab;
    ROITabWidget *regionInformationTab;

    SpectrumChart *mainChart;
    SpectrumChartView *mainChartView;
    QValueAxis *axisX;
    QValueAxis *axisY;

    int startSample = 0;
    int endSample = 1024;
    int maxMagnitude = 1;
    int threshold = 0;

    int mainMCADataIndex = -1; // 저장되는 등 주로 활용될 MCA DATA Index

    /* 
        for test
    */
    int openTime = 0;

    // MCA Data vector
    std::vector<std::shared_ptr<LevelSeriesData>> mainMCAData;
    std::vector<bool> activatedDataList; // MCA Data Vector에서 활성화되어 있는 리스트 위치
    LevelSeriesData *liveMCAData;
    std::shared_ptr<LevelSeriesData> dataCarrier;
    bool isLiveMeasuring = false;

    // ROI
    std::vector<std::pair<int, int>> roiRegions;
    int showingROIRegionIndex;

    SerialWorker *serialWorker;
    QThread *mainWorker;

// private slot for menu & tool bar
private:
    QMenu *fileMenu;
    QAction *file_open;
    QAction *file_closeAll;
    QAction *file_save;
    QAction *file_saveAs;
    QAction *file_preferences;
    QAction *file_exit;

    QMenu *viewMenu;
    QAction *view_toolBar;
    QAction *view_statusBar;
    QAction *view_spectraList;

    QMenu *acquisitionMenu;
    QAction *acquisition_acquisitionSetup;
    QAction *acquisition_changeStartStop;
    QAction *acquisition_repeatMesureTest;
    QAction *acquisition_deleteDatasetTime;
    QAction *acquisition_disconnect;

    QMenu *displayMenu;
    QAction *display_scale;
    QAction *display_calibration;

    QMenu *analyzeMenu;
    QAction *analyze_calibrate;
    QAction *analyze_peakSearch;
    QAction *analyze_defineROI;

    QMenu *helpMenu;
    QAction *help_help;

    QToolBar *toolBar;
    QAction *toolbar_open;
    QAction *toolbar_save;
    QAction *toolbar_resizeHorizontalAxis;
    QAction *toolbar_resizeVerticalAxis;
    
// private sections for main functions
private:
    void dialogTest();
    void setUISignalSlotConnection();
    void setSerialWorkerSignalSlotConnection();

signals:
    /**
     * @brief 이건 뭘까요
     * 
     */
    void mcaDataListChanged(std::vector<bool> indexes);

    void chartChanged();
    void sendGaussianDistributionData(std::vector<std::pair<double, double>> data);

    void roiRegionChanged(std::vector<std::pair<int, int>> roiRegions);

private slots:

    /**
     * @brief OOasd
     * 
     */
    void openMCAFile();

    /**
     * @brief 설명입니다
     * 
     * @param Asd:str adf 
     */
    void saveAsMCAFile();

    void showROIRegionManager();

    // SpectoChart
    void showClickedData(int xIndex);

    // ROI Tab Widget
    void calculateGaussianDistributionWithArguments(int roiRegionIndex, int pointIndex);

    void getSerialSetter(SerialSetter setter);

    /* 
    dialogs
     */
    void showSpectrumListManager();

    void getSerialData(const std::vector<int>& rawData);

    void startDetection();
    void stopDetection();

    void getROIRegions(std::vector<std::pair<int, int>> roiRegions);
    void setShowingROIRegion(int index);
};

#endif