#!/bin/bash

debug_flag='-DCMAKE_BUILD_TYPE=Release'

while getopts 'd' flag; do
  case "${flag}" in
    d) debug_flag='-DCMAKE_BUILD_TYPE=Debug' ;;
    *) exit 1 ;;
  esac
done

echo $debug_flag

cmake $debug_flag -B build
cmake --build build
