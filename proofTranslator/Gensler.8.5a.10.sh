#!/bin/bash
./translator-expect << ENDOFMESSAGE
~(Some x)(F(x)&G(x))
n
~F(d)
y
G(d)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
