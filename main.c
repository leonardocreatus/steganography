#include <stdio.h>
#include "tools.h"
#include <stdint.h>
#include <string.h>


void throw(char* msg){
    printf("%s\n", msg);
    exit(1);
}

void code(char* source, char* hiding){
    
    Img sourceImg;
    Img *ptrSourceImg = &sourceImg;

    Img hidingImg;
    Img *ptrHidingImg = &hidingImg;

    int r = read_ppm(source, ptrSourceImg);
    if(r != 0) throw("Erro ao ler a imagem de origem");

    r = read_ppm(hiding, ptrHidingImg);
    if(r != 0) throw("Erro ao ler a imagem para esconder");

    if((ptrHidingImg->width * ptrHidingImg->height * 4) > (ptrSourceImg->width * ptrSourceImg->height)){
        throw("Imagem para esconder é maior que a proporção suportada");
    } 

    int widthHideImage = ptrHidingImg->width;    // 12 bits
    int heightHideImage = ptrHidingImg->height;  // 12 bits

    uint8_t headersHideImage[3];
    
    headersHideImage[0] = (uint8_t)heightHideImage;
    headersHideImage[1] = ((uint8_t)(heightHideImage >> 8) & 0b00001111) | (uint8_t)(widthHideImage << 4);
    headersHideImage[2] = (uint8_t)(widthHideImage >> 4);

    // 4 primeiros pixels da imagem de origem
    Pixel* pixels[4];
    pixels[0] = &ptrSourceImg->pix[0];
    pixels[1] = &ptrSourceImg->pix[1];
    pixels[2] = &ptrSourceImg->pix[2];
    pixels[3] = &ptrSourceImg->pix[3];

    // colocando os headers na imagem de origem
    cipher(pixels, headersHideImage);

    for(int i = 4; i < ptrSourceImg->width * ptrSourceImg->height; i+=4){
        pixels[0] = &ptrSourceImg->pix[i];
        pixels[1] = &ptrSourceImg->pix[i+1];
        pixels[2] = &ptrSourceImg->pix[i+2];
        pixels[3] = &ptrSourceImg->pix[i+3];

        int index = i/4 - 1;
        
        if(index < (ptrHidingImg->width * ptrHidingImg->height)){
            cipher(pixels, (uint8_t*)&ptrHidingImg->pix[index]);
        }else{
            break;
        }
    }

    write_ppm("resultante-code.ppm",ptrSourceImg);

    free_ppm(ptrSourceImg);
    free_ppm(ptrHidingImg);
}


void decode(char* source){
    Img sourceImg;
    Img hidingImg;
    Img *ptrSourceImg = &sourceImg;
    Img *ptrHidingImg = &hidingImg;

    int r = read_ppm(source, ptrSourceImg);
    if(r != 0) throw("Erro ao ler a imagem de origem");

    Pixel* pixels[4];
    pixels[0] = &ptrSourceImg->pix[0];
    pixels[1] = &ptrSourceImg->pix[1];
    pixels[2] = &ptrSourceImg->pix[2];
    pixels[3] = &ptrSourceImg->pix[3];
    uint8_t* results = decipher(pixels);
    
    int height = (results[0] | (results[1] << 8)) & 0x00000FFF;
    int width = ((results[1] >> 4) | (results[2] << 4)) & 0x00000FFF;

    r = new_ppm(ptrHidingImg, width, height);
    if(r != 0) throw("Erro ao criar a imagem resultante");
    printf("width: %d, height: %d\n", ptrHidingImg->width, ptrHidingImg->height);

    for(int i = 4; i < ptrSourceImg->width * ptrSourceImg->height; i+=4){
        pixels[0] = &ptrSourceImg->pix[i];
        pixels[1] = &ptrSourceImg->pix[i+1];
        pixels[2] = &ptrSourceImg->pix[i+2];
        pixels[3] = &ptrSourceImg->pix[i+3];

        int index = i/4 - 1;
        if(index > (ptrHidingImg->width * ptrHidingImg->height)){
            break;
        }

        uint8_t* results = decipher(pixels);
        ptrHidingImg->pix[index].r = results[0];
        ptrHidingImg->pix[index].g = results[1];
        ptrHidingImg->pix[index].b = results[2];
    }

    write_ppm("resultante-decode.ppm",ptrHidingImg);
    free_ppm(ptrHidingImg);
    free_ppm(ptrSourceImg);

}


int main(int argc, char** argv)
{
    if(argc < 2) throw("./hide <method> ...");

    char* method = argv[1];
    if(strcmp(method, "code") != 0 && strcmp(method, "decode") != 0) throw("Método não encontrado, utilize 'code' ou 'decode'");

    if(strcmp(method, "code") == 0){
        if(argc != 4) throw("./hide code <source> <hiding>");
        char* source = argv[2];
        char* hiding = argv[3];
        code(source, hiding);
    }

    if(strcmp(method, "decode") == 0){
        if(argc != 3) throw("./hide decode <source>");
        char* source = argv[2];
        decode(source);
    }

    return 0;
}
