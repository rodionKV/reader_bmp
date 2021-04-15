#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#define FILE_NAME "result.bmp"

#define WIDTH 1080
#define HEIGTH 1080
#define  LINE_WIDTH 5

#define RESULT_ERROR 1
#define RESULT_OK 0

#pragma pack(push, 1)
struct TagBMPHeader {
    unsigned short BMPSign;
    unsigned int bfSize;
    unsigned int bfReserved;
    unsigned int bfOffBits;
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};
#pragma pack(pop)

struct Rgb_pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

void write_header_bmp(struct TagBMPHeader *header, FILE *f) {
    header->BMPSign = 0x4D42;
    header->bfSize = sizeof(struct Rgb_pixel) * WIDTH * HEIGTH;
    header->bfReserved = 0;
    header->bfOffBits = 54;
    header->biSize = 40;
    header->biWidth = WIDTH;
    header->biHeight = HEIGTH;
    header->biPlanes = 1;
    header->biBitCount = 24;
    header->biCompression = 0;
    header->biSizeImage = 0;
    header->biXPelsPerMeter = 0;
    header->biYPelsPerMeter = 0;
    header->biClrUsed = 0;
    header->biClrImportant = 0;

    fwrite(header, sizeof(struct TagBMPHeader), 1, f);
}

int initialization_image(struct Rgb_pixel *arr, unsigned char R, unsigned char G, unsigned char B) {
    struct Rgb_pixel full = {.blue=B, .green=G, .red=R};
    if (arr == NULL)
        return RESULT_ERROR;
    for (size_t i = 0; i < HEIGTH * WIDTH; i++) {
        arr[i] = full;
    }
    return RESULT_OK;
}

int change_pixel_in_image(struct Rgb_pixel *arr, int x, int y, unsigned char R, unsigned char G, unsigned char B) {
    if (x < 0 || x >= WIDTH)
        return RESULT_ERROR;
    if (y < 0 || y >= HEIGTH)
        return RESULT_ERROR;
    struct Rgb_pixel change_rgb = {.red=R, .green=G, .blue=B};
    arr[y * WIDTH + x] = change_rgb;
    return RESULT_OK;
}

int main() {
    FILE *f;
    struct TagBMPHeader bmp_header;
    struct Rgb_pixel *image = NULL;

    f = fopen(FILE_NAME, "wb");

    if (f==NULL)
     return RESULT_OK;

    write_header_bmp(&bmp_header, f);

    image = malloc(WIDTH * HEIGTH * sizeof(struct Rgb_pixel));
    if(image==NULL)
        return RESULT_OK;

   if( initialization_image(image, 255, 255, 255)==RESULT_ERROR) {
       free(image);
       return RESULT_OK;
   }

   int size = min(HEIGTH, WIDTH);
    for (int x = -size / 2; x < size / 2; x++) {
        for (int y = -size / 2; y < size / 2; y++) {
            if (y == 0 || x == 0)
                change_pixel_in_image(image, size / 2 + x, size / 2 + y, 0, 0, 0);

            if (sqrt(x * x + y * y) <= size/2 && (size / 2) - sqrt(x * x + y * y) < LINE_WIDTH)
                change_pixel_in_image(image, size / 2 + x, size / 2 + y, 0, 0, 0);
            else {
                int xx = rand() % size - size / 2;
                int yy = rand() % size - size / 2;
                if (sqrt(xx * xx + yy * yy) >= size /2 || (size / 2) - sqrt(xx * xx + yy * yy) >= LINE_WIDTH) {
                    change_pixel_in_image(image, size / 2 + xx, size / 2 + yy, 255, 0, 0);
                }
            }
        }
    }

    fwrite(image, sizeof(struct Rgb_pixel), HEIGTH * WIDTH, f);
    fclose(f);
    free(image);
    
    return RESULT_OK;
}
