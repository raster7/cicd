#!/bin/bash

scp src/src_cat_grep/cat/s21_cat vm2@172.24.116.8:~/
scp src/src_cat_grep/grep/s21_grep vm2@172.24.116.8:~/
ssh vm2@172.24.116.8 "echo "1" | sudo -S mv s21_cat s21_grep /usr/local/bin"