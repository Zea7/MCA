#include "toolbar.h"

ToolBar::ToolBar(){
    this->setFloatable(false);
    this->setMovable(false);

    this->open = new QAction(QIcon(":/icons/open-folder.png"), "Open");
    this->save = new QAction(QIcon(":/icons/save.png"), "Save");
    this->resizeHorizontal = new QAction(QIcon(":/icons/resize-horizontal.png"), "Auto Resize Horizontal");
    this->resizeVertical = new QAction(QIcon(":/icons/resize-vertical.png"), "Auto Resize Vertical");

    this->addAction(this->open);
    this->addAction(this->save);
    this->addAction(this->resizeHorizontal);
    this->addAction(this->resizeVertical);

    QObject::connect(this->open, SIGNAL(triggered()), this, SIGNAL(openFile()));
    QObject::connect(this->resizeHorizontal, SIGNAL(triggered()), this, SIGNAL(resizeXAxis()));
    QObject::connect(this->resizeVertical, SIGNAL(triggered()), this, SIGNAL(resizeYAxis()));
}