#include "widgets.h"

SpectrumChartView::SpectrumChartView(SpectrumChart *chart) : QChartView(chart), mainChart(chart){
    setMouseTracking(true);

    this->guideLineItem = chart->scene()->addLine(QLineF(), QPen(Qt::SolidLine));
}

