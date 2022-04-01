#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "tiler.c"
#include "quantize.c"


void gather_pixels(int xpos, int ypos, int width, int height, image input, unsigned char* redout, unsigned char* greenout, unsigned char* blueout){
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			redout[x+y*width]   = input->pix[(xpos + x + (ypos + y)*(input->w))*3 ];
			greenout[x+y*width] = input->pix[(xpos + x + (ypos + y)*(input->w))*3+1 ];
			blueout[x+y*width]  = input->pix[(xpos + x + (ypos + y)*(input->w))*3+2 ]; 
		}
	}
}

void make_top_left_block(image input, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*5*5);
	unsigned char *greenout = malloc(sizeof(char)*5*5);
	unsigned char *blueout = malloc(sizeof(char)*5*5);
	gather_pixels(0, 0, 5, 5, input, redout, greenout, blueout);
	unsigned char tile = find_tile_top_left(
		redout, greenout, blueout,
		output_black->pix,
		output_black->pix + 1,
		output_black->pix + 2,
		output_white->pix,
		output_white->pix + 1,
		output_white->pix + 2
		);
	fputc(tile, output_tiles);
}

void make_top_right_block(image input, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*5*5);
	unsigned char *greenout = malloc(sizeof(char)*5*5);
	unsigned char *blueout = malloc(sizeof(char)*5*5);
	gather_pixels(input->w-5, 0, 5, 5, input, redout, greenout, blueout);
	unsigned char tile = find_tile_top_right(
		redout, greenout, blueout,
		output_black->pix + output_black->w*3 - 3,
		output_black->pix + output_black->w*3 - 3 + 1,
		output_black->pix + output_black->w*3 - 3 + 2,
		output_white->pix + output_white->w*3 - 3,
		output_white->pix + output_white->w*3 - 3 + 1,
		output_white->pix + output_white->w*3 - 3 + 2
		);
	fputc(tile, output_tiles);
}

void make_bottom_left_block(image input, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*5*5);
	unsigned char *greenout = malloc(sizeof(char)*5*5);
	unsigned char *blueout = malloc(sizeof(char)*5*5);
	gather_pixels(0, input->h-5, 5, 5, input, redout, greenout, blueout);
	unsigned char tile = find_tile_bottom_left(
		redout, greenout, blueout,
		output_black->pix + output_black->w*output_black->h*3 - output_black->w*3,
		output_black->pix + output_black->w*output_black->h*3 - output_black->w*3 + 1,
		output_black->pix + output_black->w*output_black->h*3 - output_black->w*3 + 2,
		output_white->pix + output_white->w*output_white->h*3 - output_white->w*3,
		output_white->pix + output_white->w*output_white->h*3 - output_white->w*3 + 1,
		output_white->pix + output_white->w*output_white->h*3 - output_white->w*3 + 2
		);
	fputc(tile, output_tiles);
}

void make_bottom_right_block(image input, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*5*5);
	unsigned char *greenout = malloc(sizeof(char)*5*5);
	unsigned char *blueout = malloc(sizeof(char)*5*5);
	gather_pixels(input->w-5, input->h-5, 5, 5, input, redout, greenout, blueout);
	unsigned char tile = find_tile_bottom_right(
		redout, greenout, blueout,
		output_black->pix + output_black->w*output_black->h*3 - 3,
		output_black->pix + output_black->w*output_black->h*3 - 3 + 1,
		output_black->pix + output_black->w*output_black->h*3 - 3 + 2,
		output_white->pix + output_white->w*output_white->h*3 - 3,
		output_white->pix + output_white->w*output_white->h*3 - 3 + 1,
		output_white->pix + output_white->w*output_white->h*3 - 3 + 2
		);
	fputc(tile, output_tiles);
}

void make_top_block(image input, int x, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*6*5);
	unsigned char *greenout = malloc(sizeof(char)*6*5);
	unsigned char *blueout = malloc(sizeof(char)*6*5);
	gather_pixels(x*4 -1, 0, 6, 5, input, redout, greenout, blueout);
	unsigned char tile = find_tile_top(
		redout, greenout, blueout,
		output_black->pix + x*3,
		output_black->pix + x*3 + 1,
		output_black->pix + x*3 + 2,
		output_white->pix + x*3,
		output_white->pix + x*3 + 1,
		output_white->pix + x*3 + 2
		);
	fputc(tile, output_tiles);
}

void make_bottom_block(image input, int x, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*6*5);
	unsigned char *greenout = malloc(sizeof(char)*6*5);
	unsigned char *blueout = malloc(sizeof(char)*6*5);
	gather_pixels(x*4 -1, input->h-5, 6, 5, input, redout, greenout, blueout);
	unsigned char tile = find_tile_bottom(
		redout, greenout, blueout,
		output_black->pix + x*3 + output_black->w*output_black->h*3 - output_black->w*3,
		output_black->pix + x*3 + output_black->w*output_black->h*3 - output_black->w*3 + 1,
		output_black->pix + x*3 + output_black->w*output_black->h*3 - output_black->w*3 + 2,
		output_white->pix + x*3 + output_white->w*output_white->h*3 - output_white->w*3,
		output_white->pix + x*3 + output_white->w*output_white->h*3 - output_white->w*3 + 1,
		output_white->pix + x*3 + output_white->w*output_white->h*3 - output_white->w*3 + 2
		);
	fputc(tile, output_tiles);
}

