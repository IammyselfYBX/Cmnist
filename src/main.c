#include <stdio.h>
#include "mnistop.h"

#define TEST_IMAGES_FILE "./data/mnist/t10k-images-idx3-ubyte"
#define TEST_LABELS_FILE "./data/mnist/t10k-labels-idx1-ubyte"
#define TRAIN_IMAGES_FILE "./data/mnist/train-images-idx3-ubyte"
#define TRAIN_LABELS_FILE "./data/mnist/train-labels-idx1-ubyte"

void print_mnist_dataset(const char* images_file, const char* labels_file, const char* dataset_name) {
    unsigned char *images = NULL;
    unsigned char *labels = NULL;
    int num_images, rows, cols, num_labels;

    printf("\n\033[0;34m\033[1m========================================\033[0m\n");
    printf("\033[0;34m\033[1m读取%s...\033[0m\n", dataset_name);

    read_images(images_file, &images, &num_images, &rows, &cols);
    read_labels(labels_file, &labels, &num_labels);

    printf("%s图像数量: %d\n", dataset_name, num_images);
    printf("%s标签数量: %d\n", dataset_name, num_labels);

    // 显示前5个图像及其标签
    for (int i = 0; i < 5; i++) {
        printf("Label: %d\n", labels[i]);
        print_image(images, i, rows, cols);
        printf("\n");
    }

    free(images);
    free(labels);
}

void process_batch_dataset(const char* images_file, const char* labels_file, const char* dataset_name) {
    printf("\n\033[0;34m\033[1m处理%s批次数据...\033[0m\n", dataset_name);

    MNISTDataset* dataset = load_mnist_dataset(images_file, labels_file);
    if (!dataset) {
        printf("加载%s失败\n", dataset_name);
        return;
    }

    unsigned char* batch_images;
    unsigned char* batch_labels;
    int batch_size = 2;  // 减少批次数量以便演示
    int batch_count = 0;

    while (get_next_batch(dataset, &batch_images, &batch_labels, batch_size)) {
        printf("Batch %d:\n", ++batch_count);
        for (int i = 0; i < batch_size; i++) {
            printf("Label: %d\n", batch_labels[i]);
            print_image(batch_images, i, dataset->rows, dataset->cols);
            printf("\n");
        }

        free(batch_images);
        free(batch_labels);

        if (batch_count >= 2) break;  // 限制处理2个批次
    }

    free_mnist_dataset(dataset);
}

int main() {
    download_mnist();
    // 打印测试集和训练集的前5个图像
    print_mnist_dataset(TEST_IMAGES_FILE, TEST_LABELS_FILE, "测试集");
    print_mnist_dataset(TRAIN_IMAGES_FILE, TRAIN_LABELS_FILE, "训练集");

    // 处理测试集和训练集的批次数据
    process_batch_dataset(TEST_IMAGES_FILE, TEST_LABELS_FILE, "测试集");
    process_batch_dataset(TRAIN_IMAGES_FILE, TRAIN_LABELS_FILE, "训练集");

    return 0;
}