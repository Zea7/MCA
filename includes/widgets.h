#ifndef WIDGETS_H
#define WIDGETS_H

#define DEFAULT_ROI_COLOR Qt::red
#define DEFAULT_CHANNEL_SIZE 1024
#define DEFAULT_CHANNEL_START 0

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <cmath>
#include <algorithm>

#include <QWidget>

#include <QVBoxLayout>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QPalette>
#include <QString>

#include <QChart>
#include <QStackedBarSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QBarSet>

#include <QChartView>
#include <QMouseEvent>
#include <QGraphicsLineItem>

#include "types.h"
#include "utils.h"
// #include "dialogs.h"


class SpectrumChart : public QChart {
    Q_OBJECT

public:
    SpectrumChart();
    ~SpectrumChart();

    void loggingData(int index){if(mainSeries->count() > 0) qDebug() << index << " " << mainSeries->barSets()[0]->at(index);}

public slots:
    void selectROIRegion(int regionStartPoint, int regionEndPoint);
    void deselectROIRegion(int regionStartPoint, int regionEndPoint);
    void setChartWithLevelSeries(std::shared_ptr<LevelSeriesData> levelSeries);
    void setChartWithLevelSeries(LevelSeriesData* levelSeries);
    void setChartWithLevelSeries(const std::vector<int>& rawData);

    void resizeXAxis();
    void resizeYAxis();
    void resizeXYAxis();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    /* 

    */
    QStackedBarSeries *mainSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QLogValueAxis *logAxisY;

    // TODO : LevelSeriesData를 직접받아 관리할 필요성이 존재해보임

    std::vector<int> maxLevelData;

    int seriesCounter = 0;
    std::vector<Qt::GlobalColor> chartColorVector = {Qt::blue, Qt::green, Qt::cyan, Qt::yellow, Qt::gray, Qt::black};
    Qt::GlobalColor roiRegionDefaultColor = DEFAULT_ROI_COLOR;
    std::vector<std::pair<int, int>> roiRegions;

    int startSample = DEFAULT_CHANNEL_START;
    int endSample = DEFAULT_CHANNEL_SIZE;
    int maxMagnitude = 1;
    int threshold = 0;
};


class SpectrumChartView : public QChartView {
    Q_OBJECT

public:
    SpectrumChartView(SpectrumChart *chart);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    SpectrumChart *mainChart;
    QGraphicsLineItem *guideLineItem;

signals:
    void sendClickedXCoordinate(int xIndex);
};

class ROITabWidget : public QWidget {
    Q_OBJECT

public:
    ROITabWidget();

private:
    void setWidgetUI();
    void setInformationFrameUI();
    void setChangeColorButtonUI();
    void setPointCountComboBoxUI();
    void setSignalSlotConnection();

    QVBoxLayout *mainLayout;
    QComboBox *roiListComboBox;
    QComboBox *pointsNumberComboBox;
    QFrame *informationFrame;

    QPushButton *changeRegionsListButton;
    QHBoxLayout *roiRegionControlLayout;

    QPushButton *changeRegionColorButton;
    QHBoxLayout *changeColorButtonLayout;

    // Information Frame Widgets
    QVBoxLayout *informationFrameLayout;

    QLabel *estimatedLineFunctionLabel;
    QLabel *estimatedGaussianFunctionLabel;
    QLabel *estimatedFWHMLabel;
    QLabel *estimatedAreaSizeLabel;

    QString estimatedLineFunctionString = QStringLiteral("Linear Function Calculate by LSM : ");
    QString estimatedGaussianFunctionString = QStringLiteral("Gaussian Distribution Function Calculate by LSM : ");
    QString estimatedFWHMString = QStringLiteral("Calculated FWHM : ");
    QString estimatedAreaSizeString = QStringLiteral("Calculated Area Size : ");

    std::vector<std::pair<int, int>> roiRegions;
    int selectedROIRegionIndex;


signals:
    void sendArgumentsToCalculateGaussianDistribution(int roiIndex, int pointsNumberIndex);
    void openROIDialog();

/* 
    ROITabWidget Slot Functions

    void getROIRegions(std::vector<std::pair<int, int>> roiRegions)
    void setInformationFromDistributionData(std::vector<std::pair<double, double>> data)
*/

public slots:
    void getROIRegions(std::vector<std::pair<int, int>> roiRegions);
    void setInformationFromDistributionData(std::vector<std::pair<double, double>> data);

private slots:
    void calculateGaussianDistributionWithSelectedROIRegion(int index);
};

class InfoTabWidget : public QWidget {
    Q_OBJECT

public:
    InfoTabWidget();

private:
    void setWidgetUI();
    void setBasicInformationFrame();

    QVBoxLayout *mainLayout;
    QFrame *basicInformationFrame;
    QGridLayout *basicInformationFrameLayout;
    QComboBox *channelSizeSelectComboBox;

    QLabel *seriesName;
    QLabel *liveTimeLabel;
    QLabel *realTimeLabel;
    QLabel *deadTimeLabel;
    QLabel *totalCountLabel;

    QStringList channelComboList;

signals:
    void setChannelSize(int channelSize);

public slots:
    void getLevelSeriesData(std::shared_ptr<LevelSeriesData> data);
};

#endif