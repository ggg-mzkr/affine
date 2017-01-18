//
// Created by 堀内哲煕 on 2017/01/02.
//

#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include"bitmap.h"
#include "img_obj.h"


Image *readImage(char *filename)
{
    int   i, j;
    int   width, height;
    int   color;
    int   rowSize;
    char  *row;
    char  header[HEADER_SIZE_BITMAP];
    FILE  *fp;
    Image *img;

    if((fp = fopen(filename, "rb")) == NULL){
        fprintf(stderr, "エラー: %s が読み取れません\n", filename);
        return NULL;
    }

    fread(header, sizeof(char), HEADER_SIZE_BITMAP, fp);

    if(strncmp(header, "BM", 2)){
        fclose(fp);
        fprintf(stderr, "エラー: %s はbitmapではありません\n", filename);
        return NULL;
    }

    memcpy(&width, header + 18, sizeof(width));
    memcpy(&height, header + 22, sizeof(height));
    memcpy(&color, header + 28, sizeof(color));

    if (color != 24) {
        fclose(fp);
        fprintf(stderr, "エラー: 24bitカラーのbitmapのみ対応しています\n");
        return NULL;
    }

    if((img = initImage(width, height, HEADER_SIZE_BITMAP, header, true)) == NULL){
        fclose(fp);
        fprintf(stderr, "エラー: 画像の初期化に失敗しました\n");
        return NULL;
    }

    rowSize = width*3 + width%4;

    if((row = ( char *)malloc(sizeof( char) * rowSize)) == NULL){
        fclose(fp);
        fprintf(stderr, "エラー: メモリの初期化に失敗しました\n");
        return NULL;
    }

    for(i=0; i<height; i++){
        fread(row, 1, sizeof(char)*rowSize, fp);
        for(j=0; j<width; j++){
            img->pRgb[i*width + j].b = row[j*3];
            img->pRgb[i*width + j].g = row[j*3 + 1];
            img->pRgb[i*width + j].r = row[j*3 + 2];
        }
    }

    free(row);
    fclose(fp);

    return img;
}

int *writeImage(char *filename, Image *img)
{
    int  i, j;
    int  width, height;
    int  rowSize;
    char *row;
    char header[HEADER_SIZE_BITMAP];
    FILE *fp;
    
    int fileSize;
    long dataSize;


    if((fp = fopen(filename, "wb")) == NULL){
        fprintf(stderr, "エラー: %s が読み取れません", filename);
        return NULL;
    }

    width  = img->width;
    height = img->height;

    rowSize   = width*3 + width%4;
    dataSize  = height * rowSize;
    fileSize  = height * rowSize + HEADER_SIZE_BITMAP;


    memcpy(header, img->header, sizeof(char)*HEADER_SIZE_BITMAP);

    // ヘッダ情報の書き換え
    memcpy(header + 2, &fileSize, sizeof(fileSize));
    memcpy(header + 18, &width, sizeof(width));
    memcpy(header + 22, &height, sizeof(height));
    memcpy(header + 34, &dataSize, sizeof(dataSize));


    fwrite(header, sizeof(char), HEADER_SIZE_BITMAP, fp);

    if((row = (char *)malloc(sizeof(char)*rowSize)) == NULL){
        fprintf(stderr, "エラー: メモリの初期化に失敗しました\n");
        fclose(fp);
        return NULL;
    }

    for(i=0; i<height; i++){

        for(j=0; j<width; j++){
            row[j*3]     = img->pRgb[i*width + j].b;
            row[j*3 + 1] = img->pRgb[i*width + j].g;
            row[j*3 + 2] = img->pRgb[i*width + j].r;
        }

        for(j=width*3; j<rowSize; j++){
            row[j] = 0;
        }

        fwrite(row, sizeof(char), sizeof(char)*rowSize, fp);
    }

    free(row);
    fclose(fp);

    return 0;
}
