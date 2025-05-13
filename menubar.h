#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QIcon>

class FileMenu : public QMenu {
    Q_OBJECT
public:
    FileMenu();

private:
    QAction *open;
    QAction *closeAll;
    QAction *save;
    QAction *saveAs;
    QAction *preferences;
    QAction *exit;

signals:
    void openFile();
    void saveFile();

private slots:
    void sendOpenFile();
};

class ViewMenu : public QMenu {
    Q_OBJECT
public:
    ViewMenu();

private:
    QAction *toolBar;
    QAction *statusBar;
    QAction *spectraList;
};

class AcquisitionMenu : public QMenu {
    Q_OBJECT
public:
    AcquisitionMenu();

private:
    bool state = false;
    QAction *acquisitionSetup;
    QAction *changeStartStop;
    QAction *repeatMesureTest;
    QAction *deleteDataResetTime;
    QAction *disconnect;

signals:
    void startDetection();
    void stopDetection();

private slots:
    void changeState();
};

class DisplayMenu : public QMenu {
    Q_OBJECT
public:
    DisplayMenu();

private:
    QAction *scale;
    QAction *calibration;
};
class AnalyzeMenu : public QMenu {
    Q_OBJECT
public:
    AnalyzeMenu();

private:
    QAction *calibrate;
    QAction *peakSearch;
    QAction *defineROI;

signals:
    void openROIDialog();
    void openAutoPeakDialog();
};

class HelpMenu : public QMenu {
    Q_OBJECT
public:
    HelpMenu();

private:
    QAction *help;
};


#endif