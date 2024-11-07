#include "menubar.h"

DisplayMenu::DisplayMenu(){
    setTitle("Display");

    this->scale = new QAction("&Scale");
    this->calibration = new QAction("Calibration");

    this->calibration->setCheckable(true);

    this->addAction(this->scale);
    this->addAction(this->calibration);
}