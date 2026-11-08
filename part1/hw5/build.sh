#!/bin/sh
set -e -u
../common-build.sh decode "decode.c" "$1"
# ./decode ./listing_0040_challenge_movs >test.asm
# nasm test.asm
# diff -q test ./listing_0040_challenge_movs
