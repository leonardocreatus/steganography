#include "tools.h"

void cipher(Pixel* pixel[4],  uint8_t value[3]){
    
    // POSIÇÃO 00 01 02 03 04 05 06 07 08 09 10 11
    // BITS    00 00 00 00 11 11 11 11 22 22 22 22
    // PIXEL   00 00 00 11 11 11 22 22 22 33 33 33
    // COR     RR GG BB RR GG BB RR GG BB RR GG BB
    static int c = 0;
    if(c++ == 0 ) printf("value: %d %d %d\n", value[0], value[1], value[2]);
    for(int i = 0; i < 12 ; i++){
        int indexValue = floor(i/4);
        int indexPixel = floor(i/3);

        int bits = value[indexValue] & (0b00000011);

        if(i % 3 == 0) {
            pixel[indexPixel]->r = (pixel[indexPixel]->r & (0b11111100)) | bits;
        }else if(i % 3 == 1) {
            pixel[indexPixel]->g = (pixel[indexPixel]->g & (0b11111100)) | bits;
        }else {
            pixel[indexPixel]->b = (pixel[indexPixel]->b & (0b11111100)) | bits;
        }

        value[indexValue] = value[indexValue] >> 2;
    }
}


uint8_t* decipher(Pixel* pixel[4]){
    // POSIÇÃO 00 01 02 03 04 05 06 07 08 09 10 11
    // BITS    00 00 00 00 11 11 11 11 22 22 22 22
    // PIXEL   00 00 00 11 11 11 22 22 22 33 33 33
    // COR     RR GG BB RR GG BB RR GG BB RR GG BB

    uint8_t* values = malloc(sizeof(uint8_t) * 3);
    for(int i = 0; i < 12 ; i++){
        int indexValue = floor(i/4);
        int indexPixel = floor(i/3);
        if(i % 3 == 0) { // red
            int bits = pixel[indexPixel]->r & (0b00000011);
            bits = bits << 2 * ((i % 4));
            values[indexValue] = values[indexValue] | bits; 
        }else if(i % 3 == 1) { // green
            int bits = pixel[indexPixel]->g & (0b00000011);
            bits = bits << 2 * ((i % 4));
            values[indexValue] = values[indexValue] | bits;
        }else { // blue
            int bits = pixel[indexPixel]->b & (0b00000011);
            bits = bits << 2 * ((i % 4));
            values[indexValue] = values[indexValue] | bits;
        }
    }

    static int c = 0;
    if(c++ == 0 ) printf("value: %d %d %d\n", values[0], values[1], values[2]);

    return values;
}