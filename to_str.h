#include <stdint.h>
#include <string.h>
#include <stdio.h>
extern void To_str(double x, unsigned char* data);
void To_str(double x, unsigned char* data)
{
    char buff[50];
    sprintf(buff, "%g", x);
    for (int i = 0; buff[i] != 0; i++)
    {
        data[i] = buff[i];
    }
}
