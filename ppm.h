//
// Created by 堀内哲煕 on 2016/12/31.
//

#ifndef AFFINE_PPM_H
#define AFFINE_PPM_H

#include <stdlib.h>
#include <stdio.h>
#include "img_obj.h"


Image *read_image(char *filename);
void write_image(char *filename, Image *image);
void getNextToken(FILE *fp, char *buf);
#endif //AFFINE_PPM_H
