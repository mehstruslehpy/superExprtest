#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)G(a,x,b)
y
(Some x)(Some y)G(x,c,y)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
