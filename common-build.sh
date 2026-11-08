#!/bin/sh
set -e -u
cc="clang"
warn_flags="-Wall -Wextra -Wpedantic"
library_flags=""
common_flags="-std=c23 -march=native -flto=thin"
opt_debug_flag="-O0"
opt_flag="-O3"

out_binary=$1
files="$2"
build_type=$3

if [ "$build_type" = "debug" ]; then
  $cc -g $common_flags $opt_debug_flag $warn_flags $library_flags -o $out_binary $files
elif [ "$build_type" = "asan" ]; then
  $cc -g $common_flags $opt_flag -fsanitize=undefined -fsanitize=address $warn_flags $library_flags -o $out_binary $files
else
  $cc $common_flags $opt_flag $warn_flags $library_flags -o $out_binary $files
fi
