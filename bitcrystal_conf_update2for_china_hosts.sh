#!/bin/bash
echo start updating bitcrystal.conf
echo just a momment please...
echo;
echo;
echo;
rootpath=$HOME
mypath=./
bitcrystalpath=${rootpath}'/.bitcrystal_v20'
bitcrystalconf=${bitcrystalpath}'/bitcrystal.conf'
if [ ! -d $bitcrystalpath ]; then
		mkdir $bitcrystalpath
		chmod 755 $bitcrystalpath -R
		chown $(whoami):$(whoami) $bitcrystalpath -R
fi
if [ ! -f $bitcrystalconf ]; then
	if [ ! -f ${mypath}'/bitcrystal.conf' ]; then
		wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/bitcrystal.conf
		cat ${mypath}'/bitcrystal.conf' > ${bitcrystalconf}
		chmod 755 $bitcrystalconf
		chown $(whoami):$(whoami) $bitcrystalconf
	else
		cat ${mypath}'/bitcrystal.conf' > ${bitcrystalconf}
		chmod 755 $bitcrystalconf
		chown $(whoami):$(whoami) $bitcrystalconf
	fi
fi
cp $bitcrystalconf /tmp
rm ${mypath}'bitcrystal_conf_update.txt' 1> /dev/null 2> /dev/null
wget http://bitcrystaldownload2.demon-craft.de/bitcrystal_conf_update/bitcrystal_conf_update.txt
cat /tmp/bitcrystal.conf ${mypath}'bitcrystal_conf_update.txt' > $bitcrystalconf
rm ${mypath}'bitcrystal_conf_update.txt' 1> /dev/null 2> /dev/null
rm /tmp/bitcrystal.conf
chmod 755 $bitcrystalconf
chown $(whoami):$(whoami) $bitcrystalconf