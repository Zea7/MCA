#ifndef WIDGETS_H
#define WIDGETS_H

#define DEFAULT_ROI_COLOR Qt::red

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <cmath>

#include <QWidget>

#include <QVBoxLayout>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QPalette>
#include <QString>

#include "dialogs.h"

class ROITab : public QWidget {
    Q_OBJECT

public:
    ROITab();

private:
    void setUI();

    void setPointCountComboBox();
    void setChangeColorButton();
    void setInformationFrame();

    QVBoxLayout *mainLayout;

    QHBoxLayout *roiRegionLayout;
    QPushButton *changeROIRegions;
    QComboBox *roiComboBox;

    QComboBox *pointCountComboBox;

    QHBoxLayout *changeColorButtonLayout;
    QPushButton *changeColorButton;

    /* 
        Widgets related with Information Frame


    */
    QFrame *informationFrame;
    QVBoxLayout *frameLayout;
    QLabel *lineFunction;
    QLabel *gaussianFunction;
    QLabel *FWHM;
    QLabel *areaSize;

    QString lineLabel = QStringLiteral("Linear Function Calculate by LSM : ");
    QString gaussianLabel = QStringLiteral("Gaussian Distribution Function Calculate by LSM : ");
    QString fwhmLabel = QStringLiteral("Calculated FWHM : ");
    QString areaLabel = QStringLiteral("Calculated Area Size : ");


    std::vector<std::pair<int, int>> roiRegions;
    std::vector<std::pair<int, std::pair<int, int>>> data;

signals:
    void openROIDialog();
    void sendArgumentsToCalculateGaussian(int roiRegoinIndex, int pointsIndex);

public slots:
    void getROIRegions(std::vector<std::pair<int, int>> roiRegions);
    void getGaussianDistributionData(std::vector<std::pair<double ,double>> data);

private slots:
    void selectedROIRegionIndex(int index);
};


#endif