#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NAME_LEN 32
#define MAX_NODES 100
#define MAX_EDGES 1000
#define INF INT_MAX

// 交通方式枚举
typedef enum {
    MODE_WALKING = 0,
    MODE_DRIVING = 1,
    MODE_COUNT = 2
} TransportMode;

// 节点类型枚举
typedef enum {
    NODE_NORMAL = 0,
    NODE_TRANSPORT_HUB = 1,
    NODE_OBSTACLE = 2
} NodeType;

// 节点结构（地标信息）
typedef struct {
    int id;                     // 节点编号
    char name[MAX_NAME_LEN];    // 地点名称
    float latitude;             // 纬度
    float longitude;            // 经度
    NodeType type;              // 地点类型
    int isActive;               // 是否激活（用于避障）
} Node;

// 边结构（道路信息）
typedef struct {
    int from, to;               // 起点编号，终点编号
    int distance;               // 距离（单位：米）
    int timeCost;               // 所需时间（单位：秒）
    int isAccessible;           // 是否通行
    int modeWeight[MODE_COUNT]; // 不同交通方式的权重
} Edge;

// 邻接表节点
typedef struct AdjListNode {
    int dest;                   // 目标节点
    int edgeIndex;              // 边的索引
    struct AdjListNode* next;   // 下一个邻接节点
} AdjListNode;

// 图结构
typedef struct {
    int nodeCount;              // 节点数量
    int edgeCount;              // 边数量
    Node nodes[MAX_NODES];      // 节点数组
    Edge edges[MAX_EDGES];      // 边数组
    AdjListNode* adjList[MAX_NODES]; // 邻接表
} Graph;

// 路径结果结构
typedef struct {
    int* path;                  // 路径节点序列
    int pathLength;             // 路径长度（节点数）
    int totalDistance;          // 总距离
    int totalTime;              // 总时间
    int isValid;                // 路径是否有效
} PathResult;

// 图操作函数声明
Graph* createGraph();
void destroyGraph(Graph* graph);
int addNode(Graph* graph, const char* name, float lat, float lng, NodeType type);
int addEdge(Graph* graph, int from, int to, int distance, int timeCost, 
            int walkWeight, int driveWeight);
int removeNode(Graph* graph, int nodeId);
int removeEdge(Graph* graph, int from, int to);
int findNodeByName(Graph* graph, const char* name);
void updateEdgeWeight(Graph* graph, int from, int to, TransportMode mode, int newWeight);
void setNodeAccessible(Graph* graph, int nodeId, int accessible);
void setEdgeAccessible(Graph* graph, int from, int to, int accessible);
void printGraph(Graph* graph);

#endif // GRAPH_H 