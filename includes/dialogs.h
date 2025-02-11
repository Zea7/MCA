#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>

class SerialSettingDialog : public QDialog {
    Q_OBJECT

public:
    SerialSettingDialog();
    ~SerialSettingDialog();

private:
    void setUI();



    /* 
        QWidget part


    */

    // Layouts
    QHBoxLayout *mainLayout;
    QVBoxLayout *outerLayout;

    // Widgets
    QComboBox *typeSelectBox;
};

#endif