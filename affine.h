//
// Created by 堀内哲煕 on 2017/01/01.
//

#ifndef AFFINE_AFFINE_H
#define AFFINE_AFFINE_H

#include "img_obj.h"

// direction = x or y
Image *scale(Image *img, char direction, double scale);

// direction = x or y
Image *move(Image *img, char direction, int pix);

// direction = r or l
Image *rotation(Image *img, char direction, int degrees);

// direction = x or y
Image *skew(Image *img, char direction, int degrees);

void toInv(double mat[3][3]);

#endif //AFFINE_AFFINE_H
