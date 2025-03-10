#include "mnistop.h"

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
    *num_images = __builtin_bswap32(num);
    fread(rows, sizeof(*rows), 1, file);
    *rows = __builtin_bswap32(*rows);
    fread(cols, sizeof(*cols), 1, file);
    *cols = __builtin_bswap32(*cols);

    *images = (unsigned char *)malloc(*num_images * *rows * *cols * sizeof(unsigned char));
    if (!*images) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }

    fread(*images, sizeof(unsigned char), *num_images * *rows * *cols, file);
    fclose(file);
}

void read_labels(const char *file_path, unsigned char **labels, int *num_labels) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("无法打开文件");
        exit(EXIT_FAILURE);
    }

    unsigned int magic, num;
    fread(&magic, sizeof(magic), 1, file);
    magic = __builtin_bswap32(magic); // 大端转小端
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

    fread(*labels, sizeof(unsigned char), *num_labels, file);
    fclose(file);
}

void print_image(unsigned char *images, int index, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            unsigned char pixel = images[index * rows * cols + i * cols + j];
            if (pixel > 127) {
                printf(" ");
            } else {
                printf("#");
            }
        }
        printf("\n");
    }
}