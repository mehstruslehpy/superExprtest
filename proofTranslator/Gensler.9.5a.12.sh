#!/bin/bash
./translator-expect << ENDOFMESSAGE
(Some x)L(x,a)
n
~L(a,a)
y
(Some x)(~E(a,x)&L(x,a))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
