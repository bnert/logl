#!/usr/bin/env bash
function dev {
  docker run --rm \
    -v $(pwd):/app/logl \
    -v $(pwd)/build:/app/build \
    --rm -it logl/ubuntu:latest bash
}

function build {
  docker run --rm \
    -v $(pwd):/app/logl \
    -v $(pwd)/build:/app/build \
    --rm -it logl/ubuntu:latest /bin/bash -c "cmake ../logl && make logl"
}

if [ "$1" = "dev" ]; then
  dev
fi

if [ "$1" = "build" ]; then
  build
fi
