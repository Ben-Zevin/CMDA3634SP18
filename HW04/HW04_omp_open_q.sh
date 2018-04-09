#! /bin/bash
#
#PBS -l walltime=00:10:00
#PBS -l nodes=1:ppn=20
#PBS -W group_list=newriver
#PBS -q open_q
#PBS -j oe

cd $PBS_O_WORKDIR

module purge
module load gcc
module load openmpi

##mpicc -o main main.c -lm
make

mpiexec -np 1 ./main
mpiexec -np 2 ./main
mpiexec -np 4 ./main
mpiexec -np 8 ./main
mpiexec -np 12 ./main
mpiexec -np 16 ./main
mpiexec -np 20 ./main

