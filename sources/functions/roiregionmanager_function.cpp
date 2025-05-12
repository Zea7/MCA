#include "dialogs.h"

void ROIRegionManager::setSignalSlotConnection() {
    /* 
        OKCancelLayout Connection Setting
    */
    QObject::connect(this->OKButton, &QPushButton::clicked, this, &ROIRegionManager::okSignal);
    QObject::connect(this->OKButton, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(this->cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    /* 
        settingButtonsLayout Connection Setting
    */
   QObject::connect(this->addRegionButton, &QPushButton::clicked, this, &ROIRegionManager::addRegion);
   QObject::connect(this->replaceRegionButton, &QPushButton::clicked, this, &ROIRegionManager::replaceRegion);
   QObject::connect(this->removeRegionButton, &QPushButton::clicked, this, &ROIRegionManager::removeRegion);
   QObject::connect(this->removeAllRegionsButton, &QPushButton::clicked, this, &ROIRegionManager::removeAllRegions);
   QObject::connect(this->selectRegionButton, &QPushButton::clicked, this, &ROIRegionManager::selectRegion);
}

void ROIRegionManager::addRegion() {
    int roiStartIndex = this->ROIStartInput->text().toInt();
    int roiEndIndex = this->ROIEndInput->text().toInt();

    auto iter = std::find(roiRegions.begin(), roiRegions.end(), std::make_pair(roiStartIndex, roiEndIndex));
    if(iter == roiRegions.end()) 
        roiRegions.push_back({roiStartIndex, roiEndIndex});

    setShowROIListTable();
}

void ROIRegionManager::removeRegion() {
    QModelIndexList indexes = this->showROIListTable->selectionModel()->selectedRows();

    std::vector<std::pair<int, int>> deleteRegions;
    for(int i=indexes.count() - 1; i >= 0; i--){
        deleteRegions.push_back(roiRegions[indexes.at(i).row()]);
    }

    std::vector<std::pair<int, int>> result;
    std::set_difference(
        roiRegions.begin(), roiRegions.end(),
        deleteRegions.begin(), deleteRegions.end(),
        std::back_inserter(result)
    );
    this->roiRegions = result;
    setShowROIListTable();
}

void ROIRegionManager::removeAllRegions() {
    roiRegions.clear();
    roiRegions.shrink_to_fit();

    setShowROIListTable();
}

void ROIRegionManager::replaceRegion() {
    int roiStartIndex = this->ROIStartInput->text().toInt();
    int roiEndIndex = this->ROIEndInput->text().toInt();

    QModelIndexList indexes = this->showROIListTable->selectionModel()->selectedRows();
    if(indexes.count() > 1) return;

    roiRegions[indexes.at(0).row()] = std::make_pair(roiStartIndex, roiEndIndex);
    setShowROIListTable();
}

void ROIRegionManager::selectRegion(){
    QModelIndexList indexes = this->showROIListTable->selectionModel()->selectedRows();
    if(indexes.count() > 1) return; //TODO
    std::pair<int, int> region = roiRegions[indexes.at(0).row()];

    emit sendShowRegion(region.first, region.second);
}

void ROIRegionManager::okSignal() {
    emit sendROIRegions(roiRegions);
}

void ROIRegionManager::setShowROIListTable() {
    this->showROIListTable->setRowCount(this->roiRegions.size());

    for(int i=0;i < roiRegions.size(); i++){
        this->showROIListTable->setItem(i, 0, new QTableWidgetItem(QString::number(roiRegions[i].first)));
        this->showROIListTable->setItem(i, 1, new QTableWidgetItem(QString::number(roiRegions[i].second)));
    }
}