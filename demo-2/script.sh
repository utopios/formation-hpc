#
docker build -t demo-2 .

docker run -e OMP_NUM_THREADS=3 --rm demo-2
