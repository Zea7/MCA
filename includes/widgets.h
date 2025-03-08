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

#include <QChart>

// #include "dialogs.h"

class SpectrumChart : public QChart {
    Q_OBJECT

public:
    SpectrumChart();
    ~SpectrumChart();

    
};

class ROITabWidget : public QWidget {
    Q_OBJECT

public:
    ROITabWidget();

private:
    void setWidgetUI();
    void setInformationFrameUI();
    void setChangeColorButtonUI();
    void setPointCountComboBoxUI();

    QVBoxLayout *mainLayout;
    QComboBox *roiListComboBox;
    QComboBox *pointsNumberComboBox;
    QFrame *informationFrame;

    QPushButton *changeRegionsListButton;
    QHBoxLayout *roiRegionControlLayout;

    QPushButton *changeRegionColorButton;
    QHBoxLayout *changeColorButtonLayout;

    // Information Frame Widgets
    QVBoxLayout *informationFrameLayout;

    QLabel *estimatedLineFunctionLabel;
    QLabel *estimatedGaussianFunctionLabel;
    QLabel *estimatedFWHMLabel;
    QLabel *estimatedAreaSizeLabel;

    QString estimatedLineFunctionString = QStringLiteral("Linear Function Calculate by LSM : ");
    QString estimatedGaussianFunctionString = QStringLiteral("Gaussian Distribution Function Calculate by LSM : ");
    QString estimatedFWHMString = QStringLiteral("Calculated FWHM : ");
    QString estimatedAreaSizeString = QStringLiteral("Calculated Area Size : ");

public slots:
    void getROIRegions(std::vector<std::pair<int, int>> roiRegions);
};

#endif