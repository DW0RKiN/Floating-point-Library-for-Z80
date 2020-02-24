#!/bin/sh 
# bash, dash, ksh, sh compatibility

printf "                    binary16    danagy      bfloat      use\n"
printf "                    --------    --------    --------    --------\n"

for b in fadd+fsub fadd fsub s fmul+fdiv fmul fdiv s fln fln_fix s fmod s fpow2 fsqrt s frac fint s fwld fwst fbld s all
do

    test ${b} = "s" && printf "\n" && continue

    a="${b}:" 
    while test ${#a} -lt "15" ; do
        a="${a} " 
    done
    printf "     ${a}"
            
    for a in ../binary16 ../danagy ../bfloat ; do
        pasmo -I ${a} -d size_${b}.asm test.bin > test.asm
        size=`find test.bin -ls | awk '{print $7}'`
        printf "%s" ${size}
        kolikrat=$((12-${#size}))
        while test $kolikrat -gt 0 ; do
            printf " "
            kolikrat=$((kolikrat-1))
        done
    done
    
    test ${b} = "fmulfdiv" && printf "fmul.tab + fdiv.tab"
    test ${b} = "fmul" && printf "${b}.tab"
    test ${b} = "fdiv" && printf "${b}.tab (include itself fmul.tab)"

    test ${b} = "fln" && printf "${b}.tab (fix_ln  EQU 0)"
    test ${b} = "fln_fix" && printf "fln.tab (fix_ln  EQU 1)"

    
    test ${b} = "fpow2" && printf "${b}.tab"
    test ${b} = "fsqrt" && printf "${b}.tab"

    
    printf "\n"

done
