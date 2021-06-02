#include <stdint.h>
#include <string.h>
#include <stdio.h>
extern void To_string(float x,char* out);
void To_string(float x,char* out)
{
sprintf(out,'%f',x);
}
