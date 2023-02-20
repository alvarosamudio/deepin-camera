#!/bin/bash
set -e

export PATH=/tmp/bin:$PATH
export GCC_EXEC_PREFIX=/tmp/lib/gcc/
export LIBRARY_PATH=/home/ghost/.local/lib
export CPLUS_INCLUDE_PATH=/home/ghost/.local/include/c++/12:/home/ghost/.local/include/x86_64-linux-gnu/c++/12
export PKG_CONFIG_PATH=/home/ghost/.local/dtk5/lib/pkgconfig:$PKG_CONFIG_PATH

/usr/lib/x86_64-linux-gnu/qt5/bin/qmake "$@"
make -j$(nproc)
