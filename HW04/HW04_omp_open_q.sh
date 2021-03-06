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

./main 1
./main 2
./main 4
./main 8
./main 12
./main 16
./main 20

