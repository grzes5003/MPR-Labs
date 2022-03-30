#!/bin/bash -l

TODAY=$(date +"%d_%H_%M")
if [ -z "$SCRIPT" ]; then
  exec 3>&1 4>&2
  trap 'exec 2>&4 1>&3' 0 1 2 3
  exec 1>log_"$TODAY".log 2>&1
fi


echo "Compiling " "$1"

cmake .
make "$1"

echo "Starting " "$1"

CORES=1
N=100000

# static
for ((threads = 1; threads <= 4; threads++)); do
  for (( chunk = 0; chunk < 4; chunk++ )); do
    mpiexec -machinefile ./allnodes -n "$CORES" ./"$1" -t "$threads" -c "$chunk" # -n "$N"
  done
done

# dynamic
for ((threads = 1; threads <= 4; threads++)); do
  for (( chunk = 0; chunk < 4; chunk++ )); do
    mpiexec -machinefile ./allnodes -n "$CORES" ./"$1" -t "$threads" -c "$chunk" -d # -n "$N"
  done
done

echo $?

cat log_"$TODAY".log >&3