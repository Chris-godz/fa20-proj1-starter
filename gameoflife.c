
/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"
#include <stdbool.h>
#include <ctype.h>
//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
int dx[8] = {1,1,1,-1,-1,-1,0,0};
int dy[8] = {1,0,-1,1,0,-1,1,-1};
int ring(int x , int y)
{
	return (x+y)%y;
}
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//uint32_t rule = (argv[2][0] -'0' ) +  (argv[2][1]-'0')*0x10 + ( argv[2][2]-'0' ) * 0x100  + ( argv[2][3] - '0') * 0x1000;
	Color** colors = image->image;
	int rows = image-> rows;
	int cols = image-> cols;
	int countr = 0 , countg =0 , countb = 0;
	Color* color = colors[col + row * cols];
	bool statR = color->R == 255,statG = color-> G == 255 , statB = color -> B == 255; 
	Color* tranColor = (Color*) malloc(sizeof (struct Color));

	for(int i = 0 ;i < 8  ; i++){
		int newcol = ring(dx[i] + col , cols);
		int newrow = ring(dy[i] + row , rows); 
		Color* scanColor = colors[newcol + newrow * cols];
		
		countr += scanColor->R == 255;
		countb += scanColor->B == 255;
		countg += scanColor->G == 255;
	}
	if(statR ) statR = (( 1 <<  (countr + 9)) & rule);
	else statR = (( 1 << countr) & rule);

	if(statG ) statG = ( 1 <<  (countg + 9)) & rule;
	else statG = (( 1 << countg) & rule);

	if(statB ) statB = ( 1 <<  (countb + 9)) & rule;
	else statB = (( 1 << countb) & rule);

	tranColor->R = (statR == true ? 255 : 0);
	tranColor->G = (statG == true ? 255 : 0);
	tranColor->B = (statB == true ? 255 : 0);
	return tranColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	int rows = image -> rows;int cols = image->cols;
	Image* transImage = (Image*) malloc(sizeof (struct Image));
	Color ** colors = (Color**)malloc(rows*cols*sizeof(void*));
	transImage->image = colors;
	transImage->rows =rows;
	transImage->cols =cols;
	for(int i = 0;i<rows ;i++)
		for(int j = 0; j<cols ;j++)
			{
				colors[j + cols * i] =  evaluateOneCell(image,i,j ,rule);
			}
	return transImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
static void errorp()
{
	perror("usage: ./gameOfLife filename rule\n\
    			filename is an ASCII PPM file (type P3) with maximum value 255.\n\
    			rule is a hex number beginning with 0x; Life is 0x1808.\n");
}
static void checkppm(FILE* fp )
{
	char op[2];
	int rows,cols,Maxp;
	if( fscanf(fp,"%s%d%d%d",op,&cols,&rows,&Maxp) != 4) {errorp();exit(EXIT_FAILURE);}
	if(op[0] != 'P' || op[1]!='3' || Maxp != 255)
	{
		{errorp();exit(EXIT_FAILURE);}
	}
}
int main(int argc, char **argv)
{
	char * filename = argv[1];
	if(argc != 3 || !isdigit(argv[2][5]) || !isdigit(argv[2][4]) || !isdigit(argv[2][3]) || !isdigit(argv[2][2]) ) 
	{
		errorp();
		exit(EXIT_FAILURE);
	}
	FILE* fp;
	if((fp = fopen(filename , "r")) == NULL) {perror("fopen\n");exit(EXIT_FAILURE);}
	checkppm(fp);
	fclose(fp);
	uint32_t rule = (argv[2][5] -'0' ) +  (argv[2][4]-'0')*0x10 + ( argv[2][3]-'0' ) * 0x100  + ( argv[2][2] - '0') * 0x1000;
	Image* oriImage =  readData(filename);
	Image* iterImage = life(oriImage,rule);
	writeData(iterImage);

	freeImage(oriImage);
	freeImage(iterImage);
	return 0;
}
