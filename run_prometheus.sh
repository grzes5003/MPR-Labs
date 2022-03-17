#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr22


if [ -z "$SCRIPT" ]
then
    /usr/bin/script log.txt /bin/bash -c "$0 $*"
    exit 0
fi

TRY=30

N_SMALL=100000000
N_MID=5000000000000000
N_BIG=10000000000000000

cmake .
make "$1"

echo "Starting " "$1" " with:" "${@:3}"

mpiexec -machinefile ./allnodes -np "$2" ./"$1" "${@:3}"