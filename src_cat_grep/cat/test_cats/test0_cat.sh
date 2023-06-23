#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""
TEST_FILE="main_test.txt"

echo "main_test_flags"

for var in -b -e -n -s -t -v -E -T #--number-nonblank --number --squeeze-blank
do
          TEST="$var $TEST_FILE"
          echo "$TEST"
          ../s21_cat $TEST > s21_cat.txt
          cat $TEST > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              echo "$TEST" >> log_success.txt
              (( SUCCESS++ ))
            else
              echo "$TEST" >> log_fail.txt
              (( FAIL++ ))
          fi
          rm s21_cat.txt cat.txt
done

for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
  for var2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
  do
        if [ $var != $var2 ]
        then
          TEST="$var $var2 $TEST_FILE"
          echo "$TEST"
          ../s21_cat $TEST > s21_cat.txt
          cat $TEST > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              echo "$TEST" >> log_success.txt
              (( SUCCESS++ ))
            else
              echo "$TEST" >> log_fail.txt
              (( FAIL++ ))
          fi
          rm s21_cat.txt cat.txt
        fi
  done
done

for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
  for var2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
  do
      for var3 in -b -e -n -s -t #-v -E -T --number-nonblank --number --squeeze-blank
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST="$var $var2 $var3 $TEST_FILE"
          echo "$TEST"
          ../s21_cat $TEST > s21_cat.txt
          cat $TEST > cat.txt
          DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
          if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
            then
              echo "$TEST" >> log_success.txt
              (( SUCCESS++ ))
            else
              echo "$TEST" >> log_fail.txt
              (( FAIL++ ))
          fi
          rm s21_cat.txt cat.txt

        fi
      done
  done
done

for var in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
  for var2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
  do
      for var3 in -b -e -n -s -t #-v -E -T --number-nonblank --number --squeeze-blank
      do
          for var4 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
          do
            if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ] && [ $var != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
            then
              TEST="$var $var2 $var3 $var4 $TEST_FILE"
              echo "$TEST"
              ../s21_cat $TEST > s21_cat.txt
              cat $TEST > cat.txt
              DIFF_RES="$(diff -s s21_cat.txt cat.txt)"
              if [ "$DIFF_RES" == "Files s21_cat.txt and cat.txt are identical" ]
                then
                  echo "$TEST" >> log_success.txt
                  (( SUCCESS++ ))
                else
                  echo "$TEST" >> log_fail.txt
                  (( FAIL++ ))
              fi
              rm s21_cat.txt cat.txt

            fi
          done
      done
  done
done

echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"