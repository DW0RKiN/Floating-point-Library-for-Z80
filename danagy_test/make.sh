#!/bin/bash 

if [ $# -lt 1 ] || [ $# -gt 1 ] ; then 
  echo 
  echo " Need name test_file.asm "
  exit 1 
fi 

pasmo -I ../danagy -d ${1} 24576.bin > test.asm ; grep "BREAKPOINT" test.asm
