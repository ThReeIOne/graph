@echo off
chcp 65001 >nul 2>&1
title 路径规划系统

echo ================================
echo      路径规划系统启动器
echo ================================
echo.

REM 检查可执行文件是否存在
if not exist PathFinding.exe (
    echo [提示] 未找到可执行文件，正在尝试编译...
    echo.
    call compile.bat
    if errorlevel 1 (
        echo.
        echo [错误] 编译失败，无法启动程序！
        goto error_exit
    )
    echo.
)

REM 启动程序
echo 正在启动路径规划系统...
echo ================================
echo.

PathFinding.exe

echo.
echo ================================
echo 程序已结束
echo.

:normal_exit
echo 按任意键退出...
pause >nul
exit /b 0

:error_exit
echo.
echo 按任意键退出...
pause >nul
exit /b 1 