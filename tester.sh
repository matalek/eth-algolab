#!/bin/bash

exec=${1-a.out}

for test in ./tests/*.in; do
    base=${test::${#test}-3}
    ./$exec < $base.in | diff -bsq $base.out -
done
