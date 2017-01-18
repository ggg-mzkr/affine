//
// Created by 堀内哲煕 on 2017/01/01.
//
#define PI 3.141592

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "affine.h"
#include "img_obj.h"


Image *scale(Image *img, char direction, double scale)
{
    int i, j;
    int x, y;
    int newWidth, newHeight;
    int oldWidth, oldHeight;
    double mat[3][3] = {{1,0,0,}, {0,1,0}, {0,0,1}};
    Image *converted_img;
    Rgb black = {0,0,0};


    oldWidth = img->width;
    oldHeight = img->height;

    newWidth = img->width;
    newHeight = img->height;

    // 変換行列の作成
    if (direction == 'x') {
        mat[0][0] = scale;
        newWidth = (int)(fabs(oldWidth*scale));
    } else if (direction == 'y') {
        mat[1][1] = scale;
        newHeight = (int)(fabs(oldHeight*scale));
    } else {
        fprintf(stderr, "引数が正しくありません: scale %c %lf\n", direction, scale);
        return NULL;
    }

    // 鏡映時のオフセット
    if (scale < 0) {
        if (direction == 'x') {
            mat[0][2] = newWidth;
        } else {
            mat[1][2] = newHeight;
        }
    }

    toInv(mat);

    converted_img = initImage(newWidth, newHeight, img->headerSize, img->header, img->isColor);

    for (i = 0; i<newHeight; i++) {
        for (j = 0; j <newWidth; j++) {
            x = (int)(mat[0][0]*j + mat[0][1]*i + mat[0][2]);
            y = (int)(mat[1][0]*j + mat[1][1]*i + mat[1][2]);

            if ( (0<=x) && (x<oldWidth) && (0<=y) && (y<oldHeight)) {
                memcpy(&converted_img->pRgb[i * newWidth + j], &img->pRgb[y * oldWidth + x], sizeof(Rgb));
            } else {
                memcpy(&converted_img->pRgb[i * newWidth + j], &black, sizeof(Rgb));
            }
        }
    }

    return converted_img;
}


Image *move(Image *img, char direction, int pix)
{
    int i, j;
    int x, y;
    int newWidth, newHeight;
    int oldWidth, oldHeight;
    double mat[3][3] = {{1,0,0,}, {0,1,0}, {0,0,1}};
    Image *converted_img;
    Rgb black = {0,0,0};

    oldWidth = img->width;
    oldHeight = img->height;

    newWidth = img->width;
    newHeight = img->height;

    // 変換行列の作成
    if (direction == 'x') {
        mat[0][2] = pix;
        newWidth = oldWidth+abs(pix);
    } else if (direction == 'y') {
        mat[1][2] = pix;
        newHeight = oldHeight+abs(pix);
    } else {
        fprintf(stderr, "引数が正しくありません: move %c %d\n", direction, pix);
        return NULL;
    }


    // 負の方向へ移動する際のオフセット
    if (pix < 0) {

        if (direction == 'x') {
            mat[0][2] = 0;

        } else {
            mat[1][2] = 0;
        }
    }

    toInv(mat);

    converted_img = initImage(newWidth, newHeight, img->headerSize, img->header, img->isColor);

    for (i = 0; i<newHeight; i++) {
        for (j = 0; j <newWidth; j++) {
            x = (int)(mat[0][0]*j + mat[0][1]*i + mat[0][2]);
            y = (int)(mat[1][0]*j + mat[1][1]*i + mat[1][2]);

            if ( (0<=x) && (x<oldWidth) && (0<=y) && (y<oldHeight)) {
                memcpy(&converted_img->pRgb[i * newWidth + j], &img->pRgb[y * oldWidth + x], sizeof(Rgb));
            } else {
                memcpy(&converted_img->pRgb[i * newWidth + j], &black, sizeof(Rgb));
            }
        }
    }

    return converted_img;
}


