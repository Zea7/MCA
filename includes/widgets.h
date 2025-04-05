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

#include "types.h"

// #include "dialogs.h"

class SpectrumChart : public QChart {
    Q_OBJECT

public:
    SpectrumChart();
    ~SpectrumChart();

public slots:
    void selectROIRegion(int regionStartPoint, int regionEndPoint);
    void deselectROIRegion(int regionStartPoint, int regionEndPoint);
    void setChartWithLevelSeries(LevelSeriesData &levelSeries);

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

class ROITabWidget : public QWidget {
    Q_OBJECT

public:
    ROITabWidget();

private:
    void setWidgetUI();
    void setInformationFrameUI();
    void setChangeColorButtonUI();
    void setPointCountComboBoxUI();

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

public slots:
    void getROIRegions(std::vector<std::pair<int, int>> roiRegions);
};

#endif