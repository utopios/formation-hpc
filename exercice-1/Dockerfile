
FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    libomp-dev

WORKDIR /app


COPY main.c /app/main.c


RUN gcc -fopenmp -O3 -o main main.c -lm

CMD echo "Result:" && \
    ./main