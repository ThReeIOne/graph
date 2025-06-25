@echo off
chcp 65001 >nul 2>&1
title 创建便携式程序包

echo =====================================
echo      创建便携式程序包工具
echo =====================================
echo.

REM 检查GCC是否安装
gcc --version >nul 2>&1
if errorlevel 1 (
    echo [错误] 未找到GCC编译器！
    goto error_exit
)

REM 创建目录
if not exist portable mkdir portable
if not exist portable\bin mkdir portable\bin
if not exist portable\libs mkdir portable\libs

echo 正在编译程序...
echo.

REM 编译程序（普通版本）
gcc -Wall -Wextra -std=c99 -O2 -Iinclude src/graph.c src/pathfinding.c src/utils.c src/main.c -o portable/bin/PathFinding.exe -lm
if errorlevel 1 goto compile_error

echo 正在查找依赖的DLL文件...

REM 查找GCC路径
for /f "tokens=*" %%i in ('where gcc') do set GCC_PATH=%%i
for %%i in ("%GCC_PATH%") do set GCC_DIR=%%~dpi

echo GCC目录: %GCC_DIR%

REM 复制可能需要的运行时库
if exist "%GCC_DIR%libgcc_s_seh-1.dll" copy "%GCC_DIR%libgcc_s_seh-1.dll" portable\libs\ >nul
if exist "%GCC_DIR%libwinpthread-1.dll" copy "%GCC_DIR%libwinpthread-1.dll" portable\libs\ >nul
if exist "%GCC_DIR%libstdc++-6.dll" copy "%GCC_DIR%libstdc++-6.dll" portable\libs\ >nul

REM 创建启动脚本
echo @echo off > portable\run.bat
echo chcp 65001 ^>nul >> portable\run.bat
echo title 路径规划系统 >> portable\run.bat
echo set PATH=%%~dp0libs;%%PATH%% >> portable\run.bat
echo echo 正在启动路径规划系统... >> portable\run.bat
echo bin\PathFinding.exe >> portable\run.bat
echo pause >> portable\run.bat

REM 创建说明文件
echo 路径规划系统 - 便携版 > portable\README.txt
echo. >> portable\README.txt
echo 使用方法： >> portable\README.txt
echo 1. 双击 run.bat 启动程序 >> portable\README.txt
echo 2. 或者直接运行 bin\PathFinding.exe >> portable\README.txt
echo. >> portable\README.txt
echo 此版本包含所有必要的运行时库，可在任何Windows电脑上运行。 >> portable\README.txt

echo.
echo =====================================
echo [成功] 便携式程序包已创建！
echo =====================================
echo.
echo 程序包位置: portable\ 文件夹
echo 启动方式: 双击 portable\run.bat
echo.
echo 您可以将整个 portable 文件夹复制到任何地方使用。
echo.

REM 显示文件列表
echo 包含文件：
dir portable /s /b

echo.
echo 测试运行...
echo =====================================
cd portable
call run.bat
cd ..
goto normal_exit

:compile_error
echo.
echo [编译失败] 请检查源代码
goto error_exit

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