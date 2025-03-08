#include "mainwindow.h"
#include "dialogs.h"

void MainWindow::dialogTest() {
    SpectrumListManager *dialog = new SpectrumListManager();
    dialog->setModal(true);

    dialog->show();
}
