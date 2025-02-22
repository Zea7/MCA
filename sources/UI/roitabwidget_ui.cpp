#include "widgets.h"

ROITabWidget::ROITabWidget() {
    setWidgetUI();
}

void ROITabWidget::setWidgetUI() {
    this->mainLayout = new QVBoxLayout();
    this->roiListComboBox = new QComboBox();
    this->pointsNumberComboBox = new QComboBox();
    this->informationFrame = new QFrame();

    this->changeRegionsListButton = new QPushButton();
    this->roiRegionControlLayout = new QHBoxLayout();

    this->changeRegionColorButton = new QPushButton();
    this->changeColorButtonLayout = new QHBoxLayout();

    // Labels which shows the function of each widgets
    QLabel *roiRegionListLabel = new QLabel("ROI Region Select");
    QLabel *pointsNumberComboBoxLabel = new QLabel("Set the number of points to use calculate the Gaussian distribution");
    QLabel *setColorLabel = new QLabel("ROI Region Color");
    QLabel *informationLabel = new QLabel("Information of chosen ROI Region");

    this->mainLayout->addLayout(this->roiRegionControlLayout);
    this->mainLayout->addWidget(this->roiListComboBox);

    this->mainLayout->addStretch(1);

    setPointCountComboBoxUI();
    this->mainLayout->addWidget(pointsNumberComboBoxLabel);
    this->mainLayout->addWidget(this->pointsNumberComboBox);

    this->mainLayout->addStretch(1);

    setChangeColorButtonUI();
    this->changeColorButtonLayout->addWidget(setColorLabel);
    this->changeColorButtonLayout->addStretch(1);
    this->changeColorButtonLayout->addWidget(this->changeRegionColorButton);
    this->mainLayout->addLayout(this->changeColorButtonLayout);

    this->mainLayout->addStretch(1);

    setInformationFrameUI();
    this->mainLayout->addWidget(informationLabel);
    this->mainLayout->addWidget(this->informationFrame, 10);

    this->mainLayout->setAlignment(Qt::AlignTop);

    setLayout(this->mainLayout);
}

void ROITabWidget::setPointCountComboBoxUI(){
    this->pointsNumberComboBox->addItem("3 Points");
    this->pointsNumberComboBox->addItem("5 Points");
}

void ROITabWidget::setChangeColorButtonUI(){
    QPalette pal = this->changeRegionColorButton->palette();

    pal.setColor(QPalette::Button, QColor(DEFAULT_ROI_COLOR));
    
    this->changeRegionColorButton->setAutoFillBackground(true);
    this->changeRegionColorButton->setPalette(pal);
    this->changeRegionColorButton->update();
}

void ROITabWidget::setInformationFrameUI(){
    this->estimatedLineFunctionLabel = new QLabel(this->estimatedLineFunctionString);
    this->estimatedGaussianFunctionLabel = new QLabel(this->estimatedGaussianFunctionString);
    this->estimatedFWHMLabel = new QLabel(this->estimatedFWHMString);
    this->estimatedAreaSizeLabel = new QLabel(this->estimatedAreaSizeString);

    this->informationFrameLayout = new QVBoxLayout();

    this->informationFrameLayout->addWidget(this->estimatedLineFunctionLabel);
    this->informationFrameLayout->addWidget(this->estimatedGaussianFunctionLabel);
    this->informationFrameLayout->addWidget(this->estimatedFWHMLabel);
    this->informationFrameLayout->addWidget(this->estimatedAreaSizeLabel);
    
    this->informationFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    this->informationFrame->setLayout(this->informationFrameLayout);
}