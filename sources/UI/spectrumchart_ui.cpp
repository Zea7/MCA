#include "widgets.h"

SpectrumChart::SpectrumChart() {
    this->mainSeries = new QStackedBarSeries();

    this->addSeries(this->mainSeries);

    this->axisX = new QValueAxis();
    this->axisY = new QValueAxis();
    this->logAxisY = new QLogValueAxis();

    axisX->setRange(startSample, endSample);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");

    axisY->setRange(0, maxMagnitude);
    axisY->setTitleText("Level");
    axisY->setLabelFormat("%g");

    this->logAxisY->setBase(10);
    this->logAxisY->setMinorTickCount(10);
    this->logAxisY->setTitleText("Level");
    this->logAxisY->setLabelFormat("%g");

    this->addAxis(axisX, Qt::AlignBottom);
    this->mainSeries->attachAxis(axisX);
    this->addAxis(axisY, Qt::AlignLeft);
    this->mainSeries->attachAxis(axisY);
    this->legend()->hide();

    this->mainSeries->setBarWidth(1.0);
}

SpectrumChart::~SpectrumChart() {

}

void SpectrumChart::mousePressEvent(QGraphicsSceneMouseEvent *event){

}

void SpectrumChart::selectROIRegion(int regionStartPoint, int regionEndPoint){
    if(std::find(roiRegions.begin(), roiRegions.end(), std::make_pair(regionStartPoint, regionEndPoint)) == roiRegions.end())
        roiRegions.push_back({regionStartPoint, regionEndPoint});
    else    
        qDebug() << "ROI Region is already selected!";

    QList<QBarSet *> setList = this->mainSeries->barSets();
    QList<int> idx;

    for(int i = regionStartPoint; i <= regionEndPoint; i++){
        idx << i;
    }

    for(auto *set : setList){
        set->selectBars(idx);
    }
}

void SpectrumChart::deselectROIRegion(int regionStartPoint, int regionEndPoint) {
    auto iter = std::find(roiRegions.begin(), roiRegions.end(), std::make_pair(regionStartPoint, regionEndPoint));

    if(iter == roiRegions.end())
        qDebug() << "No ROI Region has same range";
    else    
        roiRegions.erase(iter);

    QList<QBarSet *> setList = this->mainSeries->barSets();
    QList<int> idx;

    // To Be specified
}

