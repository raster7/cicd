#!/bin/bash

echo "micro_test_flags"

../s21_cat -vT main_test.txt > t_21cat.txt
cat -vT main_test.txt > t_cat.txt
diff -s t_21cat.txt t_cat.txt