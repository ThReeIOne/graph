@echo off
chcp 65001 >nul
title 测试编译

echo 测试编译单个文件...

echo 编译 utils.c...
gcc -std=c99 -Iinclude -c src/utils.c -o utils.o
if errorlevel 1 (
    echo utils.c 编译失败
    pause
    exit /b 1
)

echo 编译 graph.c...
gcc -std=c99 -Iinclude -c src/graph.c -o graph.o
if errorlevel 1 (
    echo graph.c 编译失败
    pause
    exit /b 1
)

echo 编译 pathfinding.c...
gcc -std=c99 -Iinclude -c src/pathfinding.c -o pathfinding.o
if errorlevel 1 (
    echo pathfinding.c 编译失败
    pause
    exit /b 1
)

echo 编译 main.c...
gcc -std=c99 -Iinclude -c src/main.c -o main.o
if errorlevel 1 (
    echo main.c 编译失败
    pause
    exit /b 1
)

echo 链接...
gcc utils.o graph.o pathfinding.o main.o -o PathFinding.exe -lm
if errorlevel 1 (
    echo 链接失败
    pause
    exit /b 1
)

echo 编译成功！
pause 