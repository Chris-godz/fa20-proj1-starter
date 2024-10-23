/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color** colors = image->image;
	Color*  color = colors[col+ row * (image->rows)];
	Color* ret = (Color* )malloc(sizeof(Color));
	ret->R = ret->G = ret->B = (uint8_t) ((color->B & 1)  == 0 ? 0 : 255); 
	return ret;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	int rows = image->rows , cols= image-> cols;
	Image* ret = (Image* )malloc(sizeof (struct Image));
	ret->image = (Color**) malloc(sizeof(void*) * rows * cols );
	ret->rows = rows; ret->cols =cols;
	for(int i = 0 ;i < rows ;i++)
		for(int j = 0 ; j< cols ; j++){
			ret->image[j+ i * rows] = evaluateOnePixel(image,i,j);}
	return ret;
}	

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if(argc < 2){  perror("argc < 2\n"); exit(-1) ; }
	char* filename = argv[1];
	Image* oriImage  = readData(filename);
	Image* transImage = steganography(oriImage);
	writeData(transImage);
	Color** colors = transImage->image;
	for(int i = 0 ;i < transImage->rows; i++)
		for(int j = 0; j< transImage->cols ;j++)
		{
			struct Color * color = colors[j + i * transImage->cols];
			free(color);
		}
	colors = oriImage->image;
	for(int i = 0 ;i < oriImage->rows; i++)
		for(int j = 0; j< oriImage->cols ;j++)
		{
			struct Color * color = colors[j + i * oriImage->cols];
			free(color);
		}
	free(transImage->image);
	free(oriImage ->image);
	free(transImage);
	free(oriImage);
	return 0;
}
