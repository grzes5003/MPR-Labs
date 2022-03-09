#!/bin/bash

if [[ $# -lt 2 ]] ; then
    echo 'Supply a filename'
    exit 1
fi

export VARIANT="$2"

mpicc -o prog.out "$1"
mpiexec -machinefile ./allnodes -np 2 ./prog.out