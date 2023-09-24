#include "lib_ppm.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void cipher(Pixel* pixel[4],  uint8_t value[3]); // 8 x 3 = 24 bits
uint8_t* decipher(Pixel* pixel[4]);