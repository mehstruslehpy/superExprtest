#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(All y)(U(x,y)->L(x,y))
n
(All x)(Some y)U(x,y)
y
(All x)(Some y)L(x,y)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
