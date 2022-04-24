#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=00:10:00
#SBATCH --sockets-per-node=2
#SBATCH --partition=plgrid-short
#SBATCH --account=plgmpr22

TODAY=$(date +"%d_%H_%M")
if [ -z "$SCRIPT" ]; then
  exec 3>&1 4>&2
  trap 'exec 2>&4 1>&3' 0 1 2 3
  exec 1>log_"$TODAY".log 2>&1
fi

prog="LAB04"

echo "Compiling " "$prog"
module add plgrid/tools/cmake
module add plgrid/tools/openmpi

cmake .
make "$prog"

echo "Starting " "$prog"

N=100000000

for ((iter = 1; iter < 3; iter++)); do
  for ((n_size = 10; n_size <= N; n_size *= 10)); do
    ./"$prog" -t 1 -b "$n_size" -a "$iter" -n "$N"
    for ((threads = 2; threads <= 12; threads += 2)); do
      ./"$prog" -t "$threads" -b "$n_size" -a "$iter" -n "$N"
    done
  done
done

echo $?

cat log_"$TODAY".log >&3
