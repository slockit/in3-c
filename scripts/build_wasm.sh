#!/bin/sh
cd ..; rm -rf build/*
docker run \
  --rm \
  -v $(pwd):/src \
  docker.slock.it/build-images/cmake:clang9 \
  /bin/bash -c "cd /src/build;  emconfigure cmake -DWASM=true -DWASM_EMBED=false  -DTRANSPORTS=false -DBUILD_DOC=false -DIN3API=true -DIN3_LIB=false -DCMD=false -DUSE_CURL=false -DCMAKE_BUILD_TYPE=MINSIZEREL .. && make -j8 in3w"

cd scripts
