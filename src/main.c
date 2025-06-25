#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"
#include "../include/pathfinding.h"
#include "../include/utils.h"

// 函数声明
void showMenu();
void initSampleData(Graph* graph);
void handleAddNode(Graph* graph);
void handleAddEdge(Graph* graph);
void handleFindPath(Graph* graph);
void handleUpdateWeight(Graph* graph);
void handleToggleObstacle(Graph* graph);
void handleShowGraph(Graph* graph);

int main() {
    printf("==================================\n");
    printf("     基于图的路径规划系统\n");
    printf("==================================\n\n");
    
    // 创建图
    Graph* graph = createGraph();
    if (!graph) {
        printf("创建图失败！\n");
        return 1;
    }
    
    // 初始化示例数据
    initSampleData(graph);
    
    int choice;
    char input[100];
    
    while (1) {
        showMenu();
        printf("请输入选择: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        choice = atoi(input);
        
        switch (choice) {
            case 1:
                handleAddNode(graph);
                break;
            case 2:
                handleAddEdge(graph);
                break;
            case 3:
                handleFindPath(graph);
                break;
            case 4:
                handleUpdateWeight(graph);
                break;
            case 5:
                handleToggleObstacle(graph);
                break;
            case 6:
                handleShowGraph(graph);
                break;
            case 0:
                printf("感谢使用！再见！\n");
                destroyGraph(graph);
                return 0;
            default:
                printf("无效选择，请重新输入！\n");
        }
        
        printf("\n按回车键继续...");
        getchar();
    }
    
    destroyGraph(graph);
    return 0;
}

void showMenu() {
    printf("\n=== 路径规划系统菜单 ===\n");
    printf("1. 添加节点\n");
    printf("2. 添加边\n");
    printf("3. 查找路径\n");
    printf("4. 更新边权重\n");
    printf("5. 设置障碍\n");
    printf("6. 显示图信息\n");
    printf("0. 退出\n");
    printf("=======================\n");
}

void initSampleData(Graph* graph) {
    printf("正在初始化示例数据...\n");
    
    // 添加节点（模拟校园地图）
    addNode(graph, "校门口", 40.0001f, 116.0001f, NODE_TRANSPORT_HUB);
    addNode(graph, "图书馆", 40.0002f, 116.0002f, NODE_NORMAL);
    addNode(graph, "教学楼", 40.0003f, 116.0003f, NODE_NORMAL);
    addNode(graph, "食堂", 40.0004f, 116.0004f, NODE_NORMAL);
    addNode(graph, "宿舍区", 40.0005f, 116.0005f, NODE_NORMAL);
    addNode(graph, "体育馆", 40.0006f, 116.0006f, NODE_NORMAL);
    addNode(graph, "实验楼", 40.0007f, 116.0007f, NODE_NORMAL);
    
    // 添加边（双向道路）
    // 校门口到其他地点
    addEdge(graph, 0, 1, 300, 240, 300, 180);  // 校门口 -> 图书馆
    addEdge(graph, 1, 0, 300, 240, 300, 180);  // 图书馆 -> 校门口
    
    addEdge(graph, 0, 3, 200, 150, 200, 120);  // 校门口 -> 食堂
    addEdge(graph, 3, 0, 200, 150, 200, 120);  // 食堂 -> 校门口
    
    // 图书馆到其他地点
    addEdge(graph, 1, 2, 250, 180, 250, 150);  // 图书馆 -> 教学楼
    addEdge(graph, 2, 1, 250, 180, 250, 150);  // 教学楼 -> 图书馆
    
    addEdge(graph, 1, 6, 400, 300, 400, 240);  // 图书馆 -> 实验楼
    addEdge(graph, 6, 1, 400, 300, 400, 240);  // 实验楼 -> 图书馆
    
    // 教学楼到其他地点
    addEdge(graph, 2, 3, 150, 120, 150, 90);   // 教学楼 -> 食堂
    addEdge(graph, 3, 2, 150, 120, 150, 90);   // 食堂 -> 教学楼
    
    addEdge(graph, 2, 4, 350, 280, 350, 210);  // 教学楼 -> 宿舍区
    addEdge(graph, 4, 2, 350, 280, 350, 210);  // 宿舍区 -> 教学楼
    
    // 食堂到其他地点
    addEdge(graph, 3, 4, 180, 144, 180, 108);  // 食堂 -> 宿舍区
    addEdge(graph, 4, 3, 180, 144, 180, 108);  // 宿舍区 -> 食堂
    
    addEdge(graph, 3, 5, 220, 176, 220, 132);  // 食堂 -> 体育馆
    addEdge(graph, 5, 3, 220, 176, 220, 132);  // 体育馆 -> 食堂
    
    // 宿舍区到其他地点
    addEdge(graph, 4, 5, 280, 224, 280, 168);  // 宿舍区 -> 体育馆
    addEdge(graph, 5, 4, 280, 224, 280, 168);  // 体育馆 -> 宿舍区
    
    // 体育馆到实验楼
    addEdge(graph, 5, 6, 320, 256, 320, 192);  // 体育馆 -> 实验楼
    addEdge(graph, 6, 5, 320, 256, 320, 192);  // 实验楼 -> 体育馆
    
    printf("示例数据初始化完成！\n");
    printf("已创建校园地图：校门口、图书馆、教学楼、食堂、宿舍区、体育馆、实验楼\n");
}

void handleAddNode(Graph* graph) {
    char name[MAX_NAME_LEN];
    float lat, lng;
    int type;
    
    printf("\n=== 添加节点 ===\n");
    printf("输入节点名称: ");
    if (fgets(name, sizeof(name), stdin) == NULL) return;
    
    // 去除换行符
    name[strcspn(name, "\n")] = 0;
    trimString(name);
    
    if (!isValidString(name)) {
        printf("无效的节点名称！\n");
        return;
    }
    
    printf("输入纬度: ");
    scanf("%f", &lat);
    printf("输入经度: ");
    scanf("%f", &lng);
    printf("输入节点类型 (0-普通, 1-交通枢纽, 2-障碍): ");
    scanf("%d", &type);
    getchar(); // 消费换行符
    
    int nodeId = addNode(graph, name, lat, lng, (NodeType)type);
    if (nodeId != -1) {
        printf("节点添加成功！ID: %d\n", nodeId);
    } else {
        printf("节点添加失败！可能是图已满或节点名称重复。\n");
    }
}

void handleAddEdge(Graph* graph) {
    char fromName[MAX_NAME_LEN], toName[MAX_NAME_LEN];
    int distance, timeCost, walkWeight, driveWeight;
    
    printf("\n=== 添加边 ===\n");
    printf("输入起点名称: ");
    if (fgets(fromName, sizeof(fromName), stdin) == NULL) return;
    fromName[strcspn(fromName, "\n")] = 0;
    trimString(fromName);
    
    printf("输入终点名称: ");
    if (fgets(toName, sizeof(toName), stdin) == NULL) return;
    toName[strcspn(toName, "\n")] = 0;
    trimString(toName);
    
    int fromId = findNodeByName(graph, fromName);
    int toId = findNodeByName(graph, toName);
    
    if (fromId == -1 || toId == -1) {
        printf("起点或终点不存在！\n");
        return;
    }
    
    printf("输入距离(米): ");
    scanf("%d", &distance);
    printf("输入时间成本(秒): ");
    scanf("%d", &timeCost);
    printf("输入步行权重: ");
    scanf("%d", &walkWeight);
    printf("输入驾车权重: ");
    scanf("%d", &driveWeight);
    getchar(); // 消费换行符
    
    int edgeId = addEdge(graph, fromId, toId, distance, timeCost, walkWeight, driveWeight);
    if (edgeId != -1) {
        printf("边添加成功！ID: %d\n", edgeId);
    } else {
        printf("边添加失败！\n");
    }
}

void handleFindPath(Graph* graph) {
    char startName[MAX_NAME_LEN], endName[MAX_NAME_LEN];
    int mode;
    
    printf("\n=== 查找路径 ===\n");
    printf("输入起点名称: ");
    if (fgets(startName, sizeof(startName), stdin) == NULL) return;
    startName[strcspn(startName, "\n")] = 0;
    trimString(startName);
    
    printf("输入终点名称: ");
    if (fgets(endName, sizeof(endName), stdin) == NULL) return;
    endName[strcspn(endName, "\n")] = 0;
    trimString(endName);
    
    printf("选择交通方式 (0-步行, 1-驾车): ");
    scanf("%d", &mode);
    getchar(); // 消费换行符
    
    if (mode < 0 || mode >= MODE_COUNT) {
        printf("无效的交通方式！\n");
        return;
    }
    
    PathResult* result = findPathByName(graph, startName, endName, (TransportMode)mode);
    
    if (result && result->isValid) {
        printPath(graph, result);
        
        printf("\n是否显示详细信息? (y/n): ");
        char choice;
        scanf("%c", &choice);
        getchar(); // 消费换行符
        
        if (choice == 'y' || choice == 'Y') {
            printPathDetails(graph, result, (TransportMode)mode);
        }
    } else {
        printf("未找到从 %s 到 %s 的路径！\n", startName, endName);
    }
    
    freePathResult(result);
}

void handleUpdateWeight(Graph* graph) {
    char fromName[MAX_NAME_LEN], toName[MAX_NAME_LEN];
    int mode, newWeight;
    
    printf("\n=== 更新边权重 ===\n");
    printf("输入起点名称: ");
    if (fgets(fromName, sizeof(fromName), stdin) == NULL) return;
    fromName[strcspn(fromName, "\n")] = 0;
    trimString(fromName);
    
    printf("输入终点名称: ");
    if (fgets(toName, sizeof(toName), stdin) == NULL) return;
    toName[strcspn(toName, "\n")] = 0;
    trimString(toName);
    
    int fromId = findNodeByName(graph, fromName);
    int toId = findNodeByName(graph, toName);
    
    if (fromId == -1 || toId == -1) {
        printf("起点或终点不存在！\n");
        return;
    }
    
    printf("选择交通方式 (0-步行, 1-驾车): ");
    scanf("%d", &mode);
    printf("输入新权重: ");
    scanf("%d", &newWeight);
    getchar(); // 消费换行符
    
    updateEdgeWeight(graph, fromId, toId, (TransportMode)mode, newWeight);
    printf("边权重更新成功！\n");
}

void handleToggleObstacle(Graph* graph) {
    char name[MAX_NAME_LEN];
    int accessible;
    
    printf("\n=== 设置障碍 ===\n");
    printf("输入节点名称: ");
    if (fgets(name, sizeof(name), stdin) == NULL) return;
    name[strcspn(name, "\n")] = 0;
    trimString(name);
    
    int nodeId = findNodeByName(graph, name);
    if (nodeId == -1) {
        printf("节点不存在！\n");
        return;
    }
    
    printf("设置可访问性 (1-可访问, 0-不可访问): ");
    scanf("%d", &accessible);
    getchar(); // 消费换行符
    
    setNodeAccessible(graph, nodeId, accessible);
    printf("节点可访问性设置成功！\n");
}

void handleShowGraph(Graph* graph) {
    printf("\n=== 显示图信息 ===\n");
    printGraph(graph);
} 