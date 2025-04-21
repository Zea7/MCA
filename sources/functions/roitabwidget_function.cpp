#include "widgets.h"

void ROITabWidget::setSignalSlotConnection() {
    QObject::connect(this->changeRegionsListButton, &QPushButton::clicked, this, &ROITabWidget::openROIDialog);
    QObject::connect(this->roiListComboBox, &QComboBox::activated, this, &ROITabWidget::calculateGaussianDistributionWithSelectedROIRegion);
    QObject::connect(this->pointsNumberComboBox, &QComboBox::activated, this, &ROITabWidget::calculateGaussianDistributionWithSelectedROIRegion);
}

/* 
    public slots

    void getROIRegions(std::vector<std::pair<int, int>> roiRegions)
    
*/

void ROITabWidget::getROIRegions(std::vector<std::pair<int, int>> roiRegions){
    this->roiRegions = roiRegions;
    this->roiListComboBox->clear();

    for(auto region : roiRegions){
        QString item = QString::number(region.first) + QString(", ") + QString::number(region.second);

        this->roiListComboBox->addItem(item);
    }
}

void ROITabWidget::setInformationFromDistributionData(std::vector<std::pair<double, double>> data){
    assert(data.size() == 3);

    std::pair<double, double> linearFunction = data[0];
    std::pair<double, double> gaussianFunction = data[1];
    std::pair<double, double> areaSizes = data[2];

    double fwhm = 2.35482 * gaussianFunction.second;
    double a = 1 / sqrt(2 * M_PI * gaussianFunction.second * gaussianFunction.second);
    double b = 2 * gaussianFunction.second * gaussianFunction.second;

    QString linearInformation = QStringLiteral("y = ") + QString::number(linearFunction.first) + QStringLiteral(" x + ") + QString::number(linearFunction.second);
    QString gaussianInformation = QStringLiteral("y = ") + QString::number(a) + QStringLiteral(" * exp(-(x - ") + QString::number(gaussianFunction.first) + QStringLiteral(")^2 / ") + QString::number(b) + QStringLiteral(")");

    this->estimatedLineFunctionLabel->setText(this->estimatedLineFunctionString + QString("\n") + linearInformation);
    this->estimatedGaussianFunctionLabel->setText(this->estimatedGaussianFunctionString + QString("\n") + gaussianInformation);
    this->estimatedFWHMLabel->setText(this->estimatedFWHMString + QString("\n") + QString::number(fwhm));
    this->estimatedAreaSizeLabel->setText(this->estimatedAreaSizeString + QString("\n") + QString::number(areaSizes.first));
}

void ROITabWidget::calculateGaussianDistributionWithSelectedROIRegion(int index) {
    emit sendArgumentsToCalculateGaussianDistribution(this->roiListComboBox->currentIndex(), this->pointsNumberComboBox->currentIndex());
}