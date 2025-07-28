#include "widgets.h"

void InfoTabWidget::getLevelSeriesData(std::shared_ptr<LevelSeriesData> data) {
    this->seriesName->setText(data->getName());
    this->liveTimeLabel->setText(QString::number(data->getLiveTime()));
    this->realTimeLabel->setText(QString::number(data->getRealTime()));
    this->deadTimeLabel->setText(QString::number(data->getDeadTime())); 
    this->totalCountLabel->setText(QString::number(getAreaSize(data->getLevelSeries())));

    int defaultChannelSize = 1024;

    for(int i=0 ; defaultChannelSize <= data->getChannelSize(); i++){
        this->channelSizeSelectComboBox->addItem(this->channelComboList[i]);
        defaultChannelSize <<= 1;
    }
}