@echo off
chcp 65001 >nul 2>&1
title 路径规划系统

echo ================================
echo      路径规划系统启动器
echo ================================
echo.

REM 检查可执行文件是否存在
if not exist bin\pathfinding.exe (
    echo [提示] 未找到可执行文件，正在尝试编译...
    echo.
    
    REM 检查GCC是否安装
    gcc --version >nul 2>&1
    if errorlevel 1 (
        echo [错误] 未找到GCC编译器！
        echo.
        echo 请先安装以下任一编译环境：
        echo 1. MinGW-w64: https://www.mingw-w64.org/
        echo 2. Code::Blocks (包含MinGW)
        echo 3. Dev-C++
        echo 4. MSYS2: https://www.msys2.org/
        echo.
        goto error_exit
    )
    
    REM 创建目录
    if not exist obj mkdir obj
    if not exist bin mkdir bin
    
    REM 编译源文件
    echo 开始编译源文件...
    echo [1/4] 编译 graph.c...
    gcc -Wall -Wextra -std=c99 -g -O2 -Iinclude -c src/graph.c -o obj/graph.o
    if errorlevel 1 goto compile_error
    
    echo [2/4] 编译 pathfinding.c...
    gcc -Wall -Wextra -std=c99 -g -O2 -Iinclude -c src/pathfinding.c -o obj/pathfinding.o
    if errorlevel 1 goto compile_error
    
    echo [3/4] 编译 utils.c...
    gcc -Wall -Wextra -std=c99 -g -O2 -Iinclude -c src/utils.c -o obj/utils.o
    if errorlevel 1 goto compile_error
    
    echo [4/4] 编译 main.c...
    gcc -Wall -Wextra -std=c99 -g -O2 -Iinclude -c src/main.c -o obj/main.o
    if errorlevel 1 goto compile_error
    
    REM 链接生成可执行文件
    echo.
    echo 正在链接生成可执行文件...
    gcc obj/graph.o obj/pathfinding.o obj/utils.o obj/main.o -o bin/pathfinding.exe -lm
    if errorlevel 1 goto compile_error
    
    echo [编译成功] 可执行文件已生成
    echo.
)

REM 启动程序
echo 正在启动路径规划系统...
echo ================================
echo.

bin\pathfinding.exe

echo.
echo ================================
echo 程序已结束
echo.
goto normal_exit

:compile_error
echo.
echo [编译失败] 请检查代码是否有语法错误
goto error_exit

:error_exit
echo.
echo 按任意键退出...
pause >nul
exit /b 1

:normal_exit
echo 按任意键退出...
pause >nul
exit /b 0 