#include "dialogs.h"

AutoPeakDialog::AutoPeakDialog(int start, int end) : startSample(start), endSample(end){
    setUI();
}

void AutoPeakDialog::setUI(){
    this->resize(350, 300);
    this->setWindowTitle("Peak Search");

    outerLayout = new QGridLayout();
    innerLayout = new QGridLayout();
    okCancelLayout = new QHBoxLayout();
    buttonsLayout = new QVBoxLayout();
    peakConfigLayout = new QGridLayout();

    outerFrame = new QFrame();
    outerFrame->setFrameStyle(QFrame::Box | QFrame::Raised);

    startRange = new QLineEdit(QString::number(startSample));
    endRange = new QLineEdit(QString::number(endSample));

    setLeft = new QLineEdit(QString::number(5));
    setRight = new QLineEdit(QString::number(5));

    searchRange = new QPushButton("Search Range");
    searchAll = new QPushButton("Search All");
    addToROI = new QPushButton("Add To ROI List");
    replaceROIList = new QPushButton("Replace ROI List");

    ok = new QPushButton("OK");
    cancel = new QPushButton("Cancel");

    std::vector<QLineEdit *> edits = {startRange, endRange, setLeft, setRight};
    std::vector<QLabel *> labels = {new QLabel("Start Range"), new QLabel("End Range"), new QLabel("Left Point"), new QLabel("Right Point")};

    for(auto *i : labels){
        i->setFixedSize(100, 20);
        i->setAlignment(Qt::AlignLeft);
    }

    for(int i=0;i<4;i++){
        peakConfigLayout->addWidget(labels[i], i, 0, 1, 1);
        peakConfigLayout->addWidget(edits[i], i , 1, 1, 1);
    }

    okCancelLayout->addWidget(ok);
    okCancelLayout->addWidget(cancel);


    buttonsLayout->addWidget(searchRange);
    buttonsLayout->addWidget(searchAll);
    buttonsLayout->addWidget(addToROI);
    buttonsLayout->addWidget(replaceROIList);

    innerLayout->addLayout(peakConfigLayout, 0, 0, 1, 1);
    innerLayout->addLayout(buttonsLayout, 0, 1, 1, 1);

    outerFrame->setLayout(innerLayout);

    outerLayout->addWidget(outerFrame, 0, 0, 4, 1);
    outerLayout->addLayout(okCancelLayout, 4, 0, 1, 1);

    this->setLayout(outerLayout);

    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(sendVariables()));
    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void AutoPeakDialog::sendVariables(){
    startSample = startRange->text().toInt();
    endSample = endRange->text().toInt();
    leftPoint = setLeft->text().toInt();
    rightPoint = setRight->text().toInt();
    emit doAutoPeakSearch(startSample, endSample, leftPoint, rightPoint);
}