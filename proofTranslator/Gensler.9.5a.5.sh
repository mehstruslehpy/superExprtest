#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(All y)L(x,y)
y
(All x)(All y)((F(x)&G(y))->L(x,y))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
