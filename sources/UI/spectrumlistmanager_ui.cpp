#include "dialogs.h"

SpectrumListManager::SpectrumListManager() {
    setUI();
}

SpectrumListManager::~SpectrumListManager() {

}

void SpectrumListManager::setUI() {
    setWindowTitle("Spectrum Manager");
    resize(800, 600);

    // Set Main Layout
    this->mainLayout = new QVBoxLayout();

    // Set OKCancelLayout
    this->OKCancelLayout = new QHBoxLayout();
    setOKCancelLayoutUI();

    // Set Spectrum List Layout
    this->spectrumListLayout = new QVBoxLayout();
    setSpectrumListLayoutUI();

    this->mainLayout->addLayout(this->spectrumListLayout, 1);
    this->mainLayout->addStretch(5);
    this->mainLayout->addLayout(this->OKCancelLayout, 1);

    this->setLayout(this->mainLayout);  
}

// StyleSheet로 좀 더 정확히 조정 필요할듯
void SpectrumListManager::setSpectrumListLayoutUI() {
    this->spectrumSettingHintLabelLayout = new QHBoxLayout();
    
    QLabel *channelSizeSelectHintLabel = new QLabel("Select Channel size to show");
    QLabel *changeColorHintLabel = new QLabel("Change Color");
    QLabel *spectrumListHintLabel = new QLabel("Select Spectrum data to change setting");

    this->spectrumSettingHintLabelLayout->addStretch(1);
    this->spectrumSettingHintLabelLayout->addWidget(spectrumListHintLabel, 4);
    this->spectrumSettingHintLabelLayout->addWidget(channelSizeSelectHintLabel, 2);
    this->spectrumSettingHintLabelLayout->addWidget(changeColorHintLabel, 1);

    this->spectrumSettingLayout = new QHBoxLayout();

    this->addSpectrumButton = new QPushButton("Add Spectrum");
    this->spectrumListComboBox = new QComboBox();
    this->channelSizeSelectComboBox = new QComboBox();
    this->changeColorPushButton = new QPushButton();

    this->spectrumSettingLayout->addWidget(this->addSpectrumButton, 1);
    this->spectrumSettingLayout->addWidget(this->spectrumListComboBox, 4);
    this->spectrumSettingLayout->addWidget(this->channelSizeSelectComboBox, 2);
    this->spectrumSettingLayout->addWidget(this->changeColorPushButton, 1);

    this->spectrumListLayout->setContentsMargins(1, 0.5, 1, 0.5);
    this->spectrumListLayout->addLayout(this->spectrumSettingHintLabelLayout);
    this->spectrumListLayout->addLayout(this->spectrumSettingLayout);
}

void SpectrumListManager::setOKCancelLayoutUI() {
    this->OKCancelLayout->setAlignment(Qt::AlignRight);

    this->OKButton = new QPushButton("OK");
    this->cancelButton = new QPushButton("Cancel");

    this->OKCancelLayout->addWidget(this->OKButton);
    this->OKCancelLayout->addWidget(this->cancelButton);
}