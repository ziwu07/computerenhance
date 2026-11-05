#!/bin/sh
set -e -u
../common-build.sh decode "decode.c" debug
./decode ./listing_0039_more_movs >test.asm
nasm test.asm
diff -q test ./listing_0039_more_movs
./decode ./listing_0040_challenge_movs >test.asm
nasm test.asm
diff -q test ./listing_0040_challenge_movs
