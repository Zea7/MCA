#include "widgets.h"


ROITab::ROITab(){
    setUI();
}

void ROITab::setUI(){
    this->mainLayout = new QVBoxLayout();
    this->roiComboBox = new QComboBox();
    this->pointCountComboBox = new QComboBox();
    this->changeColorButton = new QPushButton();
    this->informationFrame = new QFrame();

    this->changeROIRegions = new QPushButton("Edit");
    this->roiRegionLayout = new QHBoxLayout();

    // Layout Label
    QLabel *roiRegionLabel = new QLabel("ROI Region Select");
    QLabel *settingPoints = new QLabel("Set the number of points to use calculate the Gaussian Distribution");
    QLabel *setColor = new QLabel("ROI Region Color");
    QLabel *information = new QLabel("Information of chosen ROI Region");

    this->changeColorButtonLayout = new QHBoxLayout();

    this->roiRegionLayout->addWidget(roiRegionLabel);
    this->roiRegionLayout->addStretch(1);
    this->roiRegionLayout->addWidget(this->changeROIRegions);
    
    this->mainLayout->addLayout(this->roiRegionLayout);
    this->mainLayout->addWidget(this->roiComboBox);

    this->mainLayout->addStretch(1);

    this->mainLayout->addWidget(settingPoints);
    this->mainLayout->addWidget(this->pointCountComboBox);

    this->mainLayout->addStretch(1);

    this->changeColorButtonLayout->addWidget(setColor);
    this->changeColorButtonLayout->addStretch(1);
    this->changeColorButtonLayout->addWidget(this->changeColorButton);
    this->mainLayout->addLayout(this->changeColorButtonLayout);

    this->mainLayout->addStretch(1);

    this->mainLayout->addWidget(information);
    this->mainLayout->addWidget(this->informationFrame, 10);

    this->mainLayout->setAlignment(Qt::AlignTop);

    setPointCountComboBox();
    setInformationFrame();
    setChangeColorButton();

    setLayout(this->mainLayout);

    QObject::connect(this->changeROIRegions, SIGNAL(clicked()), this, SIGNAL(openROIDialog()));
    QObject::connect(this->roiComboBox, &QComboBox::activated, this, &ROITab::selectedROIRegionIndex);
    QObject::connect(this->pointCountComboBox, &QComboBox::activated, this, &ROITab::selectedROIRegionIndex);
}

void ROITab::setPointCountComboBox(){
    this->pointCountComboBox->addItem("3 Points");
    this->pointCountComboBox->addItem("5 Points");
}

void ROITab::setChangeColorButton(){
    QPalette pal = this->changeColorButton->palette();

    pal.setColor(QPalette::Button, QColor(DEFAULT_ROI_COLOR));
    this->changeColorButton->setAutoFillBackground(true);
    this->changeColorButton->setPalette(pal);
    this->changeColorButton->update();
}

void ROITab::setInformationFrame(){
    this->lineFunction = new QLabel(this->lineLabel);
    this->gaussianFunction = new QLabel(this->gaussianLabel);
    this->FWHM = new QLabel(this->fwhmLabel);
    this->areaSize = new QLabel(this->areaLabel);


    this->frameLayout = new QVBoxLayout();

    this->frameLayout->addWidget(this->lineFunction);
    this->frameLayout->addWidget(this->gaussianFunction);
    this->frameLayout->addWidget(this->FWHM);
    this->frameLayout->addWidget(this->areaSize);

    this->informationFrame->setFrameStyle(QFrame::Box || QFrame::Raised);
    this->informationFrame->setLayout(this->frameLayout);
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

void ROITab::getGaussianDistributionData(std::vector<std::pair<double, double>> data){
    assert(data.size() == 3);

    std::pair<double, double> linearFunc = data[0];
    std::pair<double, double> gaussian = data[1];
    std::pair<double, double> areaSizes = data[2];

    double fwhm = 2.3548200 * gaussian.second;
    double a = 1 / sqrt(2 * M_PI * gaussian.second * gaussian.second);
    double b = 2 * gaussian.second * gaussian.second;

    QString linear = QStringLiteral("y = ") + QString::number(linearFunc.first) + QStringLiteral(" x + ") + QString::number(linearFunc.second);
    
    QString gaussianFunc = QStringLiteral("y = ") + QString::number(a) + QStringLiteral(" * exp(-(x - ") + QString::number(gaussian.first) + QStringLiteral(")^2 / ") + QString::number(b) + QStringLiteral(")");

    this->lineFunction->setText(lineLabel + QString("\n") + linear);
    this->gaussianFunction->setText(gaussianLabel + QString("\n") +  gaussianFunc);
    this->FWHM->setText(fwhmLabel +  QString("\n") + QString::number(fwhm));
    this->areaSize->setText(areaLabel +  QString("\n") + QString::number(areaSizes.first));
}

/* private slots */

void ROITab::selectedROIRegionIndex(int index){
    emit sendArgumentsToCalculateGaussian(this->roiComboBox->currentIndex(), this->pointCountComboBox->currentIndex());
}