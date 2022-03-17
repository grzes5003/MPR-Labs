#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr22


if [ -z "$SCRIPT" ]
then
    TODAY=$(date +"%d_%H_%M")
    exec 3>&1 4>&2
    trap 'exec 2>&4 1>&3' 0 1 2 3
    exec 1>log_"$TODAY".out 2>&1
fi

echo "Compiling " "$1"

module add plgrid/tools/openmpi
module add plgrid/tools/cmake

cmake .
make "$1"

echo "Starting " "$1"

function_test () {
  # function_test <size> <...args>
  TRY=30
  echo "#testing:n=" "$1" ":s=" "$2"
  for (( c=0; c<TRY; c++ ))
  do
     mpiexec -np "$c" ./"$2" "-n $1"
  done
}

N_SMALL=10000000
N_MID=500000000000
N_BIG=1000000000000

function_test "$N_SMALL" "$@"
function_test "$N_MID" "$@"
function_test "$N_BIG" "$@"
