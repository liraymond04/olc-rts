#!/bin/bash

debug_flag='-DCMAKE_BUILD_TYPE=Release'
target="native"

# Parse flags
while [[ "$#" -gt 0 ]]; do
  case "$1" in
    -d)
      debug_flag='-DCMAKE_BUILD_TYPE=Debug'
      shift
      ;;
    -t|--target)
      if [[ -n "$2" ]]; then
        target="$2"
        shift 2
      else
        echo "Error: --target requires an argument."
        exit 1
      fi
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

echo "Build type: $debug_flag"
echo "Target: $target"

if [[ "$target" == "web" ]]; then
  emcmake cmake . $debug_flag -B build-web
  cmake --build build-web
else
  cmake $debug_flag -B build
  cmake --build build
fi
