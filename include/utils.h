#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// 输入验证函数
int isValidNodeId(int nodeId, int maxNodes);
int isValidString(const char* str);
int isValidCoordinate(float coord);

// 字符串处理函数
void trimString(char* str);
void toLowerCase(char* str);
int stringCompareIgnoreCase(const char* str1, const char* str2);

// 数学工具函数
float calculateDistance(float lat1, float lng1, float lat2, float lng2);
int minInt(int a, int b);
int maxInt(int a, int b);

// 内存管理工具
void* safeMalloc(size_t size);
void* safeRealloc(void* ptr, size_t size);

// 错误处理
typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_INPUT,
    ERROR_NODE_NOT_FOUND,
    ERROR_EDGE_NOT_FOUND,
    ERROR_MEMORY_ALLOCATION,
    ERROR_GRAPH_FULL,
    ERROR_PATH_NOT_FOUND
} ErrorCode;

const char* getErrorMessage(ErrorCode error);
void printError(ErrorCode error);

#endif // UTILS_H