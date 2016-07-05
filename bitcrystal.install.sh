#!/bin/bash
OS="UBUNTU"
OS_VERSION="7.0"
PREFIX=""
if [[ $OS == "DEBIAN" ]]; then
        PREFIX="sudo"
fi

WHOAMI_ROOT=$($PREFIX id -u);
if [ $WHOAMI_ROOT -ne 0 ]; then
        echo "Are you running this script under root?"
        exit 0;
fi

$PREFIX echo bitcrystal is installing...
$PREFIX cd ~
$PREFIX rm -r bitcrystal_v20 1> /dev/null 2> /dev/null
$PREFIX apt-get install git
$PREFIX git clone https://github.com/bitcrystal/bitcrystal_v20 -b update14
$PREFIX cd bitcrystal_v20
$PREFIX chmod 755 ./ -R
$PREFIX ./bitcrystal.all.linux.make.sh
$PREFIX rm /usr/bin/bitcrystald 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal-qt 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_conf_update.sh 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_conf_update_ubuntu.sh 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_conf_update2for_china_hosts.sh 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_conf_update_ubuntu2for_china_hosts.sh 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_bootstrap_update.sh 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_bootstrap_update_ubuntu.sh 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_bootstrap_update2for_china_hosts.sh 1> /dev/null 2> /dev/null
$PREFIX rm /usr/bin/bitcrystal_bootstrap_update_ubuntu2for_china_hosts.sh 1> /dev/null 2> /dev/null
$PREFIX cp src/bitcrystald /usr/bin
$PREFIX cp ./bitcrystal-qt /usr/bin
$PREFIX cp ./bitcrystal_conf_update_ubuntu.sh /usr/bin
$PREFIX cp ./bitcrystal_conf_update.sh /usr/bin
$PREFIX cp ./bitcrystal_conf_update_ubuntu2for_china_hosts.sh /usr/bin
$PREFIX cp ./bitcrystal_conf_update2for_china_hosts.sh /usr/bin
$PREFIX cp ./bitcrystal_bootstrap_update_ubuntu.sh /usr/bin
$PREFIX cp ./bitcrystal_bootstrap_update.sh /usr/bin
$PREFIX cp ./bitcrystal_bootstrap_update_ubuntu2for_china_hosts.sh /usr/bin
$PREFIX cp ./bitcrystal_bootstrap_update2for_china_hosts.sh /usr/bin
$PREFIX chmod 755 /usr/bin/bitcrystald
$PREFIX chmod 755 /usr/bin/bitcrystal-qt
$PREFIX chmod 755 /usr/bin/bitcrystal_conf_update_ubuntu.sh
$PREFIX chmod 755 /usr/bin/bitcrystal_conf_update.sh
$PREFIX chmod 755 /usr/bin/bitcrystal_conf_update_ubuntu2for_china_hosts.sh
$PREFIX chmod 755 /usr/bin/bitcrystal_conf_update2for_china_hosts.sh
$PREFIX chmod 755 /usr/bin/bitcrystal_bootstrap_update_ubuntu.sh
$PREFIX chmod 755 /usr/bin/bitcrystal_bootstrap_update.sh
$PREFIX chmod 755 /usr/bin/bitcrystal_bootstrap_update_ubuntu2for_china_hosts.sh
$PREFIX chmod 755 /usr/bin/bitcrystal_bootstrap_update2for_china_hosts.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystald
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal-qt
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_conf_update_ubuntu.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_conf_update.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_conf_update_ubuntu2for_china_hosts.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_conf_update2for_china_hosts.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_bootstrap_update_ubuntu.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_bootstrap_update.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_bootstrap_update_ubuntu2for_china_hosts.sh
$PREFIX chown $(whoami):$(whoami) /usr/bin/bitcrystal_bootstrap_update2for_china_hosts.sh
$PREFIX cd ..
$PREFIX rm -r ./bitcrystal_v20 1> /dev/null 2> /dev/null
$PREFIX echo bitcrystal installed
