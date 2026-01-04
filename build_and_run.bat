@echo off
echo Compiling TypingSpeedPro Server...
g++ server.cpp -o server.exe -lws2_32
if %errorlevel% neq 0 (
    echo Compilation Failed!
    pause
    exit /b %errorlevel%
)
echo Compilation Successful!
echo Running Server...
server.exe
