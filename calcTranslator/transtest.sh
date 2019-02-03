#!/bin/sh
FORMULA=$(./singlePrint.lisp)
echo $FORMULA > last_formula.txt
./translator-expect << ENDOFMESSAGE
$FORMULA
y
ENDOFMESSAGE
./output_expect.txt
echo "FORMULA:"$FORMULA
