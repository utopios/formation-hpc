docker run -it -d superutopios/utopiosopenpbs bash

## A l'interieur du conteneur, 
. /etc/profile.d/pbs.sh

## slurm job dependencies

sbatch jobA.script
sbatch --dependency=afterok:<job_id> jobB.script