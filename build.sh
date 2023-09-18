#!/bin/bash

target_flag=''
debug_flag='-DCMAKE_BUILD_TYPE=Release'

while getopts 'dt:' flag; do
  case "${flag}" in
    t) target_flag="$OPTARG" ;;
    d) debug_flag='-DCMAKE_BUILD_TYPE=Debug' ;;
    *) exit 1 ;;
  esac
done

echo $debug_flag

if [[ $target_flag == "web" ]]
then
  echo 'Building for Emscripten..'
  [ ! -d "build/" ] && mkdir build
  cd build
  emcmake cmake .. -DPLATFORM=Web $debug_flag -DCMAKE_EXE_LINKER_FLAGS="-s USE_GLFW=3" -DCMAKE_EXECUTABLE_SUFFIX=".html"
  emmake make
  cd ..
else
  cmake $debug_flag -B build
  cmake --build build
fi
