affine_bitmap: main.c bitmap.o affine.o img_obj.o
	gcc main.c bitmap.o affine.o img_obj.o -o affine_bitmap

affine_ppm: main.c ppm.o affine.o img_obj.o
	gcc main.c ppm.o affine.o img_obj.o -o affine_ppm

affine.o: affine.c
	gcc -c affine.c

img_obj.o: img_obj.c
	gcc -c img_obj.c

bitmap.o: bitmap.c
	gcc -c bitmap.c

ppm.o: ppm.c
	gcc -c ppm.c

clean:
	rm *.o
	rm affine_bitmap
	rm affine_ppm