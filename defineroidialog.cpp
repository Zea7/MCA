#include "dialogs.h"

DefineROIDialog::DefineROIDialog(std::vector<std::pair<int, int>> roiRegions) : roiRegions(roiRegions){
    setUI();
}

void DefineROIDialog::setUI(){
    this->setWindowTitle("Define ROI Region");
    this->resize(300, 250);

    outerLayout = new QGridLayout();
    innerLayout = new QGridLayout();
    okCancelLayout = new QHBoxLayout();
    buttonsLayout = new QVBoxLayout();
    showROILayout = new QGridLayout();

    start = new QLineEdit();
    end = new QLineEdit();

    start->setInputMask("000000");
    end->setInputMask("000000");

    showROIWidget = new QTableWidget();

    /* 
        showROIWidget의 기본적인 내용을 세팅

        column [start, end]
        row Count = roiRegions.size()
    */
    QStringList list;
    list << "start" << "end";
    showROIWidget->setHorizontalHeaderLabels(list);
    showROIWidget->setColumnCount(2);
    showROIWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    setROITable();

    add = new QPushButton("Add");
    replace = new QPushButton("Replace");
    remove = new QPushButton("Remove");
    removeAll = new QPushButton("Remove All");
    showRegion = new QPushButton("Show ROI Region");

    QObject::connect(this->add, SIGNAL(clicked()), this, SLOT(addROIRegion()));
    QObject::connect(this->replace, SIGNAL(clicked()), this, SLOT(replaceRegion()));
    QObject::connect(this->remove, SIGNAL(clicked()), this, SLOT(deleteROIRegion()));
    QObject::connect(this->removeAll, SIGNAL(clicked()), this, SLOT(deleteAllRegions()));
    QObject::connect(this->showRegion, SIGNAL(clicked()), this, SLOT(showROIRegion()));

    ok = new QPushButton("OK");
    cancel = new QPushButton("Cancel");

    QObject::connect(this->ok, SIGNAL(clicked()), this, SLOT(okSignal()));
    QObject::connect(this->ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(this->cancel, SIGNAL(clicked()), this, SLOT(reject()));

    QLabel *startLabel = new QLabel("Start");
    QLabel *endLabel = new QLabel("End");

    startLabel->setFixedSize(100, 10);
    startLabel->setAlignment(Qt::AlignCenter);
    endLabel->setFixedSize(100, 10);
    endLabel->setAlignment(Qt::AlignCenter);

    // okCancel Layout
    okCancelLayout->addWidget(ok);
    okCancelLayout->addWidget(cancel);

    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(add);
    buttonsLayout->addWidget(replace);
    buttonsLayout->addWidget(remove);
    buttonsLayout->addWidget(removeAll);
    buttonsLayout->addWidget(showRegion);
    buttonsLayout->addStretch(2);

    innerLayout->addWidget(startLabel, 0, 0, 1, 1);
    innerLayout->addWidget(endLabel, 0, 1, 1, 1);
    innerLayout->addWidget(start, 1, 0, 1, 1);
    innerLayout->addWidget(end, 1, 1, 1, 1);
    innerLayout->addWidget(showROIWidget, 2, 0, 3, 2);
    innerLayout->addLayout(buttonsLayout, 0, 2, 5, 1);

    outerLayout->addLayout(innerLayout, 0, 0, 4, 1);
    outerLayout->addLayout(okCancelLayout, 6, 0, 1, 1);

    this->setLayout(outerLayout);
}

void DefineROIDialog::setROITable(){
    showROIWidget->setRowCount(this->roiRegions.size());
    for(int i=0;i<roiRegions.size();i++){
        showROIWidget->setItem(i, 0, new QTableWidgetItem(QString::number(roiRegions[i].first)));
        showROIWidget->setItem(i, 1, new QTableWidgetItem(QString::number(roiRegions[i].second)));
    }
}

/* 
    private slots
*/

void DefineROIDialog::addROIRegion(){
    int start = this->start->text().toInt();
    int end = this->end->text().toInt();

    auto iter = std::find(roiRegions.begin(), roiRegions.end(), std::make_pair(start, end));
    if(iter == roiRegions.end())
        roiRegions.push_back({start, end});
    setROITable();
}

void DefineROIDialog::deleteROIRegion(){
    QModelIndexList indexes = this->showROIWidget->selectionModel()->selectedRows();
    std::vector<std::pair<int, int>> deleteRegions;
    for(int i=indexes.count() - 1;i>=0;i--){
        deleteRegions.push_back(roiRegions[indexes.at(i).row()]);
    }

    std::vector<std::pair<int, int>> result;
    std::set_difference(
        roiRegions.begin(), roiRegions.end(),
        deleteRegions.begin(), deleteRegions.end(),
        std::back_inserter(result)
    );
    this->roiRegions = result;
    setROITable();
}

void DefineROIDialog::deleteAllRegions(){
    roiRegions.clear();
    roiRegions.shrink_to_fit();

    setROITable();
}

void DefineROIDialog::replaceRegion(){
    int start = this->start->text().toInt();
    int end = this->end->text().toInt();

    QModelIndexList indexes = this->showROIWidget->selectionModel()->selectedRows();
    if(indexes.count() > 1) return; //TODO
    roiRegions[indexes.at(0).row()] = std::make_pair(start, end);
    setROITable();
}

void DefineROIDialog::okSignal(){
    emit sendROIRegions(roiRegions);
}

void DefineROIDialog::showROIRegion(){
    QModelIndexList indexes = this->showROIWidget->selectionModel()->selectedRows();
    if(indexes.count() > 1) return; //TODO
    std::pair<int, int> region = roiRegions[indexes.at(0).row()];

    emit sendShowRegion(region.first, region.second);
}