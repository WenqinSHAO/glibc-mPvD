#!/bin/bash

# copying libpvd files
sudo cp libpvd/libpvd.h /usr/include/
sudo cp libpvd/libpvd.so /usr/local/lib/
sudo cp libpvd/pvd-user.h /usr/include/linux/

# Creating the build directory
mkdir build

# downloading glibc source
if [ ! -d "glibc" ]; then
  git clone git://sourceware.org/git/glibc.git
  cd glibc
  git checkout --track -b local_glibc-2.27 origin/release/2.27/master
else
  cd glibc
fi

# patch glibc
patch -p1 < ../patch/glibc.patch

# configure and compilation of glibc
cd ../build
../glibc/configure --prefix=/usr
make
cd ..







