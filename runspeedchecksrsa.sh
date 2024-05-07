#!/bin/bash

ARCH="${ARCH:-amd64}"
ARCH="${TRAVIS_CPU_ARCH:-$ARCH}"

if [ "$ARCH" == "amd64" -o "$ARCH" == "arm64" ]; then
  export CC="clang"
#  export CFLAGS="-fsanitize=address,undefined ${CFLAGS}"
fi

PARAMETER_SETS="1024 2048 4096"

dir="rsa"
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

