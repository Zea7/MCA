#include "widgets.h"

void SpectrumChartView::mousePressEvent(QMouseEvent *event) {
    QPointF mousePos = event->pos();
    QRectF plotArea = chart()->plotArea();

    qreal topY = plotArea.top();     // 가장 위쪽
    qreal ratio = (mousePos.x() - plotArea.left()) / plotArea.width();  // x는 데이터 기준

    QValueAxis *xAxis = qobject_cast<QValueAxis*>(chart()->axisX());
    qreal x = xAxis->min() + ratio*(xAxis->max());
    int index = qRound(x);

    if (index >= 0 && index < qobject_cast<QValueAxis*>(chart()->axisX())->max()){
        this->guideLineItem->setLine(QLineF(QPointF(mousePos.x(), topY), QPointF(mousePos.x(), plotArea.bottom())));
        
        this->mainChart->loggingData(index);
    }
    
    emit sendClickedXCoordinate(index);
    QChartView::mousePressEvent(event);
}