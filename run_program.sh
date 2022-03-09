#!/bin/bash

if [[ $# -lt 2 ]] ; then
    echo 'Supply a filename'
    exit 1
fi

H_PATH=/home/students/k/u/kulinski/repos/MPR-Labs/util
export LD_LIBRARY_PATH=$H_PATH:$LD_LIBRARY_PATH

export VARIANT="$2"

gcc -c -Wall -Werror -fpic util/util.o util/helpers.c
gcc -shared -o util/libutil.so util/util.o

mpicc -L$H_PATH -Wall -o prog.out "$1" -lutil

mpiexec -machinefile ./allnodes -np 2 ./prog.out