void make_left_block(image input, int y, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*5*6);
	unsigned char *greenout = malloc(sizeof(char)*5*6);
	unsigned char *blueout = malloc(sizeof(char)*5*6);
	gather_pixels(0, y*4-1, 5, 6, input, redout, greenout, blueout);
	unsigned char tile = find_tile_left(
		redout, greenout, blueout,
		output_black->pix + y * output_black->w * 3,
		output_black->pix + y * output_black->w * 3 + 1,
		output_black->pix + y * output_black->w * 3 + 2,
		output_white->pix + y * output_white->w * 3,
		output_white->pix + y * output_white->w * 3 + 1,
		output_white->pix + y * output_white->w * 3 + 2
		);
	fputc(tile, output_tiles);
}

void make_right_block(image input, int y, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*5*6);
	unsigned char *greenout = malloc(sizeof(char)*5*6);
	unsigned char *blueout = malloc(sizeof(char)*5*6);
	gather_pixels(input->w, y*4-1, 5, 6, input, redout, greenout, blueout);
	unsigned char tile = find_tile_right(
		redout, greenout, blueout,
		output_black->pix + y * output_black->w * 3 + (output_black->w-1) * 3,
		output_black->pix + y * output_black->w * 3 + (output_black->w-1) * 3 + 1,
		output_black->pix + y * output_black->w * 3 + (output_black->w-1) * 3 + 2,
		output_white->pix + y * output_white->w * 3 + (output_white->w-1) * 3,
		output_white->pix + y * output_white->w * 3 + (output_white->w-1) * 3 + 1,
		output_white->pix + y * output_white->w * 3 + (output_white->w-1) * 3 + 2
		);
	fputc(tile, output_tiles);
}

void make_block(image input, int x, int y, image output_black, image output_white, FILE *output_tiles){
	unsigned char *redout = malloc(sizeof(char)*6*6);
	unsigned char *greenout = malloc(sizeof(char)*6*6);
	unsigned char *blueout = malloc(sizeof(char)*6*6);
	gather_pixels(x*4-1, y*4-1, 6, 6, input, redout, greenout, blueout);
	unsigned char tile = find_tile(
		redout, greenout, blueout,
		output_black->pix + x * 3 + y * output_black->w * 3,
		output_black->pix + x * 3 + y * output_black->w * 3 + 1,
		output_black->pix + x * 3 + y * output_black->w * 3 + 2,
		output_white->pix + x * 3 + y * output_white->w * 3,
		output_white->pix + x * 3 + y * output_white->w * 3 + 1,
		output_white->pix + x * 3 + y * output_white->w * 3 + 2
		);
	fputc(tile, output_tiles);
}


void make_blocks(image input, image output_black, image output_white, FILE *output_tiles){
	make_top_left_block(input, output_black, output_white, output_tiles);
	for(int x=1; x < (output_black->w) -2; x++){
		make_top_block(input, x, output_black, output_white, output_tiles);
	}
	make_top_right_block(input, output_black, output_white, output_tiles);
	
	for(int y=1; y < ((output_black->h) -1); y++){
		make_left_block(input, y, output_black, output_white, output_tiles);
		for(int x=1; x < ((output_black->w) -1); x++){
			make_block(input, x, y, output_black, output_white, output_tiles);
		}
		make_right_block(input, y, output_black, output_white, output_tiles);
	}
	
	make_bottom_left_block(input, output_black, output_white, output_tiles);
	for(int x=1; x < (output_black->w) -2; x++){
		make_bottom_block(input, x, output_black, output_white, output_tiles);
	}
	make_bottom_right_block(input, output_black, output_white, output_tiles);
}


int main(int c, char *v[]){
	if (c < 2) {
		fprintf(stderr, "usage: %s ppm_file\n", v[0]);
		return 0;
	}
 
	image input = read_ppm(v[1]);
	
	image output_black= img_new((input->w)>>2, (input->h)>>2);
	image output_white = img_new((input->w)>>2, (input->h)>>2);
	
    FILE *output_tiles = fopen("tiles.bin", "wb");
    
    make_blocks(input, output_black, output_white, output_tiles);

    fclose(output_tiles);
    
    image output = img_new((input->w)>>2, (input->h)>>1);
    int wh = (output->w)*((output->h)>>1)*3;
    for(int i=0; i<wh; i++)
    	output->pix[i] = output_black->pix[i]; 
    for(int i=0; i<wh; i++)
    	output->pix[wh+i] = output_white->pix[i]; 
    	
	free(output_black);
	free(output_white);
	
	color_quant(output, 120, 0);
	
	write_ppm(output, "out.pnm");
	free(output);
	free(input);
 
	return 0;
}
