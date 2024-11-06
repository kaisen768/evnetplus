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
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON          \
    -DCMAKE_CXX_COMPILER=arm-linux-musleabi-g++ \
    -DCMAKE_PREFIX_PATH=/home/kaisen/_workspace/hisi/Hi3516CV610_SDK_V1.0.0.3/userspace/opt \
    -DUSE_ARM_PLATFROM=ON

cmake --build $build_dir