#include "widgets.h"
#include "styles.h"

InfoTabWidget::InfoTabWidget() {
    setWidgetUI();

    this->channelComboList << "1k" << "2k" << "4k" << "8k" << "16k" << "32k" << "65k";
}

void InfoTabWidget::setWidgetUI() {
    this->mainLayout = new QVBoxLayout();
    this->basicInformationFrame = new QFrame();
    this->channelSizeSelectComboBox = new QComboBox();

    this->seriesName = new QLabel(tr("No Spectrum Detected."));
    this->seriesName->setStyleSheet(STYLE_INFOTAB_SERIES_NAME);

    QLabel *basicInformationFrameHintLabel = new QLabel(tr("Basic Information"));
    QLabel *channelSizeSelectComboBoxHintLabel = new QLabel(tr("Select Channel Size"));

    setBasicInformationFrame();

    this->mainLayout->addWidget(this->seriesName);
    this->mainLayout->addStretch(1);

    this->mainLayout->addWidget(basicInformationFrameHintLabel);
    this->mainLayout->addWidget(this->basicInformationFrame, 5);

    this->mainLayout->addWidget(channelSizeSelectComboBoxHintLabel);
    this->mainLayout->addWidget(this->channelSizeSelectComboBox);
    this->mainLayout->addStretch(4);

    this->mainLayout->setAlignment(Qt::AlignTop);

    this->setLayout(this->mainLayout);
}

void InfoTabWidget::setBasicInformationFrame() {
    this->basicInformationFrameLayout = new QGridLayout();

    this->liveTimeLabel = new QLabel("");
    this->realTimeLabel = new QLabel("");
    this->deadTimeLabel = new QLabel("");
    this->totalCountLabel = new QLabel("");

    QLabel *liveTimeHintLabel = new QLabel(tr("Live Time : "));
    QLabel *realTimeHintLabel = new QLabel(tr("Real Time : "));
    QLabel *deadTimeHintLabel = new QLabel(tr("Dead Time : "));
    QLabel *totalCountHintLabel = new QLabel(tr("Total Count : "));

    this->basicInformationFrameLayout->addWidget(liveTimeHintLabel, 0, 0, 1, 1);
    this->basicInformationFrameLayout->addWidget(this->liveTimeLabel, 0, 1, 1, 1);
    this->basicInformationFrameLayout->addWidget(realTimeHintLabel, 1, 0, 1, 1);
    this->basicInformationFrameLayout->addWidget(this->realTimeLabel, 1, 1, 1, 1);
    this->basicInformationFrameLayout->addWidget(deadTimeHintLabel, 2, 0, 1, 1);
    this->basicInformationFrameLayout->addWidget(this->deadTimeLabel, 2, 1, 1, 1);
    this->basicInformationFrameLayout->addWidget(totalCountHintLabel, 3, 0, 1, 1);
    this->basicInformationFrameLayout->addWidget(this->totalCountLabel, 3, 1, 1, 1);

    this->basicInformationFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    this->basicInformationFrame->setLayout(this->basicInformationFrameLayout);
}