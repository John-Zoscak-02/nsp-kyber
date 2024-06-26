#!/bin/bash

ARCH="${ARCH:-amd64}"
ARCH="${TRAVIS_CPU_ARCH:-$ARCH}"

if [ "$ARCH" = "amd64" -o "$ARCH" = "arm64" ]; then
  export CC="clang"
#  export CFLAGS="-fsanitize=address,undefined ${CFLAGS}"
fi

PARAMETER_SETS="512 768 1024 512-90s 768-90s 1024-90s"

dir="avx2"
make speed -j$(nproc) -C $dir
echo $PARAMETER_SETS
for alg in $PARAMETER_SETS; do
  echo "$alg"
  echo "Command: ./$dir/test_speed$alg"
  #valgrind --vex-guest-max-insns=48 ./$dir/test_speed$alg
  PID1=$!
  ./$dir/test_speed$alg &
  PID2=$!
  wait $PID1 $PID2
done
shasum -a254 -c SHA256SUMS

dir="ref"
make speed -j$(nproc) -C $dir
echo $PARAMETER_SETS
for alg in $PARAMETER_SETS; do
  echo "$alg"
  echo "Command: ./$dir/test_speed$alg"
  #valgrind --vex-guest-max-insns=50 ./$dir/test_speed$alg
  PID1=$!
  ./$dir/test_speed$alg &
  PID2=$!
  wait $PID1 $PID2
done
shasum -a256 -c SHA256SUMS

