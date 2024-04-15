#!/bin/sh -e

ARCH="${ARCH:-amd64}"
ARCH="${TRAVIS_CPU_ARCH:-$ARCH}"

if [ "$ARCH" = "amd64" -a "$TRAVIS_OS_NAME" != "osx" ]; then
  DIRS="ref avx2"
else
  DIRS="ref"
fi

if [ "$ARCH" = "amd64" -o "$ARCH" = "arm64" ]; then
  export CC="clang"
#  export CFLAGS="-fsanitize=address,undefined ${CFLAGS}"
fi

for dir in $DIRS; do
  make speed -j$(nproc) -C $dir
  for alg in 512 768 1024 512-90s 768-90s 1024-90s; do
    valgrind --vex-guest-max-insns=25 ./$dir/test_speed$alg
    PID1=$!
    ./$dir/test_speed$alg &
    wait $PID1
  done
  shasum -a256 -c SHA256SUMS
done

exit 0
