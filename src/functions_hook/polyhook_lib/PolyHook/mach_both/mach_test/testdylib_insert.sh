#!/bin/bash
rm testdylib.tar.gz 2> /dev/null 1> /dev/null
tar cvzf testdylib.tar.gz ../mach_override testdylib.h testdylib.c 2> /dev/null 1> /dev/null
my_file_to_pointer ./testdylib.tar.gz testdylib_tar_gz.c
out=$(ls -la testdylib.tar.gz | cut -d' ' -f8)
lf=$'\n';
n="\\$lf";
sed -i.bak s/"\ static"/"static"/g testdylib_tar_gz.c
sed -i.bak s/"\ char\ \*\ b\ =\ (char\*)malloc(\ ${out}\ );"/"\ static\ \*\ char\ b[${out}];$n\ if(x!=0)$n\ \ return\ b;$n\ $n"/g testdylib_tar_gz.c
sed -i.bak s/"static\ char\ \*\ lib_memory()"/"static\ unsigned\ long\ long\ file_testdylib_tar_gz_file_size()$n{$n\ return ${out};${n}}${n}${n}${n}static\ char\ \* \ file_testdylib_tar_gz_${out}()"/g testdylib_tar_gz.c
