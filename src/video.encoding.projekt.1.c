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
#include <string.h>
#include <time.h>
#include <math.h>


double generateGaussianNoise(double mean, double stdDev);

int main(void) {
	puts("This file reads FOOTBALL_352x288_30_orig_01.yuv");
	FILE *input;
	long input_size;
	input = fopen("FOOTBALL_352x288_30_orig_01.yuv", "r");
	unsigned char *buffer;
	size_t result;

	if (input == NULL) {
		perror("Error opening file");
		exit(1);
	} else {
		fseek(input, 0, SEEK_END);
		input_size = ftell(input);
		rewind(input);

		// allocate memory to contain the whole file
		buffer = (unsigned char*) malloc(sizeof(unsigned char) * input_size);
		if (buffer == NULL) {
			fputs("Couldn't allocate buffer memory", stderr);
			exit(2);
		}

		// copy the file into the buffer
		result = fread(buffer, 1, input_size, input);
		if (result != input_size) {
			fputs("Reading error", stderr);
			exit(3);
		}

//		fread(buffer, input_size, 1, input); // Read in the entire file
		fclose(input);
		float frame_size = 352 * 288 * 3 / 2;
		float frames = input_size / frame_size;

		printf("file size: %ld\n", input_size);
		printf("frames: %f\n", frames);

		FILE *output;
		output = fopen("vid-with-noise.yuv", "w");
		if (output == NULL) {
			fputs("Error creating new file", stderr);
			exit(1);
		}

		int frame_size_int = frame_size;

		int j;
		for (j = 0; j < frames; ++j) {

			unsigned char frame[frame_size_int];

			memcpy(frame, buffer+j*frame_size_int, frame_size_int);

			int i;
			for (i = 0; i < frame_size_int; i++) {
				unsigned int noise = round(generateGaussianNoise(0.0, 40.0));
				fprintf(output, "%c", (frame[i]+(unsigned char)noise));
			}
		}
		fclose(output);

		free(buffer);
	}
	printf("We are done with the job");
	return EXIT_SUCCESS;
}

double generateGaussianNoise(double mean, double stdDev) {
	int hasSpare = 0;
  	double spare = 0.0;

  	if(hasSpare) {
  		hasSpare = 0;
  		return mean + stdDev * spare;
  	}

 	hasSpare = 1;
 	double u = 0.0;
 	double v = 0.0;
 	double s = 0.0;
 	do {
 		u = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
 		v = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
 		s = u * u + v * v;
 	}
 	while( (s >= 1.0) || (s == 0.0) );

 	s = sqrt(-2.0 * log(s) / s);
 	return mean + stdDev * u * s;
}
