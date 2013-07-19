#include "utils.h"

#include <math.h>

int bin2dec(char *bin, int reverse)
{
    int sum = 0;
    int len = strlen(bin);
    for (int i=0;i<len;i++)
    {
        //printf("%d,%d\n",len,i);
        if (reverse>0)
            sum = sum + pow(2,i)*(bin[i] - '0');
        else
            sum = sum + pow(2,i)*(bin[len-i-1] - '0');
    }
    return(sum);
}

double elapsed_ms(timeval start, timeval end)
{
    double elapsedTime;
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
    return elapsedTime;
}

