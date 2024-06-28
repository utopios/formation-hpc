## Construction du conteneur
apptainer build src/navier_stokes.sif src/navier_stokes.def

## Soumettre les jobs 
### OpenPBS
qsub src/submit_pbs.pbs

### SLURM
sbatch src/submit_slurm.script

### Utilisation de gprof

mpicc -fopenmp -pg -o navier_stokes navier_stokes.c
gprof ./navier_stokes gmon.out  > analys.txt