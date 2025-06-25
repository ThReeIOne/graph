@echo off
chcp 65001 >nul
title 编译路径规划系统

echo ================================
echo      编译路径规划系统
echo ================================
echo.

REM 清理旧文件
echo 清理旧文件...
del *.o 2>nul
del *.exe 2>nul

REM 检查是否有GCC编译器
gcc --version >nul 2>&1
if errorlevel 1 (
    echo [错误] 未找到GCC编译器！
    echo 请安装MinGW或其他C编译器。
    goto error
)

echo 开始编译...
echo.

REM 编译源文件
echo [1/4] 编译 graph.c...
gcc -std=c99 -Iinclude -c src/graph.c -o graph.o
if errorlevel 1 goto error

echo [2/4] 编译 pathfinding.c...
gcc -std=c99 -Iinclude -c src/pathfinding.c -o pathfinding.o
if errorlevel 1 goto error

echo [3/4] 编译 utils.c...
gcc -std=c99 -Iinclude -c src/utils.c -o utils.o
if errorlevel 1 goto error

echo [4/4] 编译 main.c...
gcc -std=c99 -Iinclude -c src/main.c -o main.o
if errorlevel 1 goto error

echo.
echo 正在链接程序...
gcc graph.o pathfinding.o utils.o main.o -o PathFinding.exe -lm
if errorlevel 1 goto error

echo.
echo ================================
echo 编译成功！生成文件: PathFinding.exe
echo ================================
goto end

:error
echo.
echo 编译失败！请检查代码或编译环境。

:end
echo.
echo 按任意键退出...
pause >nul 