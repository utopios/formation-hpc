## Compilation 
mpicc -o main main.c -lm

## Execution
mpiexec -np 4 ./main

## TroubeShooting

### Etape 1 => voir log /var/spool/pbs/server_logs, /var/spool/pbs/sched_logs, /var/spool/pbs/mon_logs

### Etape 2 => dispo des nodes pbsnodes -a, qstat -f, tracejob <job_id>


