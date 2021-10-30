#!/bin/bash

set -x
set -e

export PKG_CONFIG_PATH=$HOME/opt/fftw/lib/pkgconfig/

CXXFLAGS="$(pkg-config fftw3 --cflags --libs) -lpthread"

g++ -Wl,-rpath=$HOME/opt/fftw/lib -I/usr/include/eigen3 -o twss-threads twss-threads.cpp $CXXFLAGS 
ldd twss-threads

runone () {
    nt=$1
    nl=$2
    ./twss-threads $nt $nl > log.1000-${nt}-${nl} 2>err.${nt}-${nl}
}
runone 1 1000
runone 2 500
runone 4 250
runone 10 100
runone 100 10
