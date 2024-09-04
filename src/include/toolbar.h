#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QIcon>
#include <QAction>

class ToolBar : public QToolBar {
    Q_OBJECT

public:
    ToolBar();

private:
    QAction *open;
    QAction *save;
    QAction *resizeHorizontal;
    QAction *resizeVertical;

    QAction *roiDialog;
    QAction *magnification;

signals:
    void openFile();
    void saveFile();
    void resizeXAxis();
    void resizeYAxis();
    void openROIDialog();
    void showSpecificRegion();
};

#endif