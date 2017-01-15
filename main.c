#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include "img_obj.h"
#include "bitmap.h"
#include "ppm.h"
#include "affine.h"

void echoUsage(char *programName);

int main(int argc, char *argv[])
{


    Image *img;
    char *method;
    char *inputName, *outputName;
    char direction;
    int value;

    if ( (1<argc) && (strcmp(argv[1], "-help") == 0) ) {
        echoUsage(argv[0]);
        exit(0);
    }

    if (argc != 6) {
        fprintf(stderr, "引数が正しくありません\n");
        echoUsage(argv[0]);
        exit(1);
    }

    method     = argv[1];
    direction  = *argv[2];
    value      = atoi(argv[3]);
    inputName  = argv[4];
    outputName = argv[5];

    if ((img = read_image(inputName)) == NULL ) {
        exit(1);
    }


    if (strcmp(method, "scale") == 0) {

        if ((img = scale(img, direction, value)) == NULL) {
            exit(1);
        }

    } else if (strcmp(method, "move") == 0) {

        if ((img = move(img, direction, value)) == NULL) {
            exit(1);
        }
    } else if (strcmp(method, "rotation") == 0) {

        if ((img = rotation(img, direction, value)) == NULL) {
            exit(1);
        }

    } else if (strcmp(method, "skew") == 0) {

        if ((img = skew(img, direction, value)) == NULL) {
            exit(1);
        }

    } else {

        fprintf(stderr, "引数が正しくありません\n");
        exit(1);
    }

    if ((write_image(outputName, img)) == NULL) {
        exit(1);
    }

    freeImage(img);
}

void echoUsage(char *programName)
{

    fprintf(stderr, "使用方法\n");
    fprintf(stderr, "%s [メソッド] [方向] [大きさ] [入力ファイル名] [出力ファイル名]\n\n", programName);

    fprintf(stderr, "[メソッド] scale 画像を[方向]に拡大\n");
    fprintf(stderr, "          [方向]   x または　y　で指定\n");
    fprintf(stderr, "          [大きさ] 倍率を指定\n\n");

    fprintf(stderr, "[メソッド] move 画像を[方向]に平行移動\n");
    fprintf(stderr, "          [方向]   x または　y　で指定\n");
    fprintf(stderr, "          [大きさ] ピクセルを指定\n\n");

    fprintf(stderr, "[メソッド] rotation 画像を[方向]に回転\n");
    fprintf(stderr, "          [方向]   r(右) または　l(左)　で指定\n");
    fprintf(stderr, "          [大きさ] 角度を指定\n\n");

    fprintf(stderr, "[メソッド] skew 画像を[方向]にせん断\n");
    fprintf(stderr, "          [方向]   x または　y　で指定\n");
    fprintf(stderr, "          [大きさ] 角度を指定\n");

}

