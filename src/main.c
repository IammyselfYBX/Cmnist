#include "stdio.h"  
#include "mnistop.h"
#define TEST_IMAGES_FILE "./data/mnist/t10k-images-idx3-ubyte"
#define TEST_LABELS_FILE "./data/mnist/t10k-labels-idx1-ubyte"
#define TRAIN_IMAGES_FILE "./data/mnist/train-images-idx3-ubyte"
#define TRAIN_LABELS_FILE "./data/mnist/train-labels-idx1-ubyte"


int main() {

    download_mnist();
    unsigned char *test_images = NULL;
    unsigned char *test_labels = NULL;
    int num_test_images, rows, cols, num_test_labels;

    printf("\n\033[0;34m\033[1m========================================\033[0m\n");
    printf("\033[0;34m\033[1m读取测试集...\033[0m\n");
    read_images(TEST_IMAGES_FILE, &test_images, &num_test_images, &rows, &cols);
    read_labels(TEST_LABELS_FILE, &test_labels, &num_test_labels);

    printf("测试集图像数量: %d\n", num_test_images);
    printf("测试集标签数量: %d\n", num_test_labels);

    // 显示前5个测试集图像及其标签
    for (int i = 0; i < 5; i++) {
        printf("Label: %d\n", test_labels[i]);
        print_image(test_images, i, rows, cols);
        printf("\n");
    }

    free(test_images);
    free(test_labels);

    printf("\n\033[0;34m\033[1m========================================\033[0m\n");
    printf("\033[0;34m\033[1m读取训练集...\033[0m\n");
    unsigned char *train_images = NULL;
    unsigned char *train_labels = NULL;
    int num_train_images, num_train_labels;

    read_images(TRAIN_IMAGES_FILE, &train_images, &num_train_images, &rows, &cols);
    read_labels(TRAIN_LABELS_FILE, &train_labels, &num_train_labels);

    printf("训练集图像数量: %d\n", num_train_images);
    printf("训练集标签数量: %d\n", num_train_labels);

    // 显示前5个训练集图像及其标签
    for (int i = 0; i < 5; i++) {
        printf("Label: %d\n", train_labels[i]);
        print_image(train_images, i, rows, cols);
        printf("\n");
    }

    free(train_images);
    free(train_labels);

    return 0;
}