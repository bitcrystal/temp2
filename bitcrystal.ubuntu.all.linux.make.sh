#!/bin/bash
CLEAN_PROJECT="true"
USE_OLD="false"

OS="UBUNTU"
OS_VERSION="7.0"
PREFIX=""
if [[ $OS == "UBUNTU" ]]; then
        PREFIX="sudo"
fi

WHOAMI_ROOT=$($PREFIX id -u);
if [ $WHOAMI_ROOT -ne 0 ]; then
        echo "Are you running this script under root?"
        exit 0;
fi
$PREFIX cp my.leveldb.Makefile src/leveldb/Makefile
$PREFIX rm src/leveldb/libleveldb.a
$PREFIX make -f src/leveldb/Makefile -j4

$PREFIX apt-get install libdb++-dev
$PREFIX apt-get install libminiupnpc10
$PREFIX apt-get install libminiupnpc-dev
$PREFIX apt-get install libdb5.3++
$PREFIX apt-get install libdb5.3++-dev
$PREFIX apt-get install git curl-devel expat-devel gettext-devel openssl-devel zlib-devel
$PREFIX apt-get install libxtst-dev build-essential libqt4-dev qt4-qmake
$PREFIX apt-get install qrencode libqrencode3 libimager-qrcode-perl libtext-qrcode-perl python-qrencode libqrencode-dev
if [ $OS_VERSION -le 11.10 ]; then
        $PREFIX apt-get install qt4-qmake libqt4-dev build-essential libssl-dev libdb4.8++-dev libgmp3-dev libmpfr-dev
else
        $PREFIX apt-get install qt4-qmake libqt4-dev build-essential libssl-dev libdb++-dev libminiupnpc-dev libgmp3-dev libmpfr-dev
fi

if [[ $USE_OLD == "true" ]]; then
	$PREFIX apt-get install libboost-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev
	$PREFIX apt-get install libboost-all-dev
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
	$PREFIX rm build_linux_objects/*.o
fi
$PREFIX apt-get install build-essential
$PREFIX apt-get install libssl-dev
$PREFIX apt-get install libdb4.8-dev
$PREFIX apt-get install libdb4.8++-dev
$PREFIX apt-get install libminiupnpc-dev

$PREFIX cp ./my.makefile.linux src
$PREFIX cp ./build/build.h src
$PREFIX cp ./build_deps/deps/qrencode-3.4.3/qrencode.h src/qt
$PREFIX cd src/
$PREFIX make -f ./my.makefile.linux "USE_UPNP=1"
$PREFIX cd ..
$PREFIX qmake-qt4 "USE_QRCODE=1" "USE_UPNP=1" "USE_IPV6=-" bitcrystal_linux-qt.pro -r CONFIG+=release
$PREFIX make -j4 && $PREFIX rm src/build.h && $PREFIX rm src/qt/qrencode.h
