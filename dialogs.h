#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include <QFrame>
#include <QStringList>

#include <vector>
#include <algorithm>

#include "utils.h"
#include "serial.h"

class DefineROIDialog : public QDialog {
    Q_OBJECT
public:
    DefineROIDialog(std::vector<std::pair<int, int>> roiRegions);

private:
    /* 
        Layouts

        outerLayout     :   가장 바깥쪽 레이아웃
        innerLayout     :   위쪽 상세내역이 존재하는 레이아웃
        okCancelLayout  :   OK, Cancel 버튼이 있는 레이아웃
        buttonsLayout   :   각종 버튼이 존재하는 레이아웃
        showROILayout   :   ROI 설정과 관련된 위젯이 존재하는 레이아웃    
    */
    QGridLayout *outerLayout;
    QGridLayout *innerLayout;
    QHBoxLayout *okCancelLayout;
    QVBoxLayout *buttonsLayout;
    QGridLayout *showROILayout;


    QLineEdit *start;
    QLineEdit *end;
    QTableWidget *showROIWidget;


    QPushButton *add;
    QPushButton *replace;
    QPushButton *remove;
    QPushButton *removeAll;
    QPushButton *showRegion;

    QPushButton *ok;
    QPushButton *cancel;

    std::vector<std::pair<int, int>> roiRegions;

    void setUI();
    void setROITable();

signals:
    void sendROIRegions(std::vector<std::pair<int, int>> roiRegions);
    void sendShowRegion(int start, int end);

private slots:
    void addROIRegion();
    void deleteROIRegion();
    void replaceRegion();
    void deleteAllRegions();
    void okSignal();
    void showROIRegion();
};

class AutoPeakDialog : public QDialog {
    Q_OBJECT

public:
    AutoPeakDialog(int start, int end);

signals:
    void doAutoPeakSearch(int start, int end, int leftPoint, int rightPoint);

private:
    /* 
        Layouts

        outerLayout     :   가장 바깥쪽 레이아웃
        innerLayout     :   위쪽 상세내역이 존재하는 레이아웃
        okCancelLayout  :   OK, Cancel 버튼이 있는 레이아웃
        buttonsLayout   :   각종 버튼이 존재하는 레이아웃
        peakConfigLayout   :   ROI 설정과 관련된 위젯이 존재하는 레이아웃    
    */
    QGridLayout *outerLayout;
    QGridLayout *innerLayout;
    QHBoxLayout *okCancelLayout;
    QVBoxLayout *buttonsLayout;
    QGridLayout *peakConfigLayout;

    QFrame *outerFrame;

    QLineEdit *startRange;
    QLineEdit *endRange;
    QLineEdit *setLeft;
    QLineEdit *setRight;

    QPushButton *searchRange;
    QPushButton *searchAll;
    QPushButton *addToROI;
    QPushButton *replaceROIList;

    QPushButton *ok;
    QPushButton *cancel;

    void setUI();

    int startSample;
    int endSample;
    int leftPoint = 0;
    int rightPoint = 0;

private slots:
    void sendVariables();
};

class SerialSettingDialog : public QDialog {
    Q_OBJECT

public:
    SerialSettingDialog();
    ~SerialSettingDialog();

signals:
    void setSerialPort(QString port);

private:
    void setUI();

    void setComboBoxes();

    QHBoxLayout *mainLayout;
    QVBoxLayout *outerLayout;

    QComboBox *typeSelectComboBox;
    QComboBox *channelCountSelectComboBox;
    QComboBox *portBox;
    QLineEdit *speedEnterLine;
    QLineEdit *realTimeEnterLine;
    QPushButton *selectBackgroundSubstractFile;
    QPushButton *okButton;
    QPushButton *cancelButton;

    QGridLayout *serialSettingLayout;
    QGridLayout *experimentalSettingLayout;
    QHBoxLayout *okCancelLayout;


private slots:
    std::string getUARTPort();
    void sendSerialPort();
};


#endif