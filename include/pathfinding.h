#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "graph.h"

// 优先队列节点（用于Dijkstra算法）
typedef struct PriorityQueueNode {
    int nodeId;
    int distance;
    struct PriorityQueueNode* next;
} PriorityQueueNode;

// A*算法专用优先队列节点
typedef struct AStarQueueNode {
    int nodeId;
    int gCost;      // 从起点到当前节点的实际代价
    int hCost;      // 从当前节点到终点的启发式代价
    int fCost;      // f = g + h
    struct AStarQueueNode* next;
} AStarQueueNode;

// 优先队列结构
typedef struct {
    PriorityQueueNode* head;
    int size;
} PriorityQueue;

// A*算法专用优先队列结构
typedef struct {
    AStarQueueNode* head;
    int size;
} AStarPriorityQueue;

// 路径查找算法函数声明
PathResult* dijkstra(Graph* graph, int start, int end, TransportMode mode);
PathResult* astar(Graph* graph, int start, int end, TransportMode mode);
PathResult* findPathByName(Graph* graph, const char* startName, const char* endName, TransportMode mode);
PathResult* findPathByNameAStar(Graph* graph, const char* startName, const char* endName, TransportMode mode);
void freePathResult(PathResult* result);

// 优先队列操作函数
PriorityQueue* createPriorityQueue();
void destroyPriorityQueue(PriorityQueue* pq);
void enqueue(PriorityQueue* pq, int nodeId, int distance);
int dequeue(PriorityQueue* pq);
int isEmpty(PriorityQueue* pq);

// A*算法专用优先队列操作函数
AStarPriorityQueue* createAStarPriorityQueue();
void destroyAStarPriorityQueue(AStarPriorityQueue* pq);
void astarEnqueue(AStarPriorityQueue* pq, int nodeId, int gCost, int hCost);
int astarDequeue(AStarPriorityQueue* pq);
int isAStarEmpty(AStarPriorityQueue* pq);

// 启发式函数
int calculateHeuristic(Graph* graph, int from, int to);

// 路径结果处理函数
void printPath(Graph* graph, PathResult* result);
void printPathDetails(Graph* graph, PathResult* result, TransportMode mode);

#endif // PATHFINDING_H 