#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

#include <QLabel>

class SpectrumListManager : public QDialog {
    Q_OBJECT

public:
    SpectrumListManager();
    ~SpectrumListManager();

private:
    /* 
        private UI functions

        void setUI()    :   df
        void setOKCancelLayoutUI()     :   
        void setChartPreviewUI()    :   
        void setSpectrumListLayoutUI()   :   
    */
    void setUI();
    void setOKCancelLayoutUI();
    void setChartPreviewUI();
    void setSpectrumListLayoutUI();

    QVBoxLayout *mainLayout;

    QVBoxLayout *spectrumListLayout;
    QHBoxLayout *spectrumSettingLayout;
    QHBoxLayout *spectrumSettingHintLabelLayout;
    QPushButton *addSpectrumButton;
    QComboBox *spectrumListComboBox;
    QComboBox *channelSizeSelectComboBox;
    QPushButton *changeColorPushButton;

    QHBoxLayout *OKCancelLayout;
    QPushButton *OKButton;
    QPushButton *cancelButton;
    
};

#endif