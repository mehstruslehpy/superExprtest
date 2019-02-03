#!/bin/bash

echo "./translator-expect << MESSAGE" > transfile.txt

RANDCOUNT=$(( ( RANDOM % 4 )  + 1 ))

for i in `seq 1 $RANDCOUNT`; do
	RANDNUM=$(./singlePrint.lisp)
	echo $RANDNUM >> transfile.txt
	echo "n" >> transfile.txt

done
echo $(./singlePrint.lisp) >> transfile.txt
echo "y" >> transfile.txt
echo $(./singlePrint.lisp) >> transfile.txt
echo "MESSAGE" >> transfile.txt