Image *rotation(Image *img, char direction, int degrees)
{
    int i, j;
    int x, y;
    int newWidth, newHeight;
    int oldWidth, oldHeight;
    int tmpDegrees;
    double theta;
    double mat[3][3] = {{1,0,0,}, {0,1,0}, {0,0,1}};
    Image *converted_img;
    Rgb black = {0,0,0};


    oldWidth = img->width;
    oldHeight = img->height;

    theta = degrees * PI / 180;

    newHeight = (int)(fabs(cos(theta)*oldHeight) + fabs(sin(theta)*oldWidth));
    newWidth = (int)(fabs(sin(theta)*oldHeight) + fabs(cos(theta)*oldWidth));

    // 変換行列の作成
    if (direction == 'r') {

        mat[0][0] = cos(theta);
        mat[0][1] = sin(theta);
        mat[1][0] = -sin(theta);
        mat[1][1] = cos(theta);

    } else if (direction == 'l') {

        mat[0][0] = cos(theta);
        mat[0][1] = -sin(theta);
        mat[1][0] = sin(theta);
        mat[1][1] = cos(theta);

    } else {
        fprintf(stderr, "引数が正しくありません: rotation %c %d\n", direction, degrees);
        return NULL;
    }

    // 第１象限からはみ出す分のオフセット
    tmpDegrees = degrees % 360;
    if (0 < tmpDegrees && tmpDegrees <= 90) {

        if (direction == 'r') {
            mat[1][2] = sin(theta)*oldWidth;
        } else {
            mat[0][2] = sin(theta)*oldHeight;
        }

    } else if (90 < tmpDegrees && tmpDegrees <= 180) {

        if (direction == 'r') {
            mat[1][2] = newHeight;
            mat[0][2] = fabs(cos(theta))*oldWidth;
        } else {
            mat[1][2] = fabs(cos(theta))*oldHeight;
            mat[0][2] = newWidth;
        }

    } else if (180 < tmpDegrees && tmpDegrees <= 270) {

        if (direction == 'r') {
            mat[1][2] = fabs(cos(theta))*oldHeight;
            mat[0][2] = newWidth;
        } else {
            mat[1][2] = newHeight;
            mat[0][2] = fabs(cos(theta))*oldWidth;
        }

    } else if (270 < tmpDegrees && tmpDegrees <= 360) {

        if (direction == 'r') {
            mat[0][2] = fabs(sin(theta))*oldHeight;
        } else {
            mat[1][2] = fabs(sin(theta))*oldWidth;
        }

    }

    toInv(mat);

    converted_img = initImage(newWidth, newHeight, img->headerSize, img->header, img->isColor);

    for (i = 0; i<newHeight; i++) {
        for (j = 0; j <newWidth; j++) {
            x = (int)(mat[0][0]*j + mat[0][1]*i + mat[0][2]);
            y = (int)(mat[1][0]*j + mat[1][1]*i + mat[1][2]);

            if ( (0<=x) && (x<oldWidth) && (0<=y) && (y<oldHeight)) {
                memcpy(&converted_img->pRgb[i * newWidth + j], &img->pRgb[y * oldWidth + x], sizeof(Rgb));
            } else {
                memcpy(&converted_img->pRgb[i * newWidth + j], &black, sizeof(Rgb));
            }
        }
    }

    return converted_img;
}


Image *skew(Image *img, char direction, int degrees)
{
    int i, j;
    int x, y;
    int newWidth, newHeight;
    int oldWidth, oldHeight;
    int tmpDegrees;
    double theta;
    double mat[3][3] = {{1,0,0,}, {0,1,0}, {0,0,1}};
    Image *converted_img;
    Rgb black = {0,0,0};


    oldWidth = img->width;
    oldHeight = img->height;

    newWidth = img->width;
    newHeight = img->height;

    if (degrees == 90 || degrees == 270) {
        fprintf(stderr, "%d度ではせん断できません\n", degrees);
        return NULL;
    }

    theta = degrees * PI / 180;

    // 変換行列の作成
    if (direction == 'x') {
        mat[0][1] = tan(degrees * PI / 180);
        newWidth = (int)(oldWidth+fabs(oldHeight*tan(theta)));
    } else if (direction == 'y') {
        mat[1][0] = tan(degrees * PI / 180);
        newHeight = (int)(oldHeight+fabs(oldWidth*tan(theta)));

    } else {
        fprintf(stderr, "引数が正しくありません: skew %c %d\n", direction, degrees);
        return NULL;
    }

    // 第１象限からはみ出す分のオフセット
    tmpDegrees = degrees % 360;
    if ((90 < tmpDegrees && tmpDegrees < 180) || (270 < tmpDegrees && tmpDegrees < 360)) {
        if (direction == 'y') {
            mat[1][2] = newWidth*fabs(tan(theta));
        } else {
            mat[0][2] = newHeight*fabs(tan(theta));
        }
    }

    toInv(mat);

    converted_img = initImage(newWidth, newHeight, img->headerSize, img->header, img->isColor);

    for (i = 0; i<newHeight; i++) {
        for (j = 0; j <newWidth; j++) {
            x = (int)(mat[0][0]*j + mat[0][1]*i + mat[0][2]);
            y = (int)(mat[1][0]*j + mat[1][1]*i + mat[1][2]);

            if ( (0<=x) && (x<oldWidth) && (0<=y) && (y<oldHeight)) {
                memcpy(&converted_img->pRgb[i * newWidth + j], &img->pRgb[y * oldWidth + x], sizeof(Rgb));
            } else {
                memcpy(&converted_img->pRgb[i * newWidth + j], &black, sizeof(Rgb));
            }
        }
    }

    return converted_img;
}





void toInv(double mat[3][3])
{

    double inv[3][3] = {{1,0,0,}, {0,1,0}, {0,0,1}};;
    double buf;
    int i, j, k;

    for(i=0; i<3; i++){

        buf = 1 / mat[i][i];

        for(j=0; j<3; j++){

            mat[i][j] *= buf;
            inv[i][j] *= buf;
        }

        for(j=0; j<3; j++){

            if(i!=j){

                buf=mat[j][i];
                for(k=0; k<3; k++){
                    mat[j][k] -= mat[i][k] * buf;
                    inv[j][k] -= inv[i][k] * buf;
                }
            }
        }
    }

    for (i = 0; i<3; i++) {
        for (j = 0; j<3; j++) {
            mat[i][j] = inv[i][j];
        }
    }
}