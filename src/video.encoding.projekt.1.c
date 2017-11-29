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
void makeYUVFileName(char *file_name, int variance);

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
	}
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
	fclose(input);

	FILE *data;
	data = fopen("data.csv", "w");
	if (data == NULL) {
		fputs("Error creating data file", stderr);
		exit(1);
	}
	fprintf(data, "%s", "variance;psnr\n");

	float frame_size = 352 * 288 * 3 / 2;
	float frames = input_size / frame_size;

	printf("file size: %ld Bytes\n", input_size);
	printf("frames: %f\n", frames);

	int frame_size_int = frame_size;

	int variance;
	// shall be enough
	for (variance = 250; variance < 251; variance += 2) {
		// we need a video for each variance
		FILE *output;
		char file_name[100]; // should be enough
		makeYUVFileName(file_name, variance);
		output = fopen(file_name, "w");
		if (output == NULL) {
			fputs("Error creating new file", stderr);
			exit(1);
		}
		int j;
		double sum_psnr = 0.0;
		for (j = 0; j < frames; ++j) {
			unsigned char frame[frame_size_int];
			memcpy(frame, buffer + j * frame_size_int, frame_size_int);
			double square_error = 0.0;
			int i;
			for (i = 0; i < frame_size_int; i++) {
				unsigned int noise = round(generateGaussianNoise(0.0, sqrt((double) variance)));
				fprintf(output, "%c", (frame[i] + (unsigned char) noise));
				square_error += pow(abs(noise), 2); // pow doesnt't work with negative values???
			}
			double mean_square_error = square_error / frame_size;
			double psnr = 10 * log10(pow(255, 2) / mean_square_error);
			sum_psnr += psnr;
		}
		fclose(output);
		double mean_psnr = sum_psnr / frames;
		printf("variance: %d, mean_psnr: %.2f dB\n", variance, mean_psnr);
		char data_buffer[50];
		sprintf(data_buffer, "%d;%.2f\n", variance, mean_psnr);
		fprintf(data, "%s", data_buffer);
	}
	free(buffer); // we are done lets free the buffer
	fclose(data);

	printf("We are done with the job");
	return EXIT_SUCCESS;
}

// uses standard deviation, therefore we have to use the square root of the variance
double generateGaussianNoise(double mean, double stdDev) {
	int hasSpare = 0;
	double spare = 0.0;

	if (hasSpare) {
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
	} while ((s >= 1.0) || (s == 0.0));

	s = sqrt(-2.0 * log(s) / s);
	return mean + stdDev * u * s;
}

void makeYUVFileName(char *file_name, int variance) {
	char buffer[100];
	sprintf(buffer, "vid-with-noise-variance(%d)", variance);
	char format[] = ".yuv";
	strcpy(file_name, buffer);
	strcat(file_name, format);
}
