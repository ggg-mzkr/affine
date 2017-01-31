//
// Created by 堀内哲煕 on 2016/12/31.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"
#include "pnm.h"


Image *readImage(char *filename)
{

    int i, j;
    char buf[256];
    char header[HEADER_SIZE_PPM];
    int width, height;
    bool isColor;
    FILE *fp;
    Image *img;

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "エラー: %s が読み取れません", filename);
        return NULL;
    }

    getNextToken(fp, buf);

    if (strcmp(buf, "P1") != 0 && strcmp(buf, "P2") != 0 && strcmp(buf, "P3") != 0) {
        fprintf(stderr, "エラー: %s はテキスト系式のPNMファイルではありません\n", filename);
        return NULL;
    }

    strcpy(header + FORMAT_OFFSET, buf);

    if (strcmp(buf, "P3") != 0) {
        isColor = false;
    } else {
        isColor = true;
    }

    getNextToken(fp, buf);
    width = atoi(buf);

    getNextToken(fp, buf);
    height = atoi(buf);

    getNextToken(fp, buf);
    strcpy(header + COLOR_OFFSET, buf);


    if ((img = initImage(width ,height, HEADER_SIZE_PPM, header, isColor)) == NULL) {
        fclose(fp);
        fprintf(stderr, "エラー: 画像の初期化に失敗しました\n");
        return NULL;
    }

    if (img->isColor) {
        for (i = height-1; i >= 0; i--) {
            for (j = 0; j < width; j++) {
                fscanf(fp, "%d %d %d", &img->pRgb[width * i + j].r, &img->pRgb[width * i + j].b, &img->pRgb[width * i + j].g);
            }
        }
    } else {
        for (i = height-1; i >= 0; i--) {
            for (j = 0; j < width; j++) {
                fscanf(fp, "%d", &img->pRgb[width * i + j].r);
            }
        }
    }

    fclose(fp);

    return img;
}

int *writeImage(char *filename, Image *img)
{

    int i, j;
    int width, height;
    FILE *fp;

    if ((fp = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "エラー: %s が読み取れません", filename);
        return NULL;
    }

    width = img->width;
    height = img->height;

    fprintf(fp, "%s\n", img->header + FORMAT_OFFSET);
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "%d\n", atoi(img->header + COLOR_OFFSET));

    if (img->isColor) {
        for (i = height-1; i >= 0; i--) {
            for (j = 0; j < width; j++) {
                fprintf(fp, "%d %d %d ", img->pRgb[width * i + j].r, img->pRgb[width * i + j].b, img->pRgb[width * i + j].g);
                if (j % 10 == 0) {
                    fprintf(fp, "\n");
                }
            }
        }
    } else {

        for (i = height-1; i >= 0; i--) {
            for (j = 0; j < width; j++) {
                fprintf(fp, "%d ", img->pRgb[width * i + j].r);
                if (j % 10 == 0) {
                    fprintf(fp, "\n");
                }
            }
        }
    }

    fclose(fp);

    return 0;

}

void getNextToken(FILE *fp, char *buf)
{

    while ( *buf != EOF) {
        fscanf(fp, "%s", buf);

        if (buf[0] == '#') {
            fgets(buf, 256, fp);

        } else {
            break;

        }
    }

}
