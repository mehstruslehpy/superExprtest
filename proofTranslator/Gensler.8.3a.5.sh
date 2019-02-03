#!/bin/bash
./translator-expect << ENDOFMESSAGE
~(Some x)(F(x)&G(x))
n
(All x)~F(x)
y
(All x)G(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
