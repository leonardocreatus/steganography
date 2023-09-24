/* file:          lib_ppm.c
 * description:   PPM library
 * date:          03/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lib_ppm.h"


int read_ppm(char *file, Img *image)
{
    FILE *fptr;
    char *t;
    char buf[128];
    int r, d, i, j;
    Pixel *img_data;
    
    fptr = fopen(file, "rb");
    if (!fptr)
        return -1;
        
    t = fgets(buf, sizeof(buf), fptr);
    if (!t)
        goto failread;
    
    if (strncmp(buf, "P6\n", 3) != 0)
        goto failread;

    do {
        t = fgets(buf, sizeof(buf), fptr);
        if (!t)
            goto failread;
                
    } while (strncmp(buf, "#", 1) == 0);
    
        r = sscanf(buf, "%u %u", &image->width, &image->height);
        if (r < 2)
        goto failread;

        r = fscanf(fptr, "%u", &d);
        if ((r < 1) || (d != 255))
        goto failread;
        
        fseek(fptr, 1, SEEK_CUR);
        
        img_data = malloc(image->width * image->height * sizeof(Pixel));
        if (!img_data)
        goto failread;
        
        image->pix = img_data;
        
        for (j = 0; j < image->height; j++)
        for (i = 0; i < image->width; i++)
            fread(&img_data[j * image->width + i], 3, 1, fptr);
    
    fclose(fptr);

    return 0;
    
failread:
    fclose(fptr);
    
    return -1;
}


int write_ppm(char *file, Img *image)
{
    FILE *fptr;
    int i, j;
    
    fptr = fopen(file, "wb");
    if (!fptr)
        return -1;
        
    fprintf(fptr, "P6\n%d %d\n255\n", image->width, image->height);
        for (j = 0; j < image->height; j++)
        for (i = 0; i < image->width; i++)
            fwrite(&image->pix[j * image->width + i], 3, 1, fptr);

    fclose(fptr);
    
    return 0;
}


int new_ppm(Img *image, int width, int height)
{
    Pixel *img_data;

    image->width = width;
    image->height = height;
    
        img_data = malloc(image->width * image->height * sizeof(Pixel));
        if (!img_data)
        return -1;
        
        image->pix = img_data;
        memset(image->pix, 0, image->width * image->height * sizeof(Pixel));
    
    return 0;
}


int free_ppm(Img *image)
{
    if (image->pix) {
        free(image->pix);
        
        return 0;
    }
    
    return -1;
}
