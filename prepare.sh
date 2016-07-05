#!/bin/bash
src/bitcrystald stop
rm src/obj/windows*.o
rm src/obj/rpcdump.o
rm src/obj/mhook*.o
src/bitcrystald stop
rm src/bitcrystald
./bitcrystal.all.linux.make.sh 2> n.txt 1> n.txt
