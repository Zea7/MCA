#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>

#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>

#include <QMenuBar>
#include <QStatusBar>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    /*  
        UI를 세팅하는 함수들. 

        setUI()         :   메인 위젯을 설정해주고 기본적인 창의 크기를 결정하는 함수.
        setMenuBar()    :   메뉴바의 형태를 결정짓는 함수.
        setChartView()   :   그래프를 보여줄 chart의 형태를 결정하는 함수.
    */


    void setUI();

    void setMenuBar();

    void setChartView();


    QWidget *mainWidget;
    QGridLayout *mainLayout;

    QHBoxLayout *bottomRemoteControl;
    QVBoxLayout *sidePannel;

    QChart *mainChart;
    QLineSeries *mainSeries;
    QChartView *mainChartView;

    int sampleCount = 2048;
    int maxMagnitude = 1;
};

#endif