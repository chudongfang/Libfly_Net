#!/bin/bash
ctags -R .
make clean
make
./test_Tcpserver
#make clean
