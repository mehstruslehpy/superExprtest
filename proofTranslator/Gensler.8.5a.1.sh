#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(F(x)vG(x))
n
~F(a)
y
(Some x)G(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
