#include "menubar.h"

ViewMenu::ViewMenu(){
    setTitle("View");

    this->toolBar = new QAction("&Toolbar");
    this->statusBar = new QAction("&Status Bar");
    this->spectraList = new QAction("Spectra &List");

    this->addAction(this->toolBar);
    this->addAction(this->statusBar);
    this->addSeparator();
    this->addAction(this->spectraList);

    this->spectraList->setShortcut(Qt::Key_F6);

    this->toolBar->setCheckable(true);
    this->statusBar->setCheckable(true);
}