#include "../include/graph.h"
#include "../include/utils.h"

// 创建图
Graph* createGraph() {
    Graph* graph = (Graph*)safeMalloc(sizeof(Graph));
    if (!graph) return NULL;
    
    graph->nodeCount = 0;
    graph->edgeCount = 0;
    
    // 初始化邻接表
    for (int i = 0; i < MAX_NODES; i++) {
        graph->adjList[i] = NULL;
    }
    
    return graph;
}

// 销毁图
void destroyGraph(Graph* graph) {
    if (!graph) return;
    
    // 释放邻接表内存
    for (int i = 0; i < graph->nodeCount; i++) {
        AdjListNode* current = graph->adjList[i];
        while (current) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(graph);
}

// 添加节点
int addNode(Graph* graph, const char* name, float lat, float lng, NodeType type) {
    if (!graph || !isValidString(name) || graph->nodeCount >= MAX_NODES) {
        return -1;
    }
    
    // 检查是否已存在同名节点
    if (findNodeByName(graph, name) != -1) {
        return -1;  // 已存在
    }
    
    int nodeId = graph->nodeCount;
    Node* node = &graph->nodes[nodeId];
    
    node->id = nodeId;
    strncpy(node->name, name, MAX_NAME_LEN - 1);
    node->name[MAX_NAME_LEN - 1] = '\0';
    node->latitude = lat;
    node->longitude = lng;
    node->type = type;
    node->isActive = 1;  // 默认激活
    
    graph->nodeCount++;
    return nodeId;
}

// 添加边
int addEdge(Graph* graph, int from, int to, int distance, int timeCost, 
            int walkWeight, int driveWeight) {
    if (!graph || !isValidNodeId(from, graph->nodeCount) || 
        !isValidNodeId(to, graph->nodeCount) || graph->edgeCount >= MAX_EDGES) {
        return -1;
    }
    
    int edgeId = graph->edgeCount;
    Edge* edge = &graph->edges[edgeId];
    
    edge->from = from;
    edge->to = to;
    edge->distance = distance;
    edge->timeCost = timeCost;
    edge->isAccessible = 1;
    edge->modeWeight[MODE_WALKING] = walkWeight;
    edge->modeWeight[MODE_DRIVING] = driveWeight;
    
    // 添加到邻接表
    AdjListNode* newNode = (AdjListNode*)safeMalloc(sizeof(AdjListNode));
    if (!newNode) return -1;
    
    newNode->dest = to;
    newNode->edgeIndex = edgeId;
    newNode->next = graph->adjList[from];
    graph->adjList[from] = newNode;
    
    graph->edgeCount++;
    return edgeId;
}

// 根据名称查找节点
int findNodeByName(Graph* graph, const char* name) {
    if (!graph || !isValidString(name)) return -1;
    
    for (int i = 0; i < graph->nodeCount; i++) {
        if (stringCompareIgnoreCase(graph->nodes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// 更新边权重
void updateEdgeWeight(Graph* graph, int from, int to, TransportMode mode, int newWeight) {
    if (!graph || !isValidNodeId(from, graph->nodeCount) || 
        !isValidNodeId(to, graph->nodeCount)) return;
    
    for (int i = 0; i < graph->edgeCount; i++) {
        if (graph->edges[i].from == from && graph->edges[i].to == to) {
            graph->edges[i].modeWeight[mode] = newWeight;
            break;
        }
    }
}

// 设置节点可访问性
void setNodeAccessible(Graph* graph, int nodeId, int accessible) {
    if (!graph || !isValidNodeId(nodeId, graph->nodeCount)) return;
    graph->nodes[nodeId].isActive = accessible;
}

// 设置边可访问性
void setEdgeAccessible(Graph* graph, int from, int to, int accessible) {
    if (!graph || !isValidNodeId(from, graph->nodeCount) || 
        !isValidNodeId(to, graph->nodeCount)) return;
    
    for (int i = 0; i < graph->edgeCount; i++) {
        if (graph->edges[i].from == from && graph->edges[i].to == to) {
            graph->edges[i].isAccessible = accessible;
            break;
        }
    }
}

// 移除节点（简化实现，实际应该重新组织数据结构）
int removeNode(Graph* graph, int nodeId) {
    if (!graph || !isValidNodeId(nodeId, graph->nodeCount)) return -1;
    
    // 设置节点为不活跃状态（简化实现）
    graph->nodes[nodeId].isActive = 0;
    return 0;
}

// 移除边
int removeEdge(Graph* graph, int from, int to) {
    if (!graph || !isValidNodeId(from, graph->nodeCount) || 
        !isValidNodeId(to, graph->nodeCount)) return -1;
    
    // 从邻接表中移除
    AdjListNode* current = graph->adjList[from];
    AdjListNode* prev = NULL;
    
    while (current) {
        if (current->dest == to) {
            if (prev) {
                prev->next = current->next;
            } else {
                graph->adjList[from] = current->next;
            }
            
            // 设置边为不可访问（简化实现）
            graph->edges[current->edgeIndex].isAccessible = 0;
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    
    return -1;
}

// 打印图信息
void printGraph(Graph* graph) {
    if (!graph) return;
    
    printf("\n=== 图结构信息 ===\n");
    printf("节点数量: %d\n", graph->nodeCount);
    printf("边数量: %d\n", graph->edgeCount);
    
    printf("\n--- 节点列表 ---\n");
    for (int i = 0; i < graph->nodeCount; i++) {
        Node* node = &graph->nodes[i];
        if (node->isActive) {
            printf("ID: %d, 名称: %s, 坐标: (%.6f, %.6f), 类型: %d\n",
                   node->id, node->name, node->latitude, node->longitude, node->type);
        }
    }
    
    printf("\n--- 边列表 ---\n");
    for (int i = 0; i < graph->edgeCount; i++) {
        Edge* edge = &graph->edges[i];
        if (edge->isAccessible) {
            printf("从 %s 到 %s: 距离=%dm, 时间=%ds, 步行权重=%d, 驾车权重=%d\n",
                   graph->nodes[edge->from].name, graph->nodes[edge->to].name,
                   edge->distance, edge->timeCost, 
                   edge->modeWeight[MODE_WALKING], edge->modeWeight[MODE_DRIVING]);
        }
    }
} 