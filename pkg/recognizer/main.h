#include "math.h"
#include <stddef.h>
#define byte unsigned char
#define N 103
#define M 96
int algorithm(short pixels[N][M], short minutia_pixels[5000][3]);
double compare(short minutia1[5000][3], int n1, short minutia2[5000][3],
               int n2);
