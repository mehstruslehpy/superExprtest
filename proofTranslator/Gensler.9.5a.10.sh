#!/bin/bash
./translator-expect << ENDOFMESSAGE
L(a,b)
n
L(b,c)
y
(Some x)(L(a,x)&L(x,c))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
