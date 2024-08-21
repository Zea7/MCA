#include "utils.h"

int getClosestInt(int a){
    int first, cnt = 0;

    while(a>10){
        a = a/10;
        cnt++;
    }

    first = a + 1;

    while(cnt>0){
        first *= 10;
        cnt--;
    }

    return first;
}

std::pair<int, int> getBorderByThreshold(std::vector<int> data, int threshold){
    int end, start;
    bool check = false;
    for(int i = 0; i<data.size();i++){
        if(data[i] >= threshold) {
            if(check){
                end = i;
            }
            else {
                check = true;
                start = i;
            }
            
        }
    }

    return std::make_pair(start, end);
}

void clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item= layout->takeAt(0))) {
        if(item->layout()){
            clearLayout(item->layout());
        }
        if(item->widget()){
            delete item->widget();
        }
        delete item;
    }
}