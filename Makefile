affine_bitmap: main.c bitmap.o affine.o img_obj.o
	gcc main.c bitmap.o affine.o img_obj.o -o affine_bitmap

affine_pnm: main.c pnm.o affine.o img_obj.o
	gcc main.c pnm.o affine.o img_obj.o -o affine_pnm

affine.o: affine.c
	gcc -c affine.c

img_obj.o: img_obj.c
	gcc -c img_obj.c

bitmap.o: bitmap.c
	gcc -c bitmap.c

pnm.o: pnm.c
	gcc -c pnm.c

clean:
	rm *.o
	rm affine_bitmap
	rm affine_pnm