#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(All y)(L(x,y)->(F(x)&~F(y)))
y
(All x)(All y)(L(x,y)->~L(y,x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
