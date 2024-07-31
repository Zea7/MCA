#include "menubar.h"

DPPMenu::DPPMenu(){
    setTitle("DPP");

    this->openStartDialog = new QAction("Open Start Dialog");
    
    this->addAction(this->openStartDialog);
}