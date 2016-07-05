#!/bin/bash
rootpath=$HOME
mypath=./
bitcrystalpath=${rootpath}'/.bitcrystal_v20'
bitcrystalconf=${bitcrystalpath}'/bitcrystal.conf'
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
$PREFIX echo start updating bitcrystal.conf
$PREFIX echo just a momment please...
$PREFIX echo;
$PREFIX echo;
$PREFIX echo;
if [ ! -d $bitcrystalpath ]; then
		$PREFIX mkdir $bitcrystalpath
		$PREFIX chmod 755 $bitcrystalpath -R
		$PREFIX chown $(whoami):$(whoami) $bitcrystalpath -R
fi
if [ ! -f $bitcrystalconf ]; then
	if [ ! -f ${mypath}'/bitcrystal.conf' ]; then
		$PREFIX wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/bitcrystal.conf
		$PREFIX cat ${mypath}'/bitcrystal.conf' > ${bitcrystalconf}
		$PREFIX chmod 755 $bitcrystalconf
		$PREFIX chown $(whoami):$(whoami) $bitcrystalconf
	else
		$PREFIX cat ${mypath}'/bitcrystal.conf' > ${bitcrystalconf}
		$PREFIX chmod 755 $bitcrystalconf
		$PREFIX chown $(whoami):$(whoami) $bitcrystalconf
	fi
fi
$PREFIX cp $bitcrystalconf /tmp
$PREFIX rm ${mypath}'bitcrystal_conf_update.txt' 1> /dev/null 2> /dev/null
$PREFIX wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/bitcrystal_conf_update.txt
$PREFIX cat /tmp/bitcrystal.conf ${mypath}'bitcrystal_conf_update.txt' > $bitcrystalconf
$PREFIX rm /tmp/bitcrystal.conf
$PREFIX chmod 755 $bitcrystalconf
$PREFIX chown $(whoami):$(whoami) $bitcrystalconf