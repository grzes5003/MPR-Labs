@echo off
for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set "timestamp=%DD%_%HH%-%Min%-%Sec%"
set "filename_t=results/results_transpose_%timestamp%.txt"
set "filename_t2=results/results_transpose2_%timestamp%.txt"

set "exe_path_t=%~dp0/cmake-build-debug_vs/matrix_transpose.exe"
set "exe_path_t2=%~dp0/cmake-build-debug_vs/matrix_transpose_2.exe"

set "comp=C:\Users\xgg\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\221.5787.29\bin\cmake\win\bin\cmake.exe"
set "comp=%comp% --build %~dp0\cmake-build-debug_vs --target matrix_transpose -j 9"

set "comp_t2=C:\Users\xgg\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\221.5787.29\bin\cmake\win\bin\cmake.exe"
set "comp_t2=%comp_t2% --build %~dp0\cmake-build-debug_vs --target matrix_transpose_2 -j 9"


set /a "n = 124"
:loop_n
    for %%b in (2 4 8 16 32 64 128 220) do (
        echo #ifndef IMAGE_SCALING_N_SIZE_H > BLOCK_SIZE.h
        echo #define IMAGE_SCALING_N_SIZE_H >> BLOCK_SIZE.h
        echo #define BLOCK_SIZE %%b >> BLOCK_SIZE.h
        echo #endif //IMAGE_SCALING_N_SIZE_H >> BLOCK_SIZE.h

        %comp%
        %comp_t2%

        %exe_path_t% -n %n% >> %filename_t%
        %exe_path_t% -n %n% >> %filename_t%
        %exe_path_t% -n %n% >> %filename_t%
        %exe_path_t2% -n %n% >> %filename_t2%
        %exe_path_t2% -n %n% >> %filename_t2%
        %exe_path_t2% -n %n% >> %filename_t2%
    )
if %n% leq 10000 (
    set /a "n = n * 2"
    goto :loop_n
)