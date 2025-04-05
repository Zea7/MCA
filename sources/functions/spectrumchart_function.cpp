#include "widgets.h"

void SpectrumChart::mousePressEvent(QGraphicsSceneMouseEvent *event){

}

void SpectrumChart::resizeXAxis() {
    this->axisX->setRange(startSample, endSample);
    this->axisX->setTitleText("Samples");
}

void SpectrumChart::resizeYAxis(){
    this->axisY->setRange(0, this->maxMagnitude + 1000);
    this->axisY->setTitleText("Level");
}

void SpectrumChart::resizeXYAxis() {
    this->resizeXAxis();
    this->resizeYAxis();
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

void SpectrumChart::setChartWithLevelSeries(LevelSeriesData &levelSeries){
    QBarSet *newSet = new QBarSet("MCA DATA");

    std::vector<int> data = levelSeries.getLevelSeries();

    this->endSample = levelSeries.getChannelSize();

    for(int i=0;i<levelSeries.getChannelSize();i++){
        *newSet << data[i];

        if(data[i] > maxMagnitude) maxMagnitude = data[i];
    }

    newSet->setColor(chartColorVector[0]);
    newSet->setBorderColor(Qt::transparent);
    newSet->setSelectedColor(this->roiRegionDefaultColor);
    if(roiRegions.size()){
        for(auto i:roiRegions){
            QList<int> idx;
            for(int j=i.first;j<=i.second;j++){
                idx << j;
            }
            newSet->toggleSelection(idx);
        }
    }
    this->mainSeries->clear();
    this->mainSeries->append(newSet);
    this->seriesCounter++;

    this->resizeXYAxis();
}