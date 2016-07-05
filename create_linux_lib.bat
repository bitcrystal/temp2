gcc -shared -o example_dll.dll example_dll.o -Wl,-no-undefined,--enable-runtime-pseudo-reloc,--out-implib,libexample_dll.a
