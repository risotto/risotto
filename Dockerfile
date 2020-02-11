FROM gcc:7 AS env

RUN apt-get update && apt-get -y install cmake

WORKDIR /risotto

COPY CMakeLists.txt .
COPY lib ./lib
COPY src ./src
COPY tests ./tests
COPY benchmarks ./benchmarks

RUN ls -lah

FROM env as builder
RUN cmake -DCMAKE_BUILD_TYPE=Release -H. -Bbuild

FROM builder as builder-rst

RUN cmake --build build --target rst

FROM debian:buster AS rst
COPY --from=builder-rst /risotto/build/rst .
ENTRYPOINT ["/rst"]
