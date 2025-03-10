#include "mnistop.h"
#include "string.h"

// 大端转小端函数（可复用）
uint32_t convert_endian(uint32_t value) {
    return __builtin_bswap32(value);
}

void read_images(const char *file_path, unsigned char **images, int *num_images, int *rows, int *cols) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("无法打开文件");
        exit(EXIT_FAILURE);
    }

    unsigned int magic, num;
    fread(&magic, sizeof(magic), 1, file);
    magic = __builtin_bswap32(magic); // 大端转小端
    if (magic != 2051) {
        fprintf(stderr, "魔数不匹配，期望2051，实际%u\n", magic);
        exit(EXIT_FAILURE);
    }

    fread(&num, sizeof(num), 1, file);
    *num_images = convert_endian(num);
    
    fread(rows, sizeof(*rows), 1, file);
    *rows = convert_endian(*rows);
    
    fread(cols, sizeof(*cols), 1, file);
    *cols = convert_endian(*cols);

    size_t image_size = *num_images * *rows * *cols;
    *images = (unsigned char *)malloc(image_size * sizeof(unsigned char));
    if (!*images) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }

    size_t read_count = fread(*images, sizeof(unsigned char), image_size, file);
    if (read_count != image_size) {
        fprintf(stderr, "读取图像失败，期望%ld字节，实际%zu字节\n", image_size, read_count);
        free(*images);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

void read_labels(const char *file_path, unsigned char **labels, int *num_labels) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        fprintf(stderr, "Error: 无法打开%s\n", file_path);
        exit(EXIT_FAILURE);
    }

    unsigned int magic, num;
    fread(&magic, sizeof(magic), 1, file);
    magic = convert_endian(magic);
    if (magic != 2049) {
        fprintf(stderr, "魔数不匹配，期望2049，实际%u\n", magic);
        exit(EXIT_FAILURE);
    }

    fread(&num, sizeof(num), 1, file);
    *num_labels = __builtin_bswap32(num);

    *labels = (unsigned char *)malloc(*num_labels * sizeof(unsigned char));
    if (!*labels) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }

    size_t read_count = fread(*labels, sizeof(unsigned char), *num_labels, file);
    if (read_count != *num_labels) {
        fprintf(stderr, "读取标签失败，期望%d字节，实际%zu字节\n", *num_labels, read_count);
        free(*labels);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void print_image(unsigned char *images, int index, int rows, int cols) {
    printf("Image %d:\n", index);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            unsigned char pixel = images[index * rows * cols + i * cols + j];
            // if (pixel > 127) {
            //     printf(" ");
            // } else {
            //     printf("#");
            // }
            if (pixel > 200) { // 根据像素值不同使用不同的字符表示
                printf("  ");       // 白色区域
            } else if (pixel > 100) {
                printf("░░");       // 灰色区域
            } else if (pixel > 50) {
                printf("▒▒");       // 深灰色区域
            } else {
                printf("██");       // 黑色区域
            }
        }
        printf("\n");
    }
}

// 加载MNIST数据集
MNISTDataset* load_mnist_dataset(const char* images_path, const char* labels_path) {
    MNISTDataset* dataset = malloc(sizeof(MNISTDataset));
    if (!dataset) {
        perror("Memory allocation failed for dataset");
        return NULL;
    }

    read_images(images_path, &dataset->images, &dataset->num_images, &dataset->rows, &dataset->cols);
    read_labels(labels_path, &dataset->labels, &dataset->num_labels);

    // 验证图像和标签数量是否匹配
    if (dataset->num_images != dataset->num_labels) {
        fprintf(stderr, "Mismatch: Number of images (%d) and labels (%d) do not match\n", 
                dataset->num_images, dataset->num_labels);
        free(dataset->images);
        free(dataset->labels);
        free(dataset);
        return NULL;
    }

    dataset->current_index = 0;
    return dataset;
}

// 获取下一个批次的数据
bool get_next_batch(MNISTDataset* dataset, unsigned char** batch_images, unsigned char** batch_labels, int batch_size) {
    if (!dataset || dataset->current_index >= dataset->num_images) {
        return false;
    }

    int remaining = dataset->num_images - dataset->current_index;
    int actual_batch_size = (batch_size < remaining) ? batch_size : remaining;

    *batch_images = malloc(actual_batch_size * dataset->rows * dataset->cols * sizeof(unsigned char));
    *batch_labels = malloc(actual_batch_size * sizeof(unsigned char));

    if (!*batch_images || !*batch_labels) {
        perror("Memory allocation failed for batch");
        return false;
    }

    memcpy(*batch_images, 
           dataset->images + dataset->current_index * dataset->rows * dataset->cols, 
           actual_batch_size * dataset->rows * dataset->cols * sizeof(unsigned char));
    
    memcpy(*batch_labels, 
           dataset->labels + dataset->current_index, 
           actual_batch_size * sizeof(unsigned char));

    dataset->current_index += actual_batch_size;
    return true;
}

// 释放数据集资源
void free_mnist_dataset(MNISTDataset* dataset) {
    if (dataset) {
        free(dataset->images);
        free(dataset->labels);
        free(dataset);
    }
}