#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)L(x,x)
y
(All x)(All y)(L(x,y)->E(x,y))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
