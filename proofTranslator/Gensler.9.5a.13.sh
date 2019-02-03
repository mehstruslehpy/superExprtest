#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(All y)(Some z)((L(x,y)&L(y,z))->L(x,z))
n
(All x)(All y)(K(x,y)->L(y,x))
y
(All x)L(x,x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
