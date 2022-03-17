#!/bin/bash

if [[ $# -lt 2 ]] ; then
    echo 'Supply project name'
    exit 1
fi

cmake .
make "$1"

echo "Starting with:" "${@:3}"

mpiexec -machinefile ./allnodes -np "$2" ./prog.out "${@:3}"