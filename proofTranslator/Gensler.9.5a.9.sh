#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(All y)L(x,y)
y
(Some x)L(a,x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
