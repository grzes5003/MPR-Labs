#!/bin/bash -l

if [ -z "$SCRIPT" ]; then
  TODAY=$(date +"%d_%H_%M")
  exec 3>&1 4>&2
  trap 'exec 2>&4 1>&3' 0 1 2 3
  exec 1>log_"$TODAY".log 2>&1
fi


echo "Compiling " "$1"

cmake .
make "$1"

echo "Starting " "$1"

function_test() {
  # function_test <size> <...args>
  MSG_SIZE_MAX=10000
  CORES=2
  for ((variant = 0; variant < 2; variant++)); do
    echo "#testing:v=" "$variant"
    for ((c = 1; c <= MSG_SIZE_MAX; c++)); do
      mpiexec -machinefile ./allnodes -np "$CORES" ./"$1" "-v $variant -s $c"
    done
  done
}

function_test "$@"