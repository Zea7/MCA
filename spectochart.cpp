#include "spectochart.h"

SpectoChart::SpectoChart(){
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

    QObject::connect(this->mainSeries, SIGNAL(hovered(bool, int, QBarSet*)), this, SLOT(MouseHovered(bool, int, QBarSet*)));
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
    newSet->setSelectedColor(this->selectedColor);
    for(auto i:ROIRegions){
        QList<int> idx;
        for(int j=i.first;j<=i.second;j++){
            idx << j;
        }
        newSet->toggleSelection(idx);
    }
    this->mainSeries->append(newSet);
    this->setCounter++;
    
}

void SpectoChart::setData(std::vector<int> &set){
    QBarSet *newSet = new QBarSet("MCA DATA");
    for(int i=0;i<set.size();i++){    
        *newSet << set[i];
        if(set[i] > maxMagnitude) maxMagnitude = set[i];
    }

    newSet->setColor(colorVector[0]);
    newSet->setBorderColor(Qt::transparent);
    newSet->setSelectedColor(this->selectedColor);
    if(ROIRegions.size()){
        for(auto i:ROIRegions){
            QList<int> idx;
            for(int j=i.first;j<=i.second;j++){
                idx << j;
            }
            newSet->toggleSelection(idx);
        }
    }
    this->mainSeries->clear();
    this->mainSeries->append(newSet);
    this->setCounter++;
    this->resizeXAxis();
    this->resizeYAxis();
}

void SpectoChart::mousePressEvent(QGraphicsSceneMouseEvent *event){
    
}

void SpectoChart::selectROIRegion(int start, int end){
    if(std::find(ROIRegions.begin(), ROIRegions.end(), std::make_pair(start, end)) == ROIRegions.end()){
        ROIRegions.push_back({start, end});
    } else {
        qDebug() << "ROI Region is already selected!";
    }
    QList<QBarSet *> sets = this->mainSeries->barSets();
    QList<int> idx;


    for(auto *i : sets){
        idx.clear();
        QList<int> selected = i->selectedBars();
        for(int i=start; i<=end;i++){
            if(!selected.contains(i)) idx<<i;
        }
        i->toggleSelection(idx);
    }

}

void SpectoChart::deselectROIRegion(int start, int end){
    auto iter = std::find(ROIRegions.begin(), ROIRegions.end(), std::make_pair(start, end));
    if(iter == ROIRegions.end()) {
        qDebug() << "ROI Regions have same range do not exist.";
    }
    else{
        ROIRegions.erase(iter);
    }
    QList<QBarSet *> sets = this->mainSeries->barSets();
    QList<int> idx;

    for(auto *i : sets){
        idx.clear();
        QList<int> selected = i->selectedBars();
        for(int i=start; i<=end;i++){
            if(selected.contains(i)) idx<<i;
        }
        i->toggleSelection(idx);
    }
}
/* 
    public slots

    Needs for using in mainwindow class.
    To use in other class, these functions have to be public.

    changeSampleRange(int start, int end)   :   change X Axis range to [start, end]
    changeMaxMagnitude(int magnitude)       :   change Max Magnitude to change Y Axis range to [0, maxMagnitude]
    resizeXAxis()   :   resize X Axis range of chart
    resizeYAxis()   :   resize Y Axis range of chart
    selectROIRegion()   :   make barsets selected from [start, end]

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
    this->axisX->setTitleText("Samples");
}

void SpectoChart::resizeYAxis(){
    this->axisY->setRange(0, getClosestInt(this->maxMagnitude));
    this->axisY->setTitleText("Level");
}

void SpectoChart::ROIRegionChange(std::vector<std::pair<int, int>> regions){
    for(auto i:regions){
        if(std::find(ROIRegions.begin(), ROIRegions.end(), i) == ROIRegions.end()) selectROIRegion(i.first, i.second);
    }
    for(auto i:this->ROIRegions){
        if(std::find(regions.begin(), regions.end(), i) == regions.end()) deselectROIRegion(i.first, i.second);
    }
}

void SpectoChart::changeYAxisScale(bool scale){
    QAbstractAxis *removeaxis, *insertAxis;
    if(scale) {
        removeaxis = this->axisY;
        insertAxis = this->logAxisY;
    } else {
        removeaxis = this->logAxisY;
        insertAxis = this->axisY;
    }

    if(this->axes(Qt::Vertical).contains(removeaxis)) this->removeAxis(removeaxis);
    this->addAxis(insertAxis, Qt::AlignLeft);

    for(auto series : this->series()){
        if(series->attachedAxes().contains(removeaxis))
            series->detachAxis(removeaxis);
        series->attachAxis(insertAxis);
    }
}

/*
    private slots

*/
void SpectoChart::MouseHovered(bool status, int index, QBarSet *barset){
    emit GetHoveredData(status, index, int(barset->at(index)));
}