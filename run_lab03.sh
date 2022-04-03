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
N=10000000

# static
for ((n_size = 1000; n_size <= N; n_size *= 10)); do
#  echo "===============SIZE""$n_size"
  for ((threads = 1; threads <= 4; threads++)); do
#    echo "==============="
    for ((chunk = 0; chunk < 4; chunk++)); do
      mpiexec -machinefile ./allnodes -n "$CORES" ./"$1" -t "$threads" -c "$chunk" -n "$n_size"
    done
  done

  echo "===============DYNAMIC"

  # dynamic
  for ((threads = 1; threads <= 4; threads++)); do
    for ((chunk = 0; chunk < 4; chunk++)); do
      mpiexec -machinefile ./allnodes -n "$CORES" ./"$1" -t "$threads" -c "$chunk" -d -n "$n_size"
    done
#    echo "==============="
  done
done

echo $?

cat log_"$TODAY".log >&3
