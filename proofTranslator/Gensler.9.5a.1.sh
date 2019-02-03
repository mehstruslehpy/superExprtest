#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)L(x,a)
y
(All x)L(a,x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
