#!/bin/bash
./translator-expect << ENDOFMESSAGE
(Some x)(All y)L(x,y)
y
(Some x)L(x,a)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
