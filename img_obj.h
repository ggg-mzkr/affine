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
    int headerSize;
    char *header;
    Rgb *pRgb;
    bool isColor;
}Image;

Image *initImage(int width, int height, int headerSize, char *header, bool isColor);

void freeImage(Image *img);


#endif //AFFINE_PHOTO_OBJ_H
