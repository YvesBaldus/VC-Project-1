/*
 ============================================================================
 Name        : video.encoding.projekt.1.c
 Author      : Yves Baldus
 Version     : 0.1
 Description : Reading FOOTBALL-352x288_30_orig_01.yuv
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	puts("This file reads FOOTBALL_352x288_30_orig_01.yuv"); /* prints !!!Hello World!!! */
	FILE *input;
	long input_size;

	input = fopen("FOOTBALL_352x288_30_orig_01.yuv", "r");

	if (input == NULL)
		perror("Error opening file");
	else {
		fseek(input, 0, SEEK_END);
		input_size = ftell(input);
		fclose(input);
		float frames = input_size / (352*288*3/2);
		printf("file size: %ld\n", input_size);
		printf("frames: %f", frames);
	}
	return EXIT_SUCCESS;
}
