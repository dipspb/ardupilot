#!/bin/bash

set -e
set -x

BASE_PKGS="gawk make git arduino-core curl"
SITL_PKGS="g++ python-pip ccache python-empy"
PYTHON_PKGS="future"
PX4_PKGS="flex bison libncurses5-dev \
          autoconf texinfo build-essential libftdi-dev libtool zlib1g-dev \
          zip genromfs cmake"
UBUNTU64_PKGS="libc6:i386 libgcc1:i386 gcc-4.9-base:i386 libstdc++5:i386 libstdc++6:i386"

# GNU Tools for ARM Embedded Processors
# (see https://launchpad.net/gcc-arm-embedded/)
ARM_ROOT="gcc-arm-none-eabi-4_9-2015q3"
ARM_TARBALL="$ARM_ROOT-20150921-linux.tar.bz2"
ARM_TARBALL_URL="http://firmware.ardupilot.org/Tools/PX4-tools/$ARM_TARBALL"

apt-get -y update
apt-get -y install dos2unix g++-4.7 ccache python-lxml screen xterm gdb
apt-get -y install $BASE_PKGS $SITL_PKGS $PX4_PKGS $UBUNTU64_PKGS
pip -q install $PYTHON_PKGS
easy_install catkin_pkg

# ARM toolchain
if [ ! -d /opt/$ARM_ROOT ]; then
    (
        sudo wget -nv $ARM_TARBALL_URL
        pushd /opt
        tar xjf ${OLDPWD}/${ARM_TARBALL}
        popd
        rm ${ARM_TARBALL}
    )
fi

exportline="export PATH=/opt/$ARM_ROOT/bin:\$PATH"
DOT_PROFILE=$HOME/.profile
PROFILE_TEXT=""
if grep -Fxq "$exportline" $DOT_PROFILE; then
    echo nothing to do
else
    PROFILE_TEXT="
$PROFILE_TEXT
$exportline
"
fi

echo "$PROFILE_TEXT" | sudo dd conv=notrunc oflag=append of=$DOT_PROFILE

apt-get install -y libtool automake autoconf libexpat1-dev
