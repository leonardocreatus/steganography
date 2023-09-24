/* file:          lib_ppm.h
 * description:   PPM library
 * date:          03/2023
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int width;
    int height;
    Pixel *pix;
} Img;

int read_ppm(char *file, Img *image);
int write_ppm(char *file, Img *image);
int new_ppm(Img* image, int width, int height);
int free_ppm(Img *image);
