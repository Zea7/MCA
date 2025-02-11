#include "dialogs.h"

TempDialog::TempDialog(UartCommunicator *uart) : uart(uart){
    this->log = new QListWidget();
    this->inputLine = new QLineEdit();
    this->mainLayout = new QVBoxLayout();

    this->mainLayout->addWidget(this->log);
    this->mainLayout->addWidget(this->inputLine);
    

    QObject::connect(this->inputLine, &QLineEdit::returnPressed, this, &TempDialog::getEntered);

    this->setLayout(this->mainLayout);
}

void TempDialog::getEntered(){
    QString cmd = this->inputLine->displayText();
    this->inputLine->setText("");
    this->log->addItem(cmd);
    qDebug() << cmd;
    try{
        if(this->uart->sendCommand(cmd.toStdString())){
            QString response = QString::fromUtf8(this->uart->receiveResponse());


            this->log->addItem(response);
        }
    } catch(std::runtime_error err) {

    }
}