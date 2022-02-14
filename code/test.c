#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ll long long
int main () {
    ll _begin_time = clock(); //记录开始时间

    int ans = 0;
    int x = 13;
    for (int i = 2 ; i <= 1e9 ; i++) {
        ans += i % x;
    }
    printf("%d\n",ans);
    ll _end_time = clock();
    printf("time = %ld ms\n", _end_time - _begin_time);

    return 0;
}