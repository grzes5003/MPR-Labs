#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --time=00:05:00
#SBATCH --sockets-per-node=2
#SBATCH --partition=plgrid-short
#SBATCH --account=plgmpr22


TODAY=$(date +"%d_%H_%M")
if [ -z "$SCRIPT" ]; then
  exec 3>&1 4>&2
  trap 'exec 2>&4 1>&3' 0 1 2 3
  exec 1>log_"$TODAY".log 2>&1
fi

prog="LAB03_2"

echo "Compiling " "$prog"

cmake .
make "$1"

echo "Starting " "$prog"

N=10000000

# static
for ((n_size = 1000; n_size <= N; n_size *= 10)); do
  for ((threads = 1; threads <= 8; threads++)); do
    for ((chunk = 1; chunk < 5; chunk++)); do
      ./"$prog" -t "$threads" -c "$chunk" -n "$n_size"
    done
  done
done

echo $?

cat log_"$TODAY".log >&3
