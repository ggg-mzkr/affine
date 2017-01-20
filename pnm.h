//
// Created by 堀内哲煕 on 2016/12/31.
//

#ifndef AFFINE_PPM_H
#define AFFINE_PPM_H

#include <stdlib.h>
#include <stdio.h>
#include "img_obj.h"

#define HEADER_SIZE_PPM 20
#define FORMAT_OFFSET 0
#define COLOR_OFFSET 10

Image *readImage(char *filename);
int *writeImage(char *filename, Image *image);
void getNextToken(FILE *fp, char *buf);
#endif //AFFINE_PPM_H
