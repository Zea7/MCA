#include "toolbar.h"

ToolBar::ToolBar(){
    this->setFloatable(false);
    this->setMovable(false);

    this->open = new QAction(QIcon(":/icons/open-folder.png"), "Open");
    this->save = new QAction(QIcon(":/icons/save.png"), "Save");
    this->resizeHorizontal = new QAction(QIcon(":/icons/resize-horizontal.png"), "Auto Resize Horizontal");
    this->resizeVertical = new QAction(QIcon(":/icons/resize-vertical.png"), "Auto Resize Vertical");
    this->roiDialog = new QAction(QIcon(":/icons/region-select.png"), "Define ROI");
    this->magnification = new QAction(QIcon(":/icons/magnification.png"), "Show Specific Region");

    this->addAction(this->open);
    this->addAction(this->save);
    this->addAction(this->resizeHorizontal);
    this->addAction(this->resizeVertical);

    this->addAction(this->roiDialog);
    this->insertSeparator(this->roiDialog);
    this->addAction(this->magnification);

    QObject::connect(this->open, SIGNAL(triggered()), this, SIGNAL(openFile()));
    QObject::connect(this->save, SIGNAL(triggered()), this, SIGNAL(saveFile()));
    QObject::connect(this->resizeHorizontal, SIGNAL(triggered()), this, SIGNAL(resizeXAxis()));
    QObject::connect(this->resizeVertical, SIGNAL(triggered()), this, SIGNAL(resizeYAxis()));
    QObject::connect(this->roiDialog, SIGNAL(triggered()), this, SIGNAL(openROIDialog()));
    QObject::connect(this->magnification, SIGNAL(triggered()), this, SIGNAL(showSpecificRegion()));
}