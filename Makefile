affine_bitmap: main.c bitmap.o affine.o img.o
	gcc main.c bitmap.o affine.o img.o -o affine_bitmap

affine_pnm: main.c pnm.o affine.o img.o
	gcc main.c pnm.o affine.o img.o -o affine_pnm

affine.o: affine.c
	gcc -c affine.c

img.o: img.c
	gcc -c img.c

bitmap.o: bitmap.c
	gcc -c bitmap.c

pnm.o: pnm.c
	gcc -c pnm.c

clean:
	rm *.o
	rm affine_bitmap
	rm affine_pnm