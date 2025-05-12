#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QComboBox>

#include <QLineEdit>
#include <QStringList>

#include <QLabel>
#include <QChartView>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <algorithm>
#include <vector>

#include "widgets.h"
#include "types.h"

class SpectrumListManager : public QDialog {
    Q_OBJECT

public:
    SpectrumListManager();
    ~SpectrumListManager();

private:
    /* 
        private UI functions

        void setUI()    :   df
        void setOKCancelLayoutUI()     :   
        void setChartPreviewUI()    :   
        void setSpectrumListLayoutUI()   :   
    */
    void setUI();
    void setOKCancelLayoutUI();
    void setChartPreviewUI();
    void setSpectrumListLayoutUI();

    QVBoxLayout *mainLayout;

    QVBoxLayout *spectrumListLayout;
    QHBoxLayout *spectrumSettingLayout;
    QHBoxLayout *spectrumSettingHintLabelLayout;
    QPushButton *addSpectrumButton;
    QComboBox *spectrumListComboBox;
    QComboBox *channelSizeSelectComboBox;
    QPushButton *changeColorPushButton;

    QHBoxLayout *OKCancelLayout;
    QPushButton *OKButton;
    QPushButton *cancelButton;

    SpectrumChart *previewChart;
    QChartView *previewChartView;

    LevelSeriesData *data;

private:
    void setSignalSlotConnection();

private slots:
    void openMCAFile();

};

class ROIRegionManager : public QDialog {
    Q_OBJECT

public:
    ROIRegionManager();
    ROIRegionManager(std::vector<std::pair<int, int>> roiRegions);
    ~ROIRegionManager();

private:
    /* 
        Layouts

        mainLayout     :   가장 바깥쪽 레이아웃
        ROIManageLayout     :   위쪽 상세내역이 존재하는 레이아웃
        OKCancelLayout  :   OK, Cancel 버튼이 있는 레이아웃
        settingButtonsLayout   :   각종 버튼이 존재하는 레이아웃
        showROIListLayout   :   ROI 설정과 관련된 위젯이 존재하는 레이아웃    
    */
    QVBoxLayout *mainLayout;
    QGridLayout *ROIManageLayout;
    QHBoxLayout *OKCancelLayout;
    QVBoxLayout *settingButtonsLayout;


    QLineEdit *ROIStartInput;
    QLineEdit *ROIEndInput;
    QTableWidget *showROIListTable;


    QPushButton *addRegionButton;
    QPushButton *replaceRegionButton;
    QPushButton *removeRegionButton;
    QPushButton *removeAllRegionsButton;
    QPushButton *selectRegionButton;

    QPushButton *OKButton;
    QPushButton *cancelButton;

    std::vector<std::pair<int, int>> roiRegions;

    void setUI();
    void setShowROIListTableUI();
    void setROIManageLayoutUI();
    void setOKCancelLayoutUI();
    void setSettingButtonsLayoutUI();

private:    
    void setSignalSlotConnection();
    void setShowROIListTable();

signals:
    void sendROIRegions(std::vector<std::pair<int, int>> roiRegions);
    void sendShowRegion(int start, int end);

private slots:
    void addRegion();
    void removeRegion();
    void replaceRegion();
    void removeAllRegions();
    void okSignal();
    void selectRegion();

};

#endif