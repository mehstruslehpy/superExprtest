#!/bin/bash
#need to figure out equals relation
./translator-expect << ENDOFMESSAGE
(All x)(All y)(L(x,y)->E(x,y))
y
(All x)L(a,a)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
