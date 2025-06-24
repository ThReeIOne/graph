#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "graph.h"

// 优先队列节点（用于Dijkstra算法）
typedef struct PriorityQueueNode {
    int nodeId;
    int distance;
    struct PriorityQueueNode* next;
} PriorityQueueNode;

// 优先队列结构
typedef struct {
    PriorityQueueNode* head;
    int size;
} PriorityQueue;

// 路径查找算法函数声明
PathResult* dijkstra(Graph* graph, int start, int end, TransportMode mode);
PathResult* findPathByName(Graph* graph, const char* startName, const char* endName, TransportMode mode);
void freePathResult(PathResult* result);

// 优先队列操作函数
PriorityQueue* createPriorityQueue();
void destroyPriorityQueue(PriorityQueue* pq);
void enqueue(PriorityQueue* pq, int nodeId, int distance);
int dequeue(PriorityQueue* pq);
int isEmpty(PriorityQueue* pq);

// 路径结果处理函数
void printPath(Graph* graph, PathResult* result);
void printPathDetails(Graph* graph, PathResult* result, TransportMode mode);

#endif // PATHFINDING_H 