#ifndef WIDGETS_H
#define WIDGETS_H

#include <vector>

#include <QWidget>

#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>

class ROITab : public QWidget {
    Q_OBJECT

public:
    ROITab();

private:
    void setUI();

    void setPointCountComboBox();

    QVBoxLayout *mainLayout;
    QComboBox *roiComboBox;
    QComboBox *pointCountComboBox;

    std::vector<std::pair<int, int>> roiRegions;
    std::vector<std::pair<int, std::pair<int, int>>> data;

public slots:
    void getROIRegions(std::vector<std::pair<int, int>> roiRegions);
};


#endif