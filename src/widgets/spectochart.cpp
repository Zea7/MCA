#include "spectochart.h"

SpectoChart::SpectoChart(){
    this->mainSeries = new QStackedBarSeries();

    this->addSeries(this->mainSeries);

    this->axisX = new QValueAxis();
    this->axisY = new QValueAxis();

    axisX->setRange(startSample, endSample);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");

    this->axisY = new QValueAxis;
    axisY->setRange(0, maxMagnitude);
    axisY->setTitleText("Level");

    this->addAxis(axisX, Qt::AlignBottom);
    this->mainSeries->attachAxis(axisX);
    this->addAxis(axisY, Qt::AlignLeft);
    this->mainSeries->attachAxis(axisY);
    this->legend()->hide();

    this->mainSeries->setBarWidth(1.0);
}

void SpectoChart::addData(std::vector<int>& set){
    int setSize = set.size();
    int seriesSize = this->maxLevels.size();

    QBarSet *newSet = new QBarSet("MCA Data");
    
    if(setCounter){
        for(int i=0;i<setSize;i++){
            int value = set[i];
            if(seriesSize > i)
                value = (set[i] - maxLevels[i] > 0 ? set[i] - maxLevels[i] : 0);
            *newSet << value;
        }
        if(setSize > seriesSize){
            for(int i=0;i<seriesSize;i++){
                if(maxLevels[i] < set[i]) maxLevels[i] = set[i];
            }
            for(int i=seriesSize;i<setSize;i++){
                maxLevels.push_back(set[i]);
            }
        }
        else{
            for(int i=0;i<setSize;i++){
                if(maxLevels[i] < set[i]) maxLevels[i] = set[i];
            }
        }

        

    } else{
        for(int i=0;i<setSize;i++){
            *newSet << set[i];
            maxLevels.push_back(set[i]);
        }

    }

    newSet->setColor(colorVector[setCounter]);
    newSet->setBorderColor(Qt::transparent);
    this->mainSeries->append(newSet);
    this->setCounter++;
}

void SpectoChart::mousePressEvent(QGraphicsSceneMouseEvent *event){
    
}

/* 
    public slots

    Needs for using in mainwindow class.
    To use in other class, these functions have to be public.

    changeSampleRange(int start, int end)   :   change X Axis range to [start, end]
    resizeXAxis()   :   resize X Axis range of chart
    resizeYAxis()   :   resize Y Axis range of chart

*/

void SpectoChart::changeSampleRange(int start, int end){
    // qDebug() << end;
    this->startSample = start;
    this->endSample = end;
}

void SpectoChart::changeMaxMagnitude(int magnitude){
    // qDebug() << magnitude;
    if(magnitude > maxMagnitude) this->maxMagnitude = magnitude;
}

void SpectoChart::resizeXAxis(){
    this->axisX->setRange(startSample, endSample);
    this->axisX->setLabelFormat("%g");
    this->axisX->setTitleText("Samples");
}

void SpectoChart::resizeYAxis(){
    this->axisY->setRange(0, getClosestInt(this->maxMagnitude));
    this->axisY->setTitleText("Level");
}

/*
    private slots

*/
