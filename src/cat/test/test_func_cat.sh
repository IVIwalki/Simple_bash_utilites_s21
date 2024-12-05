#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
RED="$(tput setaf 1)"
GREEN="$(tput setaf 2)"
BASE="$(tput setaf 7)"
declare -a tests=(
    "s test_0_cat.txt"
    "s test_1_cat.txt"
    "s test_2_cat.txt"
    "s test_3_cat.txt"
    "-b test_0_cat.txt"
    "-b test_1_cat.txt"
    "-b test_2_cat.txt"
    "-b test_3_cat.txt"
    "-e test_0_cat.txt"
    "-e test_1_cat.txt"
    "-e test_2_cat.txt"
    "-e test_3_cat.txt"
    "-n test_0_cat.txt"
    "-n test_1_cat.txt"
    "-n test_2_cat.txt"
    "-n test_3_cat.txt"
    "-s test_0_cat.txt"
    "-s test_1_cat.txt"
    "-s test_2_cat.txt"
    "-s test_3_cat.txt"
    "-t test_0_cat.txt"
    "-t test_1_cat.txt"
    "-t test_2_cat.txt"
    "-t test_3_cat.txt"
    "-v test_0_cat.txt"
    "-v test_1_cat.txt"
    "-v test_2_cat.txt"
    "-v test_3_cat.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    RESULT="SUCCESS"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        RESULT="SUCCESS"
    else
      (( FAIL++ ))
        RESULT="FAIL"
    fi
    echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"
    rm test_s21_cat.log test_sys_cat.log
}

for i in "${tests[@]}"
do
    var="-"
    testing $i
done

for var1 in b n t v e s
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done


for var1 in b n t v e s
do
    for var2 in b n t v e s
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done


for var1 in b n t v e s
do
    for var2 in b n t v e s
    do
        for var3 in b n t v e s
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done


for var1 in b n t v e s
do
    for var2 in b n t v e s
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

for var1 in b n t v e s
do
    for var2 in b n t v e s
    do
        for var3 in b n t v e s
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "${RED}FAIL: ${FAIL}${BASE}"
echo "${GREEN}SUCCESS: ${SUCCESS}${BASE}"
echo "ALL: $COUNTER"