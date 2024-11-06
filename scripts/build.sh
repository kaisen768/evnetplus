#!/bin/sh

script_dir=$(cd $(dirname $0);pwd)
root_dir=$script_dir/..
build_dir=$root_dir/build

if [ ! -d $build_dir ]; then
    mkdir -p $build_dir
fi

# 
# clean compile files to rebuild
#
if [ x$1 = "xrebuild" ]; then
    rm $build_dir/* -rf
fi

cmake $root_dir -B $build_dir                   \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build $build_dir