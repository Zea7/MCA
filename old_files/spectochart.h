#ifndef SPECTOCHART_H
#define SPECTOCHART_H

#include <QChart>
#include <QStackedBarSeries>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QBarSet>
#include <vector>
#include <algorithm>

#include "utils.h"

class SpectoChart : public QChart {
    Q_OBJECT

public:
    SpectoChart();
    void addData(std::vector<int>& set);
    void setData(std::vector<int>& set);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    /*  
        SpectoChart's private members

        setCounter  :   Counter for QBarSets which are in mainSeries. Use for coloring.
    */
    QStackedBarSeries *mainSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QLogValueAxis *logAxisY;

    std::vector<int> maxLevels;

    int setCounter = 0;
    std::vector<Qt::GlobalColor> colorVector = {Qt::blue, Qt::green, Qt::cyan, Qt::yellow, Qt::gray, Qt::black};
    Qt::GlobalColor selectedColor = Qt::red;
    std::vector<std::pair<int, int>> ROIRegions;
    int startSample = 0;
    int endSample = 1024;
    int maxMagnitude = 1;
    int threshold = 0;

    bool doingROI = false;

    void selectROIRegion(int start, int end);
    void deselectROIRegion(int start, int end);

signals:
    void GetHoveredData(bool status, int index, int value);

public slots:
    void changeSampleRange(int start, int end);
    void changeMaxMagnitude(int magnitude);
    void resizeXAxis();
    void resizeYAxis();

    void ROIRegionChange(std::vector<std::pair<int, int>> regions);
    
    void changeYAxisScale(bool scale);

private slots:
    // Interact with mouse event
    void MouseHovered(bool status, int index, QBarSet *barset);
};

#endif