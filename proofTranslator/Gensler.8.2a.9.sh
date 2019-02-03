#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(F(x)->G(x))
n
(All x)(~F(x)->H(x))
y
(All x)(G(x)vH(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
