#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)L(x,a)
n
(All x)(L(a,x)->E(x,b))
y
(All x)L(x,b)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
