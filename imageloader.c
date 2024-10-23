/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE * fp ;
	if((fp = fopen(filename,"r") ) == NULL)
	{
		perror("fopen\n");
		exit(-1);
	}
	char op[2];int rows,cols,Maxp;
	fscanf(fp,"%s%d%d%d", op,&rows,&cols,&Maxp);
	if(op[0] != 'P' || op[1] != '3' )
	{
		perror("ppm error\n");
		exit(-1);
	}
	Image* ret = (Image* ) malloc(sizeof (struct Image));
	Color** images = (Color**)malloc(sizeof(void*)* rows * cols);
	for(int i = 0 ;i < rows  ; i++)
		for(int j = 0; j<cols ;	j++)
		{
			int r,g,b;
			fscanf(fp,"%d%d%d",&r,&g,&b);
			if(0<=r && r <= 255 && 0<=g && g<=255 && 0<= b && b<=255)
			{
				Color* color = (Color *)malloc(sizeof(struct Color)); 
				color->R = (uint8_t)r;
				color->G = (uint8_t)g;
				color->B = (uint8_t)b; 
			 	images[j + i * cols ] = color;	
			}
			else perror("cur range out\n");
		}
	ret->image = images;
	ret->rows = rows;
	ret->cols = cols;
	return ret;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	uint8_t rows = image->rows;
	uint8_t cols = image->cols;
	Color** colors = image-> image;
	printf("%s\n%d %d\n%d\n","P3",rows,cols,255);
	for(int i = 0 ; i<rows; i++)
	{
		for(int j = 0 ; j<cols ;j++)
		{
			struct Color * color = colors[j + i * cols];
			printf("%3u %3u %3u ",color->R,color->G,color->B);
		} 
		putc('\n',stdout);
	}
}

//Frees an image
void freeImage(Image *image)
{
	Color** colors = image->image;
	int rows = image -> rows;
	int cols = image -> cols;
	for(int i = 0 ;i < rows; i++)
		for(int j = 0; j< cols ;j++)
		{
			struct Color * color = colors[j+ i*cols];
			free(color);
		}
	free(image);
}
	