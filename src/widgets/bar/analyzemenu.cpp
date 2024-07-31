#include "menubar.h"

AnalyzeMenu::AnalyzeMenu(){
    setTitle("Analyze");

    this->calibrate = new QAction("&Calibrate");
    this->peakSearch = new QAction("&Peak Search");
    this->defineROI = new QAction("Define ROI");

    this->addAction(this->calibrate);
    this->addAction(this->peakSearch);
    this->addAction(this->defineROI);

    this->calibrate->setShortcut(Qt::Key_F5);
}