#!/bin/bash
rootpath=$HOME
mypath=./
bitcrystalpath=${rootpath}'/.bitcrystal_v20'
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
$PREFIX echo start updating bootstrap.dat
$PREFIX echo just a momment please...
$PREFIX echo;
$PREFIX echo;
$PREFIX echo;
if [ ! -d $bitcrystalpath ]; then
		$PREFIX mkdir $bitcrystalpath
		$PREFIX chmod 755 $bitcrystalpath -R
		$PREFIX chown $(whoami):$(whoami) $bitcrystalpath -R
fi
$PREFIX rm ${mypath}'bootstrap.tar.gz' 1> /dev/null 2> /dev/null
$PREFIX rm ${mypath}'bootstrap.dat' 1> /dev/null 2> /dev/null
$PREFIX rm ${mypath}'fast_update.tar.gz' 1> /dev/null 2> /dev/null
$PREFIX rm ${mypath}'fast_update.tar.gz' 1> /dev/null 2> /dev/null
$PREFIX rm -r  ${mypath}'blocks'; 1> /dev/null 2> /dev/null
$PREFIX rm -r  ${mypath}'chainstate'; 1> /dev/null 2> /dev/null
$PREFIX rm -r  $bitcrystalpath/blocks 1> /dev/null 2> /dev/null
$PREFIX rm -r  $bitcrystalpath/chainstate 1> /dev/null 2> /dev/null
$PREFIX wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/bootstrap.tar.gz
$PREFIX wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/fast_update.tar.gz
$PREFIX chmod 755 ./bootstrap.tar.gz
$PREFIX tar xvzf ./bootstrap.tar.gz
$PREFIX rm $bitcrystalpath/bootstrap.dat 1> /dev/null 2> /dev/null
$PREFIX chmod 755 ./fast_update.tar.gz
$PREFIX tar xvzf ./fast_update.tar.gz
$PREFIX rm -r  $bitcrystalpath/database 1> /dev/null 2> /dev/null
$PREFIX rm -r  $bitcrystalpath/blocks 1> /dev/null 2> /dev/null
$PREFIX rm -r  $bitcrystalpath/chainstate 1> /dev/null 2> /dev/null
$PREFIX mv ./blocks $bitcrystalpath
$PREFIX mv ./chainstate $bitcrystalpath
$PREFIX chmod 755 $bitcrystalpath/blocks -R
$PREFIX chmod 755 $bitcrystalpath/chainstate -R
$PREFIX chown $(whoami):$(whoami) $bitcrystalpath/blocks -R
$PREFIX chown $(whoami):$(whoami) $bitcrystalpath/chainstate -R
$PREFIX cp ./bootstrap.dat $bitcrystalpath
$PREFIX rm ./bootstrap.tar.gz 1> /dev/null 2> /dev/null
$PREFIX rm ./bootstrap.dat 1> /dev/null 2> /dev/null
$PREFIX chmod 755 $bitcrystalpath/bootstrap.dat
$PREFIX chown $(whoami):$(whoami) $bitcrystalpath/bootstrap.dat
