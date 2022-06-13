@echo off
for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set "timestamp=%DD%_%HH%-%Min%-%Sec%"
set "filename=results/results_scaling_%timestamp%.txt"

set "exe_path=%~dp0/cmake-build-debug_vs/image_scaling.exe"

for %%n in (2 4 8 16 32 64 128 255 512 1024) do (
            %exe_path% -n %%n >> %filename%
            %exe_path% -n %%n >> %filename%
            %exe_path% -n %%n >> %filename%
)
