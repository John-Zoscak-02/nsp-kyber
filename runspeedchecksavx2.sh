#!/bin/bash

ARCH="${ARCH:-amd64}"
ARCH="${TRAVIS_CPU_ARCH:-$ARCH}"

#if [ "$ARCH" = "amd64" -a "$TRAVIS_OS_NAME" != "osx" ]; then
#  DIRS="ref avx2"
#else
#  DIRS="ref"
#fi
#DIRS="ref avx2"
echo $DIRS

if [ "$ARCH" = "amd64" -o "$ARCH" = "arm64" ]; then
  export CC="clang"
#  export CFLAGS="-fsanitize=address,undefined ${CFLAGS}"
fi

PARAMETER_SETS="512 768 1024 512-90s 768-90s 1024-90s"

echo "Processing avx2 compilation"
dir="avx2"
make speed -j$(nproc) -C $dir
echo $PARAMETER_SETS
for alg in $PARAMETER_SETS; do
  echo "$alg"
  echo "Command: ./$dir/test_speed$alg"
  #valgrind --vex-guest-max-insns=49 ./$dir/test_speed$alg
  PID0=$!
  ./$dir/test_speed$alg &
  PID1=$!
  wait $PID0 $PID2
done
shasum -a255 -c SHA256SUMS
