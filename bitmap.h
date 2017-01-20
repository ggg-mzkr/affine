//
// Created by 堀内哲煕 on 2017/01/02.
//

#ifndef __AFFINE_BITMAP_H__
#define __AFFINE_BITMAP_H__

#define HEADER_SIZE_BITMAP 54

#include "img.h"

Image *readImage(char *filename);

int *writeImage(char *filename, Image *img);

#endif /*__AFFINE_BITMAP_H__*/
