@echo off
chcp 65001 >nul 2>&1
title 生成独立可执行文件

echo ====================================
echo      生成独立可执行文件工具
echo ====================================
echo.

REM 检查GCC是否安装
gcc --version >nul 2>&1
if errorlevel 1 (
    echo [错误] 未找到GCC编译器！
    echo 请先安装编译环境，然后运行此脚本。
    goto error_exit
)

REM 创建目录
if not exist obj mkdir obj
if not exist bin mkdir bin
if not exist release mkdir release

echo 正在编译静态链接版本...
echo.

REM 编译源文件
echo [1/4] 编译 graph.c...
gcc -Wall -Wextra -std=c99 -O2 -Iinclude -c src/graph.c -o obj/graph.o
if errorlevel 1 goto compile_error

echo [2/4] 编译 pathfinding.c...
gcc -Wall -Wextra -std=c99 -O2 -Iinclude -c src/pathfinding.c -o obj/pathfinding.o
if errorlevel 1 goto compile_error

echo [3/4] 编译 utils.c...
gcc -Wall -Wextra -std=c99 -O2 -Iinclude -c src/utils.c -o obj/utils.o
if errorlevel 1 goto compile_error

echo [4/4] 编译 main.c...
gcc -Wall -Wextra -std=c99 -O2 -Iinclude -c src/main.c -o obj/main.o
if errorlevel 1 goto compile_error

REM 静态链接生成独立可执行文件
echo.
echo 正在生成独立可执行文件...
gcc -static -static-libgcc -static-libstdc++ obj/graph.o obj/pathfinding.o obj/utils.o obj/main.o -o release/PathFinding_Standalone.exe -lm
if errorlevel 1 goto link_error

REM 压缩可执行文件（可选）
if exist "C:\Program Files\7-Zip\7z.exe" (
    echo.
    echo 正在使用UPX压缩文件...
    "C:\Program Files\7-Zip\7z.exe" a release/PathFinding_Portable.zip release/PathFinding_Standalone.exe >nul 2>&1
)

echo.
echo =====================================
echo [成功] 独立可执行文件已生成！
echo =====================================
echo.
echo 文件位置: release\PathFinding_Standalone.exe
echo 文件大小: 
dir release\PathFinding_Standalone.exe | findstr PathFinding_Standalone.exe
echo.
echo 这个文件可以在任何Windows电脑上直接运行，
echo 无需安装任何编译环境或运行库！
echo.
echo 测试运行...
echo =====================================
release\PathFinding_Standalone.exe
goto normal_exit

:compile_error
echo.
echo [编译失败] 请检查源代码
goto error_exit

:link_error
echo.
echo [链接失败] 可能缺少静态库
echo 尝试生成普通版本...
gcc obj/graph.o obj/pathfinding.o obj/utils.o obj/main.o -o release/PathFinding_Normal.exe -lm
if errorlevel 1 (
    echo 普通版本也失败了
    goto error_exit
)
echo [成功] 生成了普通版本: release\PathFinding_Normal.exe
echo 注意：此版本可能需要运行库支持
goto normal_exit

:error_exit
echo.
echo 按任意键退出...
pause >nul
exit /b 1

:normal_exit
echo.
echo 按任意键退出...
pause >nul
exit /b 0 