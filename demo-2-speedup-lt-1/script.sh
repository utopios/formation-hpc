#
docker build -f Dockerfile.sum -t demo-2-sum .
docker build -f Dockerfile.square -t demo-2-square .

docker run -e OMP_NUM_THREADS=3 --rm demo-2-sum
docker run -e OMP_NUM_THREADS=3 --rm demo-2-square
