

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//zos include
#include "zos_errors.h"
#include "zos_vfs.h"
#include "zos_sys.h"
#include "zos_video.h"

//函数声明
void randArray(int a[], int n);
void rotateMatrix(int data[4][4], int count);
void copyArray(int data[4][4], int src[4][4]);

//全局变量定义
int data[4][4];

//初始化数据
void init(int data[4][4], int initNum) {
	int random[4];
	randArray(random, 4);
	if (random[0] == random[2] && random[1] == random[3]) {
		init(data, 2);//若坐标相同,则重新生成坐标
	} else {
		data[random[0]][random[1]] = initNum;
		data[random[2]][random[3]] = initNum;
	}
}

//紧凑数组(向左就凑)
void compact(int data[4]) {
	int i, j = 0;
	for (i = 0; i < 4; i++) {
		if (data[i] != 0) {
			data[j] = data[i];
			j++;
		}
	}
	for (; j < 4; j++)data[j] = 0;
}

//向左合并
void mergerLeft(int data[4][4]) {
	for (int i = 0; i < 4; i++) {
		compact(data[i]);
		for (int j = 0; j < 3; j++) {
			if (data[i][j] == data[i][j + 1]) {
				data[i][j] = 2 * data[i][j];
				data[i][j + 1] = 0;
				compact(data[i]);
			}
		}
	}
}

//向右合并
void mergerRight(int data[4][4]) {
	rotateMatrix(data, 2);
	mergerLeft(data);
	rotateMatrix(data, 2);
}

//向上合并
void mergerUp(int data[4][4]) {
	rotateMatrix(data, 3);
	mergerLeft(data);
	rotateMatrix(data, 1);
}

//向下合并
void mergerDown(int data[4][4]) {
	rotateMatrix(data, 1);
	mergerLeft(data);
	rotateMatrix(data, 3);
}

//生成新数字
void newNum(int data[4][4], int initNum) {
	int random[2];
	while (1) {
		int flag = 1;
		randArray(random, 2);//随即产生坐标
		for (int i = 0; i < 4; i++) {//检测坐标是否重复
			for (int j = 0; j < 4; j++) {
				if (data[i][j] != 0 && i == random[0] && j == random[1]) {
					flag = 0;
				}
			}
		}
		if (flag)break;
	}
	data[random[0]][random[1]] = initNum;
}

//获取随机数
void randArray(int a[], int n) {
	for (int i = 0; i < n; i++) {//产生随机坐标
		srand((unsigned)time(NULL) + rand() + i);
		a[i] = rand() % 4;
	}
}

//显示界面
void show(int data[4][4]) {
	printf("\n2048-zos");
	for (int i = 0; i < 4; i++) {
		printf("\n|-----|-----|-----|-----|\n|");
		for (int j = 0; j < 4; j++) {
			if (data[i][j] == 0) {
				printf("    *|");
			} else {
				printf("%5d|", data[i][j]);
			}
		}
	}
	printf("\n|-----|-----|-----|-----|\n");
}

//拷贝数组
void copyArray(int data[4][4], int src[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			data[i][j] = src[i][j];
		}
	}
}

//旋转矩阵
void rotateMatrix(int data[4][4], int count) {
	int temp[4][4];
	copyArray(temp, data);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			data[i][j] = temp[4 - j - 1][i];
		}
	}
	if (count > 1)rotateMatrix(data, count-1);
}

//保存文件
void saveFile() {
	zos_dev_t dev = open("B:/2048.dat", O_WRONLY | O_CREAT | O_TRUNC);
	if (dev == NULL) {
		printf("\nUnable to save game\n");
		return;
	}
	else if (dev < 0) {
        printf("Error %d occured3\n", dev);
	}
	char *buffer = malloc(64); // 用malloc()函数分配一块内存空间，并用一个指针变量来指向它
	int index = 0; // 定义一个索引变量来记录buffer中的位置
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sprintf(buffer + index, "%10d ", data[i][j]); // 用sprintf()函数将数据转换为字符串，并写入到buffer中
			index += 11; // 更新索引，跳过空格
		}
		sprintf(buffer + index, "\n"); // 写入换行符
		index += 1; // 更新索引，跳过换行符
	}
	uint16_t size = index; // 定义一个变量来记录写入的字节数
	write(dev, buffer, &size); // 向文件中写入buffer中的内容
	free(buffer); // 用free()函数释放内存空间
	close(dev);
}

//读取文件
void readFile() {
    zos_dev_t dev2 = open("B:/2048.dat", O_RDONLY);
	if (dev2 == NULL) {
		printf("\nNo game save, starting a new game...\n");
		init(data, 2);
		return;
	}
	else if (dev2 < 0) {
        printf("\nNo game save, starting a new game...\n");
		init(data, 2);
		return;
	}
	char *buffer = malloc(256); // 用malloc()函数分配一块内存空间，并用一个指针变量来指向它
	uint16_t size = 256; // 定义一个变量来记录读取的字节数
	read(dev2, buffer, &size); // 从文件中读取64个字节到buffer中
	int index = 0; // 定义一个索引变量来记录buffer中的位置
	char *endptr; // 定义一个指针变量来记录字符串的结束位置
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			data[i][j] = strtol(buffer + index, &endptr, 10); // 用strtol()函数将字符串转换为长整型，并更新结束位置
			index = endptr - buffer + 1; // 更新索引，跳过空格
		}
		index += 1; // 更新索引，跳过换行符
	}
	free(buffer); // 用free()函数释放内存空间
	close(dev2);
}

int main() {
	int ch;//方向

    // system("clear");
	ioctl(DEV_STDOUT, CMD_CLEAR_SCREEN, NULL);
	printf("1: New Game\n");
	printf("2: Load Game\n");
	ch = getchar();
	if(ch == '1'){
    	// system("clear");
		ioctl(DEV_STDOUT, CMD_CLEAR_SCREEN, NULL);
		init(data, 2);
	} else {
		ioctl(DEV_STDOUT, CMD_CLEAR_SCREEN, NULL);
		readFile();
	}
	getchar(); // 读取回车键

	while (1) {
		show(data);
		ch = getchar();
		getchar(); // 读取回车键
		if (ch == 'Q') {
			break;
		}
		switch (ch) {
			case 'w': // Up
				mergerUp(data);
				newNum(data, 2);
				break;
			case 'd': // Right
				mergerRight(data);
				newNum(data, 2);
				break;
			case 's': // Down
				mergerDown(data);
				newNum(data, 2);
				break;
			case 'a': // Left
				mergerLeft(data);
				newNum(data, 2);
				break;
			case 'S': // Save
				saveFile();
				return 0;
		}
		saveFile();
		ioctl(DEV_STDOUT, CMD_CLEAR_SCREEN, NULL);
	}
	return 0;
}
