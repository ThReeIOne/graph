# 基于图结构的路径规划系统

## 项目简介

这是一个用C语言实现的基于图结构的路径规划系统，能够模拟城市道路网络或校园地图，为用户提供最优路径查询服务。系统采用了模块化设计，具有良好的代码结构和可扩展性。

## 项目特性

### 核心功能
- **图数据结构**: 使用邻接表实现的带权有向图
- **路径查找**: 基于Dijkstra算法的最短路径计算
- **多种交通方式**: 支持步行和驾车两种出行模式
- **动态权重调整**: 支持实时修改道路权重（模拟交通拥堵）
- **障碍设置**: 可设置节点或边为不可通行状态
- **关键词搜索**: 支持通过地点名称进行路径查询

### 技术特性
- **内存安全**: 完善的内存管理和错误处理机制
- **模块化设计**: 清晰的文件结构和功能分离
- **高效算法**: 使用优先队列优化的Dijkstra算法
- **用户友好**: 直观的交互界面和详细的路径信息显示

## 项目结构

```
graph/
├── include/                 # 头文件目录
│   ├── graph.h             # 图数据结构定义
│   ├── pathfinding.h       # 路径查找算法
│   └── utils.h             # 工具函数
├── src/                    # 源代码目录
│   ├── graph.c             # 图操作实现
│   ├── pathfinding.c       # 路径查找实现
│   ├── utils.c             # 工具函数实现
│   └── main.c              # 主程序
├── data/                   # 数据文件目录
│   └── sample_graph.txt    # 示例图数据
├── obj/                    # 目标文件目录（编译时创建）
├── bin/                    # 可执行文件目录（编译时创建）
├── Makefile                # 编译配置
├── README.md               # 原始需求文档
└── PROJECT_README.md       # 项目说明文档
```

## 编译与运行

### 系统要求
- GCC编译器
- Make工具
- Linux/macOS/Windows(WSL)系统

### 编译步骤

1. **克隆或下载项目**
```bash
cd graph
```

2. **编译项目**
```bash
make all
```

3. **运行程序**
```bash
make run
```

或者直接运行：
```bash
./bin/pathfinding
```

### 其他编译选项

```bash
# 清理编译文件
make clean

# 调试模式编译
make debug

# 发布模式编译
make release

# 查看帮助
make help
```

## 使用说明

### 主菜单功能

程序启动后会显示以下菜单：

```
=== 路径规划系统菜单 ===
1. 添加节点
2. 添加边  
3. 查找路径
4. 更新边权重
5. 设置障碍
6. 显示图信息
0. 退出
=======================
```

### 功能详解

#### 1. 添加节点
- 输入节点名称、经纬度坐标和节点类型
- 节点类型：0=普通，1=交通枢纽，2=障碍

#### 2. 添加边
- 输入起点和终点名称
- 设置距离、时间成本、步行权重、驾车权重

#### 3. 查找路径
- 输入起点和终点名称
- 选择交通方式（步行/驾车）
- 系统自动计算并显示最优路径

#### 4. 更新边权重
- 选择特定边和交通方式
- 修改权重值（可模拟交通拥堵）

#### 5. 设置障碍
- 选择节点设置为可访问或不可访问
- 用于模拟道路封闭等情况

#### 6. 显示图信息
- 查看当前图中所有节点和边的信息

## 示例数据

系统预置了校园地图示例数据，包含以下地点：
- 校门口（交通枢纽）
- 图书馆
- 教学楼  
- 食堂
- 宿舍区
- 体育馆
- 实验楼

## 使用示例

### 查找路径示例
```
输入起点名称: 校门口
输入终点名称: 图书馆
选择交通方式 (0-步行, 1-驾车): 0

=== 路径信息 ===
路径: 校门口 → 图书馆
总距离: 300 米
预计时间: 240 秒 (4.0 分钟)
```

### 交通拥堵模拟
```
输入起点名称: 校门口
输入终点名称: 图书馆
选择交通方式 (0-步行, 1-驾车): 0
输入新权重: 600

边权重更新成功！
```

## 算法说明

### Dijkstra算法
- 使用优先队列优化，时间复杂度O((V+E)logV)
- 支持动态权重和障碍设置
- 保证找到最短路径（如果路径存在）

### 数据结构
- **邻接表**: 存储图的拓扑结构，空间效率高
- **优先队列**: 使用链表实现的最小堆
- **路径回溯**: 记录前驱节点，重构完整路径

## 扩展功能

系统设计具有良好的可扩展性，可以轻松添加：

1. **A*启发式算法**: 利用坐标信息加速路径搜索
2. **Floyd-Warshall算法**: 计算所有节点对间的最短路径
3. **实时路况**: 集成实时交通数据
4. **路径可视化**: 图形界面显示路径
5. **多种出行方式**: 公交、骑行等模式

## 技术细节

### 内存管理
- 使用`safeMalloc`和`safeRealloc`包装器
- 完善的内存释放和错误处理
- 避免内存泄漏和悬空指针

### 错误处理
- 定义了完整的错误码体系
- 输入验证和边界检查
- 友好的错误信息提示

### 代码质量
- 遵循C99标准
- 清晰的命名规范
- 详细的注释文档
- 模块化的函数设计

## 许可证

本项目仅用于学习和研究目的。

## 联系方式

如有问题或建议，请通过以下方式联系：
- 项目Issue
- 邮件联系

---

**最后更新**: 2024年12月
**版本**: 1.0.0 