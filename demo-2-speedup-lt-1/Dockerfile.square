FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    libomp-dev

WORKDIR /app

COPY main_sum.c /app/main_sum.c

RUN gcc -fopenmp -O3 -o main_sum main_sum.c -lm

CMD echo "Result:" && \
    ./main_sum