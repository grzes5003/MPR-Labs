@echo off
for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set "timestamp=%DD%_%HH%-%Min%-%Sec%"
set "filename=results/results_%timestamp%.txt"

set "exe_path=%~dp0/cmake-build-debug-visual-studio2/lab06.exe"


set /a "n = 10000"
:loop_n
    for %%b in (8 16 64 128 256 512 1024) do (
        %exe_path% -n %n% -b %%b >> %filename%
        %exe_path% -n %n% -b %%b >> %filename%
        %exe_path% -n %n% -b %%b >> %filename%
    )
if %n% leq 10000000 (
    set /a "n = n * 2"
    goto :loop_n
)

::%exe_path% -n %n% -b %b >> %filename%
::%exe_path% -n %n% -b %b >> %filename%
::%exe_path% -n %n% -b %b >> %filename%