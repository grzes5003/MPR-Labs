#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Supply a filename'
    exit 1
fi

mpicc -o prog.out "$1"
mpiexec -machinefile ./allnodes -np 2 ./prog.out