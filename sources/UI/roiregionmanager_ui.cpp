#include "dialogs.h"

ROIRegionManager::ROIRegionManager() {
    setUI();
}

ROIRegionManager::~ROIRegionManager() {
    
}

ROIRegionManager::ROIRegionManager(std::vector<std::pair<int, int>> roiRegions){
    setUI();
}

void ROIRegionManager::setUI() {
    this->setWindowTitle("ROI Regions Manager Dialog");
    this->resize(360, 300);

    this->mainLayout = new QVBoxLayout();
    this->ROIManageLayout = new QGridLayout();
    this->OKCancelLayout = new QHBoxLayout();
    this->settingButtonsLayout = new QVBoxLayout();

    setShowROIListTableUI();
    setSettingButtonsLayoutUI();
    setROIManageLayoutUI();
    setOKCancelLayoutUI();

    this->mainLayout->addLayout(this->ROIManageLayout, 4);
    this->mainLayout->addStretch(1);
    this->mainLayout->addLayout(this->OKCancelLayout, 1);

    this->setLayout(this->mainLayout);
    setSignalSlotConnection();
}

void ROIRegionManager::setShowROIListTableUI() {
    this->showROIListTable = new QTableWidget();

    QStringList list;
    list << "start" << "end";

    this->showROIListTable->setHorizontalHeaderLabels(list);
    this->showROIListTable->setColumnCount(2);
    this->showROIListTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    setShowROIListTable();
}

void ROIRegionManager::setROIManageLayoutUI() {    
    this->ROIStartInput = new QLineEdit();
    this->ROIEndInput = new QLineEdit();

    this->ROIStartInput->setInputMask("000000");
    this->ROIEndInput->setInputMask("000000");

    QLabel *startHintLabel = new QLabel("Start");
    QLabel *endHintLabel = new QLabel("End");

    startHintLabel->setFixedSize(100, 10);
    startHintLabel->setAlignment(Qt::AlignCenter);
    endHintLabel->setFixedSize(100, 10);
    endHintLabel->setAlignment(Qt::AlignCenter);

    this->ROIManageLayout->addWidget(startHintLabel, 0, 0, 1, 1);
    this->ROIManageLayout->addWidget(endHintLabel, 0, 1, 1, 1);
    this->ROIManageLayout->addWidget(this->ROIStartInput, 1, 0, 1, 1);
    this->ROIManageLayout->addWidget(this->ROIEndInput, 1, 1, 1, 1);
    this->ROIManageLayout->addWidget(this->showROIListTable, 2, 0, 3, 2);
    this->ROIManageLayout->addLayout(this->settingButtonsLayout, 0, 2, 5, 1);
}

void ROIRegionManager::setOKCancelLayoutUI() {
    this->OKButton = new QPushButton("OK");
    this->cancelButton = new QPushButton("Cancel");

    this->OKCancelLayout->addWidget(this->OKButton);
    this->OKCancelLayout->addWidget(this->cancelButton);
}

void ROIRegionManager::setSettingButtonsLayoutUI() {
    this->addRegionButton = new QPushButton("Add");
    this->replaceRegionButton = new QPushButton("Replace");
    this->removeRegionButton = new QPushButton("Remove");
    this->removeAllRegionsButton = new QPushButton("Remove All");
    this->selectRegionButton = new QPushButton("Select Region");

    this->settingButtonsLayout->addStretch(1);
    this->settingButtonsLayout->addWidget(addRegionButton);
    this->settingButtonsLayout->addWidget(replaceRegionButton);
    this->settingButtonsLayout->addWidget(removeRegionButton);
    this->settingButtonsLayout->addWidget(removeAllRegionsButton);
    this->settingButtonsLayout->addWidget(selectRegionButton);
    this->settingButtonsLayout->addStretch(2);
}