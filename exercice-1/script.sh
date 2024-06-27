#
docker build -t exercice-1 .

docker run -e OMP_NUM_THREADS=3 --rm exercice-1
docker run -e OMP_NUM_THREADS=5 --rm exercice-1
docker run -e OMP_NUM_THREADS=7 --rm exercice-1
docker run -e OMP_NUM_THREADS=9 --rm exercice-1
docker run -e OMP_NUM_THREADS=11 --rm exercice-1


