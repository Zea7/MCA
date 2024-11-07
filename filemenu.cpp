#include "menubar.h"

FileMenu::FileMenu(){
    setTitle("File");

    this->open = new QAction("&Open");
    this->closeAll = new QAction("Close All");
    this->save = new QAction("&Save");
    this->saveAs = new QAction("Save &As");
    this->preferences = new QAction("Preferences");
    this->exit = new QAction("Exit");

    this->addAction(this->open);
    this->addAction(this->closeAll);
    this->addAction(this->save);
    this->addAction(this->saveAs);
    this->addAction(this->preferences);
    this->addAction(this->exit);


    this->open->setShortcut(Qt::CTRL | Qt::Key_O);
    this->open->setIcon(QIcon(":/icons/open-folder.png"));
    QObject::connect(this->open, SIGNAL(triggered()), this, SIGNAL(openFile()));

    this->saveAs->setShortcut(Qt::CTRL | Qt::Key_S);
    this->saveAs->setIcon(QIcon(":/icons/save.png"));
    QObject::connect(this->save, SIGNAL(triggered()), this, SIGNAL(saveFile()));
}

/* 
    Signals. FileMenu에서 시그널을 발생시켜 mainwindow 등 다른 class에서 처리하기 위함.
*/
void FileMenu::sendOpenFile(){
    emit openFile();
}