#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(Some y)L(x,y)
y
L(a,a)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
