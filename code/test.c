//
// Created by 张淞钦 on 2022/1/30.
//

#include <stdio.h>

struct arr {
    int arr [10];
};

int main () {

    struct arr first;
    struct arr second;
    for (int i = 0 ,j = 9; i < 10 ;i++,j--) {
        first.arr [i] = i;
        second.arr [i] = i
    }


    return 0;
}