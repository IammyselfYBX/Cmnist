#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <zlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// #define TRAIN_IMAGES_URL "http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz"
// #define TRAIN_LABELS_URL "http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz"
// #define TEST_IMAGES_URL "http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz"
// #define TEST_LABELS_URL "http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz"
#define TRAIN_IMAGES_URL "https://gitee.com/iammyself001/Data/raw/master/MNIST/train-images-idx3-ubyte.gz"
#define TRAIN_LABELS_URL "https://gitee.com/iammyself001/Data/raw/master/MNIST/train-labels-idx1-ubyte.gz"
#define TEST_IMAGES_URL "https://gitee.com/iammyself001/Data/raw/master/MNIST/t10k-images-idx3-ubyte.gz"
#define TEST_LABELS_URL "https://gitee.com/iammyself001/Data/raw/master/MNIST/t10k-labels-idx1-ubyte.gz"

#define TRAIN_IMAGES_FILE "./data/mnist/train-images-idx3-ubyte.gz"
#define TRAIN_LABELS_FILE "./data/mnist/train-labels-idx1-ubyte.gz"
#define TEST_IMAGES_FILE "./data/mnist/t10k-images-idx3-ubyte.gz"
#define TEST_LABELS_FILE "./data/mnist/t10k-labels-idx1-ubyte.gz"

#define TRAIN_IMAGES_UNCOMPRESSED "./data/mnist/train-images-idx3-ubyte"
#define TRAIN_LABELS_UNCOMPRESSED "./data/mnist/train-labels-idx1-ubyte"
#define TEST_IMAGES_UNCOMPRESSED "./data/mnist/t10k-images-idx3-ubyte"
#define TEST_LABELS_UNCOMPRESSED "./data/mnist/t10k-labels-idx1-ubyte"

// 回调函数，用于写入下载的数据
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

// 下载文件
void download_file(const char *url, const char *filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        fp = fopen(filename, "wb");
        if (fp == NULL) {
            perror("无法打开文件");
            exit(EXIT_FAILURE);
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() 失败: %s\n", curl_easy_strerror(res));
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

// 解压.gz文件
void gunzip_file(const char *gz_filename, const char *out_filename) {
    gzFile gzfile = gzopen(gz_filename, "rb");
    if (!gzfile) {
        perror("无法打开.gz文件");
        exit(EXIT_FAILURE);
    }

    FILE *out_file = fopen(out_filename, "wb");
    if (!out_file) {
        perror("无法打开输出文件");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int num_read;
    while ((num_read = gzread(gzfile, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, num_read, out_file);
    }

    gzclose(gzfile);
    fclose(out_file);
}

// 检查文件是否存在
int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// 创建目录
void create_directory(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0777);
    }
}

void download_mnist() {
    // 创建必要的目录
    create_directory("./data");
    create_directory("./data/mnist");

    // 检查文件是否存在，如果不存在则下载
    if (!file_exists(TRAIN_IMAGES_FILE)) {
        printf("下载 %s\n", TRAIN_IMAGES_FILE);
        download_file(TRAIN_IMAGES_URL, TRAIN_IMAGES_FILE);
    } else {
        printf("%s 已存在，跳过下载。\n", TRAIN_IMAGES_FILE);
    }

    if (!file_exists(TRAIN_LABELS_FILE)) {
        printf("下载 %s\n", TRAIN_LABELS_FILE);
        download_file(TRAIN_LABELS_URL, TRAIN_LABELS_FILE);
    } else {
        printf("%s 已存在，跳过下载。\n", TRAIN_LABELS_FILE);
    }

    if (!file_exists(TEST_IMAGES_FILE)) {
        printf("下载 %s\n", TEST_IMAGES_FILE);
        download_file(TEST_IMAGES_URL, TEST_IMAGES_FILE);
    } else {
        printf("%s 已存在，跳过下载。\n", TEST_IMAGES_FILE);
    }

    if (!file_exists(TEST_LABELS_FILE)) {
        printf("下载 %s\n", TEST_LABELS_URL);
        download_file(TEST_LABELS_URL, TEST_LABELS_FILE);
    } else {
        printf("%s 已存在，跳过下载。\n", TEST_LABELS_FILE);
    }

    // 解压文件
    if (!file_exists(TRAIN_IMAGES_UNCOMPRESSED)) {
        printf("解压 %s\n", TRAIN_IMAGES_FILE);
        gunzip_file(TRAIN_IMAGES_FILE, TRAIN_IMAGES_UNCOMPRESSED);
    } else {
        printf("%s 已存在，跳过解压。\n", TRAIN_IMAGES_UNCOMPRESSED);
    }

    if (!file_exists(TRAIN_LABELS_UNCOMPRESSED)) {
        printf("解压 %s\n", TRAIN_LABELS_FILE);
        gunzip_file(TRAIN_LABELS_FILE, TRAIN_LABELS_UNCOMPRESSED);
    } else {
        printf("%s 已存在，跳过解压。\n", TRAIN_LABELS_UNCOMPRESSED);
    }

    if (!file_exists(TEST_IMAGES_UNCOMPRESSED)) {
        printf("解压 %s\n", TEST_IMAGES_FILE);
        gunzip_file(TEST_IMAGES_FILE, TEST_IMAGES_UNCOMPRESSED);
    } else {
        printf("%s 已存在，跳过解压。\n", TEST_IMAGES_UNCOMPRESSED);
    }

    if (!file_exists(TEST_LABELS_UNCOMPRESSED)) {
        printf("解压 %s\n", TEST_LABELS_FILE);
        gunzip_file(TEST_LABELS_FILE, TEST_LABELS_UNCOMPRESSED);
    } else {
        printf("%s 已存在，跳过解压。\n", TEST_LABELS_UNCOMPRESSED);
    }

    printf("下载和解压完成。\n");
}


