#include "../include/pathfinding.h"
#include "../include/utils.h"
#include <math.h>

// 创建优先队列
PriorityQueue* createPriorityQueue() {
    PriorityQueue* pq = (PriorityQueue*)safeMalloc(sizeof(PriorityQueue));
    if (!pq) return NULL;
    
    pq->head = NULL;
    pq->size = 0;
    return pq;
}

// 销毁优先队列
void destroyPriorityQueue(PriorityQueue* pq) {
    if (!pq) return;
    
    while (pq->head) {
        PriorityQueueNode* temp = pq->head;
        pq->head = pq->head->next;
        free(temp);
    }
    free(pq);
}

// 入队（按距离排序）
void enqueue(PriorityQueue* pq, int nodeId, int distance) {
    if (!pq) return;
    
    PriorityQueueNode* newNode = (PriorityQueueNode*)safeMalloc(sizeof(PriorityQueueNode));
    if (!newNode) return;
    
    newNode->nodeId = nodeId;
    newNode->distance = distance;
    newNode->next = NULL;
    
    // 按距离插入到正确位置
    if (!pq->head || distance < pq->head->distance) {
        newNode->next = pq->head;
        pq->head = newNode;
    } else {
        PriorityQueueNode* current = pq->head;
        while (current->next && current->next->distance <= distance) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    pq->size++;
}

// 出队
int dequeue(PriorityQueue* pq) {
    if (!pq || !pq->head) return -1;
    
    PriorityQueueNode* temp = pq->head;
    int nodeId = temp->nodeId;
    pq->head = pq->head->next;
    free(temp);
    pq->size--;
    
    return nodeId;
}

// 检查队列是否为空
int isEmpty(PriorityQueue* pq) {
    return !pq || pq->size == 0;
}

// Dijkstra算法实现
PathResult* dijkstra(Graph* graph, int start, int end, TransportMode mode) {
    if (!graph || !isValidNodeId(start, graph->nodeCount) || 
        !isValidNodeId(end, graph->nodeCount)) {
        return NULL;
    }
    
    // 初始化距离数组和前驱数组
    int* dist = (int*)safeMalloc(graph->nodeCount * sizeof(int));
    int* prev = (int*)safeMalloc(graph->nodeCount * sizeof(int));
    int* visited = (int*)safeMalloc(graph->nodeCount * sizeof(int));
    
    if (!dist || !prev || !visited) {
        free(dist);
        free(prev);
        free(visited);
        return NULL;
    }
    
    // 初始化
    for (int i = 0; i < graph->nodeCount; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;
    
    // 创建优先队列
    PriorityQueue* pq = createPriorityQueue();
    if (!pq) {
        free(dist);
        free(prev);
        free(visited);
        return NULL;
    }
    
    enqueue(pq, start, 0);
    
    // 主循环
    while (!isEmpty(pq)) {
        int current = dequeue(pq);
        
        if (visited[current] || !graph->nodes[current].isActive) continue;
        visited[current] = 1;
        
        if (current == end) break;  // 找到目标节点
        
        // 遍历邻接节点
        AdjListNode* adjNode = graph->adjList[current];
        while (adjNode) {
            int neighbor = adjNode->dest;
            Edge* edge = &graph->edges[adjNode->edgeIndex];
            
            // 检查边和节点是否可访问
            if (!edge->isAccessible || !graph->nodes[neighbor].isActive) {
                adjNode = adjNode->next;
                continue;
            }
            
            int weight = edge->modeWeight[mode];
            int newDist = dist[current] + weight;
            
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                prev[neighbor] = current;
                enqueue(pq, neighbor, newDist);
            }
            
            adjNode = adjNode->next;
        }
    }
    
    // 构建路径结果
    PathResult* result = (PathResult*)safeMalloc(sizeof(PathResult));
    if (!result) {
        destroyPriorityQueue(pq);
        free(dist);
        free(prev);
        free(visited);
        return NULL;
    }
    
    if (dist[end] == INF) {
        // 无路径
        result->path = NULL;
        result->pathLength = 0;
        result->totalDistance = 0;
        result->totalTime = 0;
        result->isValid = 0;
    } else {
        // 回溯构建路径
        int pathLength = 0;
        int temp = end;
        while (temp != -1) {
            pathLength++;
            temp = prev[temp];
        }
        
        result->path = (int*)safeMalloc(pathLength * sizeof(int));
        if (!result->path) {
            free(result);
            destroyPriorityQueue(pq);
            free(dist);
            free(prev);
            free(visited);
            return NULL;
        }
        
        result->pathLength = pathLength;
        result->totalDistance = 0;
        result->totalTime = 0;
        result->isValid = 1;
        
        // 填充路径数组（逆序）
        temp = end;
        for (int i = pathLength - 1; i >= 0; i--) {
            result->path[i] = temp;
            temp = prev[temp];
        }
        
        // 计算总距离和时间
        for (int i = 0; i < pathLength - 1; i++) {
            int from = result->path[i];
            int to = result->path[i + 1];
            
            // 找到对应的边
            for (int j = 0; j < graph->edgeCount; j++) {
                if (graph->edges[j].from == from && graph->edges[j].to == to) {
                    result->totalDistance += graph->edges[j].distance;
                    result->totalTime += graph->edges[j].timeCost;
                    break;
                }
            }
        }
    }
    
    destroyPriorityQueue(pq);
    free(dist);
    free(prev);
    free(visited);
    
    return result;
}

// 根据名称查找路径
PathResult* findPathByName(Graph* graph, const char* startName, const char* endName, TransportMode mode) {
    if (!graph || !isValidString(startName) || !isValidString(endName)) {
        return NULL;
    }
    
    int startId = findNodeByName(graph, startName);
    int endId = findNodeByName(graph, endName);
    
    if (startId == -1 || endId == -1) {
        return NULL;
    }
    
    return dijkstra(graph, startId, endId, mode);
}

// 释放路径结果
void freePathResult(PathResult* result) {
    if (!result) return;
    
    if (result->path) {
        free(result->path);
    }
    free(result);
}

// 打印路径
void printPath(Graph* graph, PathResult* result) {
    if (!graph || !result || !result->isValid) {
        printf("无效路径或路径不存在！\n");
        return;
    }
    
    printf("\n=== 路径信息 ===\n");
    printf("路径: ");
    for (int i = 0; i < result->pathLength; i++) {
        printf("%s", graph->nodes[result->path[i]].name);
        if (i < result->pathLength - 1) {
            printf(" → ");
        }
    }
    printf("\n");
    printf("总距离: %d 米\n", result->totalDistance);
    printf("预计时间: %d 秒 (%.1f 分钟)\n", result->totalTime, result->totalTime / 60.0);
}

// 打印详细路径信息
void printPathDetails(Graph* graph, PathResult* result, TransportMode mode) {
    if (!graph || !result || !result->isValid) {
        printf("无效路径或路径不存在！\n");
        return;
    }
    
    const char* modeStr = (mode == MODE_WALKING) ? "步行" : "驾车";
    
    printf("\n=== 详细路径信息 (%s模式) ===\n", modeStr);
    
    for (int i = 0; i < result->pathLength - 1; i++) {
        int from = result->path[i];
        int to = result->path[i + 1];
        
        // 找到对应的边
        for (int j = 0; j < graph->edgeCount; j++) {
            if (graph->edges[j].from == from && graph->edges[j].to == to) {
                printf("第%d段: %s → %s\n", i + 1, 
                       graph->nodes[from].name, graph->nodes[to].name);
                printf("  距离: %d 米\n", graph->edges[j].distance);
                printf("  时间: %d 秒\n", graph->edges[j].timeCost);
                printf("  权重: %d\n", graph->edges[j].modeWeight[mode]);
                printf("\n");
                break;
            }
        }
    }
    
    printf("总计: %d 米, %d 秒\n", result->totalDistance, result->totalTime);
}

// ==================== A*算法实现 ====================

// 创建A*算法专用优先队列
AStarPriorityQueue* createAStarPriorityQueue() {
    AStarPriorityQueue* pq = (AStarPriorityQueue*)safeMalloc(sizeof(AStarPriorityQueue));
    if (!pq) return NULL;
    
    pq->head = NULL;
    pq->size = 0;
    return pq;
}

// 销毁A*算法专用优先队列
void destroyAStarPriorityQueue(AStarPriorityQueue* pq) {
    if (!pq) return;
    
    while (pq->head) {
        AStarQueueNode* temp = pq->head;
        pq->head = pq->head->next;
        free(temp);
    }
    free(pq);
}

// A*优先队列入队（按fCost排序）
void astarEnqueue(AStarPriorityQueue* pq, int nodeId, int gCost, int hCost) {
    if (!pq) return;
    
    AStarQueueNode* newNode = (AStarQueueNode*)safeMalloc(sizeof(AStarQueueNode));
    if (!newNode) return;
    
    newNode->nodeId = nodeId;
    newNode->gCost = gCost;
    newNode->hCost = hCost;
    newNode->fCost = gCost + hCost;
    newNode->next = NULL;
    
    // 按fCost插入到正确位置，fCost相同时按hCost排序
    if (!pq->head || newNode->fCost < pq->head->fCost || 
        (newNode->fCost == pq->head->fCost && newNode->hCost < pq->head->hCost)) {
        newNode->next = pq->head;
        pq->head = newNode;
    } else {
        AStarQueueNode* current = pq->head;
        while (current->next && (current->next->fCost < newNode->fCost ||
               (current->next->fCost == newNode->fCost && current->next->hCost <= newNode->hCost))) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    pq->size++;
}

// A*优先队列出队
int astarDequeue(AStarPriorityQueue* pq) {
    if (!pq || !pq->head) return -1;
    
    AStarQueueNode* temp = pq->head;
    int nodeId = temp->nodeId;
    pq->head = pq->head->next;
    free(temp);
    pq->size--;
    
    return nodeId;
}

// 检查A*优先队列是否为空
int isAStarEmpty(AStarPriorityQueue* pq) {
    return !pq || pq->size == 0;
}

// 计算启发式函数（欧几里得距离）
int calculateHeuristic(Graph* graph, int from, int to) {
    if (!graph || !isValidNodeId(from, graph->nodeCount) || 
        !isValidNodeId(to, graph->nodeCount)) {
        return 0;
    }
    
    Node* fromNode = &graph->nodes[from];
    Node* toNode = &graph->nodes[to];
    
    // 计算经纬度差值
    float latDiff = fromNode->latitude - toNode->latitude;
    float lonDiff = fromNode->longitude - toNode->longitude;
    
    // 简化的欧几里得距离计算（近似转换为米）
    // 1度纬度 ≈ 111000米，1度经度 ≈ 85000米（在北纬40度左右）
    float latDistanceM = latDiff * 111000.0f;
    float lonDistanceM = lonDiff * 85000.0f;
    
    // 欧几里得距离
    float distance = sqrtf(latDistanceM * latDistanceM + lonDistanceM * lonDistanceM);
    
    return (int)distance;
}

// A*算法实现
PathResult* astar(Graph* graph, int start, int end, TransportMode mode) {
    if (!graph || !isValidNodeId(start, graph->nodeCount) || 
        !isValidNodeId(end, graph->nodeCount)) {
        return NULL;
    }
    
    // 初始化数组
    int* gScore = (int*)safeMalloc(graph->nodeCount * sizeof(int));
    int* fScore = (int*)safeMalloc(graph->nodeCount * sizeof(int));
    int* prev = (int*)safeMalloc(graph->nodeCount * sizeof(int));
    int* visited = (int*)safeMalloc(graph->nodeCount * sizeof(int));
    
    if (!gScore || !fScore || !prev || !visited) {
        free(gScore);
        free(fScore);
        free(prev);
        free(visited);
        return NULL;
    }
    
    // 初始化
    for (int i = 0; i < graph->nodeCount; i++) {
        gScore[i] = INF;
        fScore[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
    }
    
    gScore[start] = 0;
    fScore[start] = calculateHeuristic(graph, start, end);
    
    // 创建A*专用优先队列
    AStarPriorityQueue* pq = createAStarPriorityQueue();
    if (!pq) {
        free(gScore);
        free(fScore);
        free(prev);
        free(visited);
        return NULL;
    }
    
    astarEnqueue(pq, start, gScore[start], calculateHeuristic(graph, start, end));
    
    // A*主循环
    while (!isAStarEmpty(pq)) {
        int current = astarDequeue(pq);
        
        if (visited[current] || !graph->nodes[current].isActive) continue;
        visited[current] = 1;
        
        if (current == end) break;  // 找到目标节点
        
        // 遍历邻接节点
        AdjListNode* adjNode = graph->adjList[current];
        while (adjNode) {
            int neighbor = adjNode->dest;
            Edge* edge = &graph->edges[adjNode->edgeIndex];
            
            // 检查边和节点是否可访问
            if (!edge->isAccessible || !graph->nodes[neighbor].isActive || visited[neighbor]) {
                adjNode = adjNode->next;
                continue;
            }
            
            int weight = edge->modeWeight[mode];
            int tentativeGScore = gScore[current] + weight;
            
            if (tentativeGScore < gScore[neighbor]) {
                // 找到更好的路径
                prev[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                int hCost = calculateHeuristic(graph, neighbor, end);
                fScore[neighbor] = gScore[neighbor] + hCost;
                
                astarEnqueue(pq, neighbor, gScore[neighbor], hCost);
            }
            
            adjNode = adjNode->next;
        }
    }
    
    // 构建路径结果
    PathResult* result = (PathResult*)safeMalloc(sizeof(PathResult));
    if (!result) {
        destroyAStarPriorityQueue(pq);
        free(gScore);
        free(fScore);
        free(prev);
        free(visited);
        return NULL;
    }
    
    if (gScore[end] == INF) {
        // 无路径
        result->path = NULL;
        result->pathLength = 0;
        result->totalDistance = 0;
        result->totalTime = 0;
        result->isValid = 0;
    } else {
        // 回溯构建路径
        int pathLength = 0;
        int temp = end;
        while (temp != -1) {
            pathLength++;
            temp = prev[temp];
        }
        
        result->path = (int*)safeMalloc(pathLength * sizeof(int));
        if (!result->path) {
            free(result);
            destroyAStarPriorityQueue(pq);
            free(gScore);
            free(fScore);
            free(prev);
            free(visited);
            return NULL;
        }
        
        result->pathLength = pathLength;
        result->totalDistance = 0;
        result->totalTime = 0;
        result->isValid = 1;
        
        // 填充路径数组（逆序）
        temp = end;
        for (int i = pathLength - 1; i >= 0; i--) {
            result->path[i] = temp;
            temp = prev[temp];
        }
        
        // 计算总距离和时间
        for (int i = 0; i < pathLength - 1; i++) {
            int from = result->path[i];
            int to = result->path[i + 1];
            
            // 找到对应的边
            for (int j = 0; j < graph->edgeCount; j++) {
                if (graph->edges[j].from == from && graph->edges[j].to == to) {
                    result->totalDistance += graph->edges[j].distance;
                    result->totalTime += graph->edges[j].timeCost;
                    break;
                }
            }
        }
    }
    
    destroyAStarPriorityQueue(pq);
    free(gScore);
    free(fScore);
    free(prev);
    free(visited);
    
    return result;
}

// 根据名称使用A*算法查找路径
PathResult* findPathByNameAStar(Graph* graph, const char* startName, const char* endName, TransportMode mode) {
    if (!graph || !isValidString(startName) || !isValidString(endName)) {
        return NULL;
    }
    
    int startId = findNodeByName(graph, startName);
    int endId = findNodeByName(graph, endName);
    
    if (startId == -1 || endId == -1) {
        return NULL;
    }
    
    return astar(graph, startId, endId, mode);
} 