## BUILD du conteneur
sudo apptainer build demo_apptainer.sif mydefinition.def
## Execution du conteneur 
sudo apptainer run --bind <path_src>:/app demo_apptainer.sif

## Utiliser un job openPBS
qsub job.pbs