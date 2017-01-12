//
// Created by 堀内哲煕 on 2016/12/31.
//

#ifndef AFFINE_PHOTO_OBJ_H
#define AFFINE_PHOTO_OBJ_H

#include <stdbool.h>

typedef struct{
    int r;
    int b;
    int g;
}Rgb;

typedef struct {
    int width;
    int height;
    int color;
    char *format;
    Rgb *pRgb;
    bool isColor;
}Image;

Image *initImage(int width, int height, int color, char *format);

void freeImage(Image *img);


#endif //AFFINE_PHOTO_OBJ_H
