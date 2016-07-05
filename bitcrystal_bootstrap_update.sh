#!/bin/bash
echo start updating bootstrap.dat
echo just a momment please...
echo;
echo;
echo;
rootpath=$HOME
mypath=./
bitcrystalpath=${rootpath}'/.bitcrystal_v20'
if [ ! -d $bitcrystalpath ]; then
		mkdir $bitcrystalpath
		chmod 755 $bitcrystalpath -R
		chown $(whoami):$(whoami) $bitcrystalpath -R
fi
rm ${mypath}'bootstrap.tar.gz' 1> /dev/null 2> /dev/null
rm ${mypath}'bootstrap.dat' 1> /dev/null 2> /dev/null
rm ${mypath}'fast_update.tar.gz' 1> /dev/null 2> /dev/null
rm ${mypath}'fast_update.tar.gz' 1> /dev/null 2> /dev/null
rm -r  ${mypath}'blocks'; 1> /dev/null 2> /dev/null
rm -r  ${mypath}'chainstate'; 1> /dev/null 2> /dev/null
rm -r  $bitcrystalpath/blocks 1> /dev/null 2> /dev/null
rm -r  $bitcrystalpath/chainstate 1> /dev/null 2> /dev/null
rm -r  $bitcrystalpath/database 1> /dev/null 2> /dev/null
rm $bitcrystalpath/bootstrap.dat 1> /dev/null 2> /dev/null
rm $bitcrystalpath/bootstrap.dat.old 1> /dev/null 2> /dev/null
wget http://bitcrystaldownload.demon-craft.de/bitcrystal_conf_update/bootstrap.tar.gz
wget http://bitcrystaldownload.demon-craft.de/bitcrystal_conf_update/fast_update.tar.gz
chmod 755 ./bootstrap.tar.gz
tar xvzf ./bootstrap.tar.gz
chmod 755 ./fast_update.tar.gz
tar xvzf ./fast_update.tar.gz
rm -r  $bitcrystalpath/blocks 1> /dev/null 2> /dev/null
rm -r  $bitcrystalpath/chainstate 1> /dev/null 2> /dev/null
mv ./blocks $bitcrystalpath
mv ./chainstate $bitcrystalpath
chmod 755 $bitcrystalpath/blocks -R
chmod 755 $bitcrystalpath/chainstate -R
chown $(whoami):$(whoami) $bitcrystalpath/blocks -R
chown $(whoami):$(whoami) $bitcrystalpath/chainstate -R
cp ./bootstrap.dat $bitcrystalpath
rm ./bootstrap.tar.gz 1> /dev/null 2> /dev/null
rm ./bootstrap.dat 1> /dev/null 2> /dev/null
chmod 755 $bitcrystalpath/bootstrap.dat
chown $(whoami):$(whoami) $bitcrystalpath/bootstrap.dat
