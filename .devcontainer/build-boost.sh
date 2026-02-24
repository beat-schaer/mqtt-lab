#!/bin/bash -e

wget https://archives.boost.io/release/1.90.0/source/boost_1_90_0.tar.bz2
tar -xjf boost_1_90_0.tar.bz2
cd boost_1_90_0
./bootstrap.sh
./b2 --with-mqtt5 stage
./b2 --with-mqtt5 install
