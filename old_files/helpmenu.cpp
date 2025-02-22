#include "menubar.h"

HelpMenu::HelpMenu(){
    setTitle("Help");

    this->help = new QAction("&Help");

    this->addAction(this->help);
}