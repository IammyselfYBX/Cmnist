#ifndef __MNISTOP_H__
#define __MNISTOP_H__

#include <stdio.h>
#include <stdlib.h>
void download_mnist();
void read_images(const char *file_path, unsigned char **images, int *num_images, int *rows, int *cols);
void read_labels(const char *file_path, unsigned char **labels, int *num_labels);
void print_image(unsigned char *images, int index, int rows, int cols);

#endif // __MNISTOP_H__