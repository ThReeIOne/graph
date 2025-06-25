@echo off
chcp 65001 >nul 2>&1
title 路径规划系统编译器

echo ================================
echo      路径规划系统编译器
echo ================================
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
echo 创建编译目录...
if not exist obj mkdir obj
if not exist bin mkdir bin

REM 编译源文件
echo.
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

echo.
echo [编译成功] 可执行文件已生成: bin\pathfinding.exe
echo.
echo 现在可以运行以下命令启动程序：
echo    run.bat
echo 或者双击 run.bat 文件
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