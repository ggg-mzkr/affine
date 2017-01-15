//
// Created by 堀内哲煕 on 2017/01/01.
//

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"img_obj.h"

Image *initImage(int width, int height, int headerSize, char *header, bool isColor)
{
    Image *img;

    if ((img = (Image *)malloc(sizeof(Image))) == NULL) {
        return NULL;
    }

    if ((img->pRgb = (Rgb *)malloc(sizeof(Rgb)*width*height)) == NULL) {
        return NULL;
    }

    if ((img->header = (char *)malloc(sizeof(char)*headerSize)) == NULL) {
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->isColor = isColor;
    img->headerSize = headerSize;
    memcpy(img->header, header, sizeof(char)*headerSize);

    return img;
}

void freeImage(Image *img)
{
    free(img->header);

    free(img->pRgb);

    free(img);
}