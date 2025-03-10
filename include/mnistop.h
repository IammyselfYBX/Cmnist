#ifndef __MNISTOP_H__
#define __MNISTOP_H__

#include <stdio.h>
#include <stdlib.h>
// MNIST数据集结构体
typedef struct {
    unsigned char* images;  // 图像数据
    unsigned char* labels;  // 标签数据
    int num_images;         // 图像数量
    int num_labels;         // 标签数量
    int rows;               // 图像行数
    int cols;               // 图像列数
    int current_index;      // 当前索引，用于迭代
} MNISTDataset;

#include <stdint.h>
#include <stdbool.h>

uint32_t convert_endian(uint32_t value);// 大端转小端函数（可复用）
void download_mnist();
void read_images(const char *file_path, unsigned char **images, int *num_images, int *rows, int *cols);
void read_labels(const char *file_path, unsigned char **labels, int *num_labels);
void print_image(unsigned char *images, int index, int rows, int cols);
MNISTDataset* load_mnist_dataset(const char* images_path, const char* labels_path);// 加载MNIST数据集
bool get_next_batch(MNISTDataset* dataset, unsigned char** batch_images, unsigned char** batch_labels, int batch_size); // 获取下一个批次的数据
void free_mnist_dataset(MNISTDataset* dataset); // 释放MNIST数据集

#endif // __MNISTOP_H__