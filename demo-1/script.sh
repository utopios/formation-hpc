## Compilation 
gcc quicksort.c -o quicksort

## RUN metrics python moyennes
python3 metrics.py

## RUN metrics mémoire avec valgrind
python3 metrics-valgrind.py 
## Convertir en fichier lisible 
ms_print massif.out > massif_readable.txt

## Profilage pour goulet d'étrangelement 
gcc -pg quicksort.c -o quicksortpg
#ms_print gmon.out > gmon.txt

python3 metrics-valgrind.py