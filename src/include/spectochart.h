#ifndef SPECTOCHART_H
#define SPECTOCHART_H

#include <QChart>
#include <QStackedBarSeries>
#include <QValueAxis>
#include <QBarSet>
#include <vector>

#include "utils.h"

class SpectoChart : public QChart {
    Q_OBJECT

public:
    SpectoChart();
    void addData(std::vector<int>& set);

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

    std::vector<int> maxLevels;

    int setCounter = 0;
    std::vector<Qt::GlobalColor> colorVector = {Qt::blue, Qt::green, Qt::red, Qt::cyan};
    int startSample = 0;
    int endSample = 1024;
    int maxMagnitude = 1;
    int threshold = 0;

    bool doingROI = false;

public slots:
    void changeSampleRange(int start, int end);
    void changeMaxMagnitude(int magnitude);
    void resizeXAxis();
    void resizeYAxis();
};

#endif