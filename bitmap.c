//
// Created by 堀内哲煕 on 2017/01/02.
//

#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include"bitmap.h"
#include "img_obj.h"


Image *read_image(char *filename)
{
    int   i, j;
    int   width, height;
    int   color;
    int   row_size;
    char  *row;
    char  header_buf[HEADERSIZE];
    FILE  *fp;
    Image *img;

    if((fp = fopen(filename, "rb")) == NULL){
        fprintf(stderr, "エラー: %s ファイルが開けません", filename);
        exit(1);
    }

    fread(header_buf, sizeof(char), HEADERSIZE, fp);

    if(strncmp(header_buf, "BM", 2)){
        fclose(fp);
        fprintf(stderr, "エラー: %s はbitmapではありません", filename);
        exit(1);
    }

    memcpy(&width, header_buf + 18, sizeof(width));
    memcpy(&height, header_buf + 22, sizeof(height));
    memcpy(&color, header_buf + 28, sizeof(color));

    if (color != 24) {
        fclose(fp);
        fprintf(stderr, "エラー: 24bitカラーのbitmapのみ対応しています\n");
        exit(1);
    }

    if((img = initImage(width, height, color, "BM")) == NULL){
        fclose(fp);
        fprintf(stderr, "エラー: 画像の初期化に失敗しました\n");
        exit(1);
    }

    row_size = width*3 + width%4;

    if((row = ( char *)malloc(sizeof( char) * row_size)) == NULL){
        fprintf(stderr, "エラー: メモリの初期化に失敗しました\n");
        return NULL;
    }

    for(i=0; i<height; i++){
        fread(row, 1, row_size, fp);
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

void write_image(char *filename, Image *img)
{
    int  i, j;
    int  row_size;
    int  width, height;
    char *row;
    char header_buf[HEADERSIZE] = {0};
    FILE *fp;
    
    int file_size, offset, planes, color;
    long info_header_size, data_size;


    if((fp = fopen(filename, "wb")) == NULL){
        fprintf(stderr, "エラー: %s ファイルが開けません", filename);
        exit(1);
    }

    width  = img->width;
    height = img->height;
    color  = img->color;

    row_size         = width*3 + width%4;
    data_size        = height * row_size;
    file_size        = height * row_size + HEADERSIZE;
    offset           = HEADERSIZE;
    info_header_size = INFOHEADERSIZE;
    planes           = 1;


    // 情報ヘッダの作成
    strcpy(header_buf, "BM");
    memcpy(header_buf + 2, &file_size, sizeof(file_size));
    memcpy(header_buf + 10, &offset, sizeof(offset));

    // ファイルヘッダの作成
    memcpy(header_buf + 14, &info_header_size, sizeof(info_header_size));
    memcpy(header_buf + 18, &width, sizeof(width));
    memcpy(header_buf + 22, &height, sizeof(height));
    memcpy(header_buf + 26, &planes, sizeof(planes));
    memcpy(header_buf + 28, &color, sizeof(color));
    memcpy(header_buf + 34, &data_size, sizeof(data_size));


    fwrite(header_buf, sizeof( char), HEADERSIZE, fp);

    if((row = (char *)malloc(sizeof(char)*row_size)) == NULL){
        fprintf(stderr, "エラー: メモリの初期化に失敗しました\n");
        fclose(fp);
        exit(1);
    }

    for(i=0; i<height; i++){

        for(j=0; j<width; j++){
            row[j*3]     = img->pRgb[i*width + j].b;
            row[j*3 + 1] = img->pRgb[i*width + j].g;
            row[j*3 + 2] = img->pRgb[i*width + j].r;
        }

        for(j=width*3; j<row_size; j++){
            row[j] = 0;
        }

        fwrite(row, sizeof(char), row_size, fp);
    }

    free(row);

    fclose(fp);
}
