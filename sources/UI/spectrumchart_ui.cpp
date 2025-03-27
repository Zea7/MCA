#include "widgets.h"

SpectrumChart::SpectrumChart() {

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

