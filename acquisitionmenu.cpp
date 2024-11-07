#include "menubar.h"

AcquisitionMenu::AcquisitionMenu(){
    setTitle("Acquisition");

    this->acquisitionSetup = new QAction("&Acquisition Setup");
    this->changeStartStop = new QAction("&Start");
    this->repeatMesureTest = new QAction("Repeat Measure Test");
    this->deleteDataResetTime = new QAction("Delete Data and Reset Time");
    this->disconnect = new QAction("Disconnect");

    this->addAction(this->acquisitionSetup);
    this->addAction(this->changeStartStop);
    this->addAction(this->repeatMesureTest);
    this->addAction(this->deleteDataResetTime);
    this->addAction(this->disconnect);

    this->acquisitionSetup->setShortcut(Qt::Key_F9);
    this->changeStartStop->setShortcut(Qt::Key_F3);
    this->disconnect->setShortcut(Qt::Key_F2);

    QObject::connect(this->changeStartStop, &QAction::triggered, this, &AcquisitionMenu::changeState);
}

void AcquisitionMenu::changeState(){
    if(!state) {
        this->changeStartStop->setText("&End");
        emit startDetection();
    }
    else {
        this->changeStartStop->setText("&Start");
        emit stopDetection();
    }

    state = !state;
}