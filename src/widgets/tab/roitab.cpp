#include "widgets.h"


ROITab::ROITab(){
    setUI();
}

void ROITab::setUI(){
    this->mainLayout = new QVBoxLayout();
    this->roiComboBox = new QComboBox();
    this->pointCountComboBox = new QComboBox();

    this->mainLayout->addWidget(this->roiComboBox);
    this->mainLayout->addWidget(this->pointCountComboBox);

    setPointCountComboBox();

    setLayout(this->mainLayout);
}

void ROITab::setPointCountComboBox(){
    this->pointCountComboBox->addItem("3칸");
    this->pointCountComboBox->addItem("5칸");
}

/* public slots */

void ROITab::getROIRegions(std::vector<std::pair<int, int>> roiRegions){
    this->roiRegions = roiRegions;
    this->roiComboBox->clear();

    for(auto region : roiRegions){
        QString item = QString::number(region.first) + QString(", ") + QString::number(region.second);

        this->roiComboBox->addItem(item);
    }
}