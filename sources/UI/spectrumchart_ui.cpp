#include "widgets.h"

SpectrumChart::SpectrumChart() {
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
}

SpectrumChart::~SpectrumChart() {

}