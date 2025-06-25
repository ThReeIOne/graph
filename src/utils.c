#include "../include/utils.h"

// 输入验证函数
int isValidNodeId(int nodeId, int maxNodes) {
    return nodeId >= 0 && nodeId < maxNodes;
}

int isValidString(const char* str) {
    return str != NULL && strlen(str) > 0;
}

int isValidCoordinate(float coord) {
    return coord >= -180.0f && coord <= 180.0f;
}

// 字符串处理函数
void trimString(char* str) {
    if (!str) return;
    
    // 去除前导空格
    char* start = str;
    while (isspace(*start)) start++;
    
    // 去除尾部空格
    char* end = str + strlen(str) - 1;
    while (end > start && isspace(*end)) end--;
    
    // 移动字符串
    size_t len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}

void toLowerCase(char* str) {
    if (!str) return;
    
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int stringCompareIgnoreCase(const char* str1, const char* str2) {
    if (!str1 || !str2) return -1;
    
    char* temp1 = (char*)safeMalloc(strlen(str1) + 1);
    char* temp2 = (char*)safeMalloc(strlen(str2) + 1);
    
    if (!temp1 || !temp2) {
        free(temp1);
        free(temp2);
        return -1;
    }
    
    strcpy(temp1, str1);
    strcpy(temp2, str2);
    
    toLowerCase(temp1);
    toLowerCase(temp2);
    
    int result = strcmp(temp1, temp2);
    
    free(temp1);
    free(temp2);
    
    return result;
}

// 数学工具函数
float calculateDistance(float lat1, float lng1, float lat2, float lng2) {
    // 简化的欧几里得距离计算（适用于小范围地理坐标）
    float dlat = lat2 - lat1;
    float dlng = lng2 - lng1;
    return sqrtf(dlat * dlat + dlng * dlng) * 111000; // 近似转换为米
}

int minInt(int a, int b) {
    return (a < b) ? a : b;
}

int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

// 内存管理工具
void* safeMalloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr && size > 0) {
        fprintf(stderr, "内存分配失败: %zu 字节\n", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* safeRealloc(void* ptr, size_t size) {
    void* newPtr = realloc(ptr, size);
    if (!newPtr && size > 0) {
        fprintf(stderr, "内存重新分配失败: %zu 字节\n", size);
        free(ptr);
        exit(EXIT_FAILURE);
    }
    return newPtr;
}

// 错误处理
const char* getErrorMessage(ErrorCode error) {
    switch (error) {
        case ERR_NONE:
            return "无错误";
        case ERR_INVALID_INPUT:
            return "无效输入";
        case ERR_NODE_NOT_FOUND:
            return "节点未找到";
        case ERR_EDGE_NOT_FOUND:
            return "边未找到";
        case ERR_MEMORY_ALLOCATION:
            return "内存分配失败";
        case ERR_GRAPH_FULL:
            return "图已满";
        case ERR_PATH_NOT_FOUND:
            return "路径未找到";
        default:
            return "未知错误";
    }
}

void printError(ErrorCode error) {
    fprintf(stderr, "错误: %s\n", getErrorMessage(error));
} 