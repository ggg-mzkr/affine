//
// Created by 堀内哲煕 on 2017/01/01.
//

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"img_obj.h"

Image *initImage(int width, int height, int color, char *format)
{
    Image *img;

    if ((img = (Image *)malloc(sizeof(Image))) == NULL) {
        return NULL;
    }

    if ((img->pRgb = (Rgb *)malloc(sizeof(Rgb)*width*height)) == NULL) {
        return NULL;
    }

    if ((img->format = (char *)malloc(sizeof(format))) == NULL) {
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->color = color;
    strcpy(img->format, format);

    return img;
}

void freeImage(Image *img)
{
    free(img->format);

    free(img->pRgb);

    free(img);
}