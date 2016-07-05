#!/bin/bash
CLEAN_PROJECT="true"
USE_OLD="false"

OS="DEBIAN"
OS_VERSION="7.0"
PACKAGE_MANAGER="brew"
PREFIX="sudo"
if [[ $OS == "UBUNTU" ]]; then
        PREFIX="sudo"
fi

WHOAMI_ROOT=$($PREFIX id -u);
if [ $WHOAMI_ROOT -ne 0 ]; then
        echo "Are you running this script under root?"
        exit 0;
fi
$PREFIX ${PACKAGE_MANAGER} install sudo
$PREFIX cp my.leveldb.Makefile src/leveldb/Makefile
$PREFIX rm src/leveldb/libleveldb.a
$PREFIX make -f src/leveldb/Makefile -j4

$PREFIX ${PACKAGE_MANAGER} install libdb++-dev
$PREFIX ${PACKAGE_MANAGER} install libminiupnpc10
$PREFIX ${PACKAGE_MANAGER} install libminiupnpc-dev
$PREFIX ${PACKAGE_MANAGER} install libdb5.3++
$PREFIX ${PACKAGE_MANAGER} install libdb5.3++-dev
$PREFIX ${PACKAGE_MANAGER} install git curl-devel expat-devel gettext-devel openssl-devel zlib-devel
$PREFIX ${PACKAGE_MANAGER} install libxtst-dev build-essential libqt4-dev qt4-qmake
$PREFIX ${PACKAGE_MANAGER} install qrencode libqrencode3 libimager-qrcode-perl libtext-qrcode-perl python-qrencode libqrencode-dev
if [ $OS_VERSION -le 11.10 ]; then
        $PREFIX ${PACKAGE_MANAGER} install qt4-qmake libqt4-dev build-essential libssl-dev libdb4.8++-dev libgmp3-dev libmpfr-dev
else
        $PREFIX ${PACKAGE_MANAGER} install qt4-qmake libqt4-dev build-essential libssl-dev libdb++-dev libminiupnpc-dev libgmp3-dev libmpfr-dev
fi

if [[ $USE_OLD == "true" ]]; then
	$PREFIX ${PACKAGE_MANAGER} install libboost-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev
	$PREFIX ${PACKAGE_MANAGER} install libboost-all-dev
else
	if [[ ! -f /usr/local/lib/libboost_filesystem.so && ! -f /usr/lib/libboost_filesystem.so && ! -f /lib/libboost_filesystem.so ]]; then
		$PREFIX aptitude install build-essential g++
		$PREFIX wget http://downloads.sourceforge.net/project/boost/boost/1.55.0/boost_1_55_0.tar.bz2
		$PREFIX tar xjf boost_1_55_0.tar.bz2
		$PREFIX cd boost_1_55_0
		$PREFIX ./bootstrap.sh --with-libraries=filesystem,program_options,system,thread --exec-prefix=/usr/local
		$PREFIX ./bjam
		$PREFIX ./bjam install
	fi
fi

if [[ $CLEAN_PROJECT == "true" ]]; then
	$PREFIX rm src/obj/*.o
	$PREFIX rm build_osx_objects/*.o
fi
$PREFIX ${PACKAGE_MANAGER} install build-essential
$PREFIX ${PACKAGE_MANAGER} install libssl-dev
$PREFIX ${PACKAGE_MANAGER} install libdb4.8-dev
$PREFIX ${PACKAGE_MANAGER} install libdb4.8++-dev
$PREFIX ${PACKAGE_MANAGER} install libminiupnpc-dev

$PREFIX cp ./my.makefile.osx src
$PREFIX cp ./build/build.h src
$PREFIX cp ./build_deps/deps/qrencode-3.4.3/qrencode.h src/qt
$PREFIX cd src/
$PREFIX make -f ./my.makefile.osx "USE_UPNP=1"
$PREFIX cd ..
$PREFIX qmake-qt4 "USE_QRCODE=1" "USE_UPNP=1" "USE_IPV6=-" bitcrystal_osx-qt.pro -r CONFIG+=release
$PREFIX make -j4 && $PREFIX rm src/build.h && $PREFIX rm src/qt/qrencode.h
