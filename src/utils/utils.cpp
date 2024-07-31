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