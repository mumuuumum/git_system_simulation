//
// Created by 张淞钦 on 2022/1/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct arr {
    int arr [10];
};

int main () {
    char str [10] = "abc def ";
    for (int i = 0; i < 10 ;i++) {
        printf("%c",str [i]);
    }


    return 0;
